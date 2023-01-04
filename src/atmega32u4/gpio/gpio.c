#include "io/gpio.h"
#include "atmega32u4/gpio/pin.h"
#include "gpio_map.h"

#include <stddef.h>
#include <stdint.h>

void gpio_setup_pin(const GPIO_pin pin, const GPIO_pin_mode mode) {
    const GPIO_pin_internals pin_internals = gpio_get_pin(pin);
    if (pin_internals.data_dir == NULL) {
        // pin map doesn't exist
        return;
    }

    switch (mode) {
        case GPIO_PIN_INPUT:
            *pin_internals.data_dir &= ~(1 << pin_internals.pin);
            break;
        case GPIO_PIN_OUTPUT:
            *pin_internals.data_dir |= (1 << pin_internals.pin);
            break;
    }
}

void gpio_write_pin(const GPIO_pin pin, const bool value) {
    const GPIO_pin_internals pin_internals = gpio_get_pin(pin);
    if (pin_internals.data_dir == NULL) {
        // pin map doesn't exist
        return;
    }

    if (value) {
        *pin_internals.port_out |= (1 << pin_internals.pin);
    } else {
        *pin_internals.port_out &= ~(1 << pin_internals.pin);
    }
}

void gpio_toggle_pin(const GPIO_pin pin) {
    const GPIO_pin_internals pin_internals = gpio_get_pin(pin);
    if (pin_internals.data_dir == NULL) {
        // pin map doesn't exist
        return;
    }

    *pin_internals.port_out ^= (1 << pin_internals.pin);
}

bool gpio_read_pin(const GPIO_pin pin) {
    const GPIO_pin_internals pin_internals = gpio_get_pin(pin);
    if (pin_internals.data_dir == NULL) {
        // pin map doesn't exist
        return false;
    }

    return (*pin_internals.port_in & (1 << pin_internals.pin)) ==
           (1 << pin_internals.pin);
}