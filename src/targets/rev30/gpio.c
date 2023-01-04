#include "io/gpio.h"
#include <avr/io.h>
#include <stddef.h>
#include <stdint.h>

typedef struct GPIO_pin_internals {
    volatile uint8_t* data_dir;
    volatile uint8_t* port_in;
    volatile uint8_t* port_out;
    const uint8_t pin;
} GPIO_pin_internals;

/* GPIO pin definitions */
static const GPIO_pin_internals gpio_pins[GPIO_NUM_STATES] = {
    // LEDs
    [GPIO_LED_BLINKY] = {.data_dir = &DDRD,
                         .port_in = &PIND,
                         .port_out = &PORTD,
                         .pin = 7},
    [GPIO_LED_STRIP] = {.data_dir = &DDRD,
                        .port_in = &PIND,
                        .port_out = &PORTD,
                        .pin = 6},

    // buttons
    [GPIO_BUTTON_BRIGHTNESS] = {.data_dir = &DDRD,
                                .port_in = &PIND,
                                .port_out = &PORTD,
                                .pin = 2},
    [GPIO_BUTTON_ANIMATION] = {.data_dir = &DDRB,
                               .port_in = &PINB,
                               .port_out = &PORTB,
                               .pin = 4},
    [GPIO_BUTTON_MODE] = {.data_dir = &DDRB,
                          .port_in = &PINB,
                          .port_out = &PORTB,
                          .pin = 5},

    // interrupts
    [GPIO_INT_IMU] = {.data_dir = &DDRD,
                      .port_in = &PIND,
                      .port_out = &PORTD,
                      .pin = 3},
    [GPIO_INT_GASGAUGE] = {
        .data_dir = &DDRB, .port_in = &PINB, .port_out = &PORTB, .pin = 6}};

GPIO_pin_internals get_pin(const GPIO_pin pin) { return gpio_pins[pin]; }

void gpio_setup_pin(const GPIO_pin pin, const GPIO_pin_mode mode) {
    const GPIO_pin_internals pin_internals = get_pin(pin);
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
    const GPIO_pin_internals pin_internals = get_pin(pin);
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
    const GPIO_pin_internals pin_internals = get_pin(pin);
    if (pin_internals.data_dir == NULL) {
        // pin map doesn't exist
        return;
    }

    *pin_internals.port_out ^= (1 << pin_internals.pin);
}

bool gpio_read_pin(const GPIO_pin pin) {
    const GPIO_pin_internals pin_internals = get_pin(pin);
    if (pin_internals.data_dir == NULL) {
        // pin map doesn't exist
        return false;
    }

    return (*pin_internals.port_in & (1 << *pin_internals.port_in)) ==
           (1 << *pin_internals.port_in);
}