#include "io/blinky.h"
#include "io/gpio.h"
#include <stdbool.h>

void blinky_init() { gpio_setup_pin(GPIO_LED_BLINKY, GPIO_PIN_OUTPUT); }

void blinky_toggle() { gpio_toggle_pin(GPIO_LED_BLINKY); }

void blinky_on() { gpio_write_pin(GPIO_LED_BLINKY, true); }

void blinky_off() { gpio_write_pin(GPIO_LED_BLINKY, false); }

void blinky_pwm(uint8_t duty_cycle) {
    uint8_t i;
    for (i = 0; i < 255; i++) {
        if (i < duty_cycle) {
            blinky_on();
        } else {
            blinky_off();
        }
    }
}