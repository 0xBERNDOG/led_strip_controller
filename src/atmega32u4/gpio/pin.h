#ifndef GPIO_INTERNALS_HEADER
#define GPIO_INTERNALS_HEADER

#include "io/gpio.h"
#include <stdint.h>

typedef struct GPIO_pin_internals {
    volatile uint8_t* data_dir;
    volatile uint8_t* port_in;
    volatile uint8_t* port_out;
    const uint8_t pin;
} GPIO_pin_internals;

GPIO_pin_internals gpio_get_pin(const GPIO_pin pin);

#endif