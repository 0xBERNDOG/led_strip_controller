#include "board_config.h"
#include "io/i2c.h"
#include "io/blinky.h"

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>

uint8_t read_from_eeprom();

// todo: remove/move into eeprom files
#define EEPROM_ADDR 0x50

int main(void) {
    blinky_init();

    // Enable Global interrupts
    sei();

    i2c_init(I2C_FREQ);
    uint8_t data[] = {0x00, 0x00, 0x15};  // first 2 bytes are memory address
    i2c_write(EEPROM_ADDR, data, 3, true);

    // flash blinky to show that we're doing something
    blinky_off();
    uint8_t i;
    for (i = 0; i < 5; i++) {
        blinky_toggle();
        _delay_ms(50);
    }
    blinky_off();

    uint8_t duty_cycle = read_from_eeprom();
    while (1) {
        blinky_pwm(duty_cycle);
    }
}

uint8_t read_from_eeprom() {
    uint8_t memory_address[] = {0x00, 0x00};
    uint8_t data[1];
    i2c_write(EEPROM_ADDR, memory_address, 2, false);
    i2c_read(EEPROM_ADDR, data, 1, true);

    return data[0];
}