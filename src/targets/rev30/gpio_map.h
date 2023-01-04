#ifndef GPIO_PIN_MAPPING_HEADER
#define GPIO_PIN_MAPPING_HEADER

#include "io/gpio.h"
#include "atmega32u4/gpio/pin.h"

#include <avr/io.h>

/* GPIO pin definitions */
const GPIO_pin_internals gpio_pins[GPIO_NUM_STATES] = {
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

GPIO_pin_internals gpio_get_pin(const GPIO_pin pin) { return gpio_pins[pin]; }

#endif