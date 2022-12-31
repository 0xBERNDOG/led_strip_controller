#include "io/blinky.h"
#include <avr/io.h>

// todo: proper gpio/pin mapping library
#define BLINKY_MODE_PORT DDRD
#define BLINKY_PORT PORTD
#define BLINKY_PIN 7

void blinky_init() { BLINKY_MODE_PORT |= (1 << BLINKY_PIN); }

void blinky_toggle() { BLINKY_PORT ^= (1 << BLINKY_PIN); }

void blinky_on() { BLINKY_PORT |= (1 << BLINKY_PIN); }

void blinky_off() { BLINKY_PORT &= ~(1 << BLINKY_PIN); }

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