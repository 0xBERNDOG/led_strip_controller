#ifndef GPIO_HEADER
#define GPIO_HEADER

#include <stdbool.h>

typedef enum { GPIO_PIN_INPUT, GPIO_PIN_OUTPUT } GPIO_pin_mode;

/* define possible GPIO pins
it isn't guaranteed that every hardware rev will implement every pin */
typedef enum {
    // LEDs
    GPIO_LED_BLINKY,
    GPIO_LED_STRIP,

    // buttons
    GPIO_BUTTON_BRIGHTNESS,
    GPIO_BUTTON_ANIMATION,
    GPIO_BUTTON_MODE,

    // interrupts
    GPIO_INT_IMU,
    GPIO_INT_GASGAUGE,

    GPIO_NUM_STATES  // used to determine size of definitions array
} GPIO_pin;

void gpio_setup_pin(const GPIO_pin pin, const GPIO_pin_mode mode);
void gpio_write_pin(const GPIO_pin pin, const bool value);
void gpio_toggle_pin(const GPIO_pin pin);
bool gpio_read_pin(const GPIO_pin pin);

#endif