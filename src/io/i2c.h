#ifndef I2C_HEADER
#define I2C_HEADER

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum { SUCCESS, FAILURE } i2c_result;

void i2c_init();

i2c_result i2c_write(const uint8_t address, uint8_t const *buffer,
                     const size_t num_bytes, const bool send_stop);

i2c_result i2c_read(const uint8_t address, uint8_t *buffer,
                    const size_t num_bytes, const bool send_stop);

#endif