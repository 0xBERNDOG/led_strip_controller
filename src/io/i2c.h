#ifndef I2C_HEADER
#define I2C_HEADER

#include <stdbool.h>
#include <stdint.h>

typedef enum { SUCCESS, FAILURE } i2c_result;

void i2c_init(const uint32_t i2c_freq);

i2c_result i2c_write(const uint8_t address, uint8_t const *buffer,
                     uint8_t num_bytes, bool send_stop);

i2c_result i2c_read(const uint8_t address, uint8_t *buffer, uint8_t num_bytes,
                    bool send_stop);

#endif