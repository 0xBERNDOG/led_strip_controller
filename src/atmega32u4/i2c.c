#include "board_config.h"
#include "io/i2c.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include <util/twi.h>

#define I2C_SEND_START() \
    (TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE))
#define I2C_SEND_START_NO_INT() \
    (TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN))
#define I2C_SEND_STOP() \
    (TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN) | (1 << TWIE))
#define I2C_CONTINUE_TX() (TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE))
#define I2C_SEND_ACK() \
    (TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA))
#define I2C_SEND_NACK() (TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE))

#define I2C_BUFFER_LENGTH 32
static uint8_t i2c_buffer[I2C_BUFFER_LENGTH];
static volatile uint8_t i2c_buffer_index;
static volatile uint8_t i2c_buffer_length;
static volatile uint8_t i2c_slawr;

typedef enum { READY, MASTER_TX, MASTER_RX } i2c_state;
static volatile i2c_state i2c_current_state;
static volatile i2c_result i2c_current_result;
static volatile bool i2c_send_stop;
static volatile bool i2c_in_repeated_start;

void i2c_init() {
    static bool init = false;
    if (init) {
        // already initialised
        return;
    }

    // enable interrupts
    sei();

    // initialise internal state
    i2c_current_state = READY;
    i2c_send_stop = false;
    i2c_in_repeated_start = false;

    // set prescaler (1x) and bitrate
    TWSR &= ~(1 << TWPS0);
    TWSR &= ~(1 << TWPS1);
    TWBR = (F_CPU / (8 * I2C_FREQ)) - 2;

    // enable I2C, interrupts, and acks
    TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWEA);

    init = true;
}

i2c_result i2c_write(const uint8_t address, uint8_t const *buffer,
                     const size_t num_bytes, const bool send_stop) {
    if (num_bytes > I2C_BUFFER_LENGTH) {
        return FAILURE;
    }

    while (i2c_current_state != READY) {
        continue;
    }

    i2c_current_state = MASTER_TX;
    i2c_send_stop = send_stop;

    i2c_buffer_index = 0;
    i2c_buffer_length = num_bytes;

    if (buffer != NULL) {
        // buffer is allowed to be NULL when data is being thrown away (e.g.
        // when sending a signal to a device)
        for (size_t i = 0; i < num_bytes; i++) {
            i2c_buffer[i] = buffer[i];
        }
    }

    i2c_slawr = TW_WRITE | (address << 1);

    if (i2c_in_repeated_start) {
        i2c_in_repeated_start = false;
        TWDR = i2c_slawr;
        I2C_SEND_ACK();
    } else {
        I2C_SEND_START();
    }

    while (i2c_current_state == MASTER_TX) {
        continue;
    }

    return i2c_current_result;
}

i2c_result i2c_read(const uint8_t address, uint8_t *buffer,
                    const size_t num_bytes, const bool send_stop) {
    if (num_bytes > I2C_BUFFER_LENGTH) {
        return FAILURE;
    }

    while (i2c_current_state != READY) {
        continue;
    }

    i2c_current_state = MASTER_RX;
    i2c_send_stop = send_stop;

    i2c_buffer_index = 0;
    i2c_buffer_length = num_bytes - 1;

    i2c_slawr = TW_READ | (address << 1);

    if (i2c_in_repeated_start) {
        i2c_in_repeated_start = false;
        TWDR = i2c_slawr;
        I2C_SEND_ACK();
    } else {
        I2C_SEND_START();
    }

    while (i2c_current_state == MASTER_RX) {
        continue;
    }

    size_t num_bytes_read = num_bytes;
    if (i2c_buffer_index < num_bytes) {
        num_bytes_read = i2c_buffer_index;
    }

    if (buffer != NULL) {
        // buffer is allowed to be NULL when data is being thrown away (e.g.
        // when sending a signal to a device)
        size_t i;
        for (i = 0; i < num_bytes_read; i++) {
            buffer[i] = i2c_buffer[i];
        }
    }

    return i2c_current_result;
}

ISR(TWI_vect) {
    switch (TW_STATUS) {
        case TW_START:
        case TW_REP_START: {
            TWDR = i2c_slawr;
            I2C_SEND_ACK();
            break;
        }

        // master transmitter
        case TW_MT_SLA_ACK:
        case TW_MT_DATA_ACK: {
            if (i2c_buffer_index < i2c_buffer_length) {
                TWDR = i2c_buffer[i2c_buffer_index++];
                I2C_SEND_ACK();
            } else {
                if (i2c_send_stop) {
                    i2c_current_state = READY;
                    i2c_current_result = SUCCESS;
                    I2C_SEND_STOP();
                } else {
                    i2c_in_repeated_start = true;
                    i2c_current_state = READY;
                    i2c_current_result = SUCCESS;
                    I2C_SEND_START_NO_INT();
                }
            }
            break;
        }

        case TW_MT_SLA_NACK:
        case TW_MT_DATA_NACK: {
            i2c_current_state = READY;
            i2c_current_result = FAILURE;
            I2C_SEND_STOP();
            break;
        }

        case TW_MT_ARB_LOST: {
            i2c_current_state = READY;
            i2c_current_result = FAILURE;
            I2C_SEND_ACK();
            break;
        }

        // master receiver
        case TW_MR_DATA_ACK:
            i2c_buffer[i2c_buffer_index++] = TWDR;
        case TW_MR_SLA_ACK: {
            if (i2c_buffer_index < i2c_buffer_length) {
                I2C_SEND_ACK();
            } else {
                I2C_SEND_NACK();
            }
            break;
        }

        case TW_MR_DATA_NACK: {
            i2c_buffer[i2c_buffer_index++] = TWDR;
            if (i2c_send_stop) {
                i2c_current_state = READY;
                i2c_current_result = SUCCESS;
                I2C_SEND_STOP();
            } else {
                i2c_in_repeated_start = true;
                i2c_current_state = READY;
                i2c_current_state = SUCCESS;
                I2C_SEND_START_NO_INT();
            }
            break;
        }

        case TW_MR_SLA_NACK: {
            i2c_current_state = READY;
            i2c_current_result = FAILURE;
            I2C_SEND_STOP();
            break;
        }

        // misc
        case TW_NO_INFO:
            break;

        case TW_BUS_ERROR: {
            i2c_current_state = READY;
            i2c_current_result = FAILURE;
            I2C_SEND_STOP();
            break;
        }
    }
}