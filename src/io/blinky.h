#ifndef BLINKY_HEADER
#define BLINKY_HEADER

#include <stdint.h>

void blinky_init();
void blinky_toggle();
void blinky_on();
void blinky_off();
void blinky_pwm(uint8_t duty_cycle);

#endif