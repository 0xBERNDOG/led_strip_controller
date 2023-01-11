#include "board_config.h"
#include "io/blinky.h"
#include "io/gpio.h"
#include <util/delay.h>

int main(void) {
    blinky_init();
    /*
        while (1) {
            blinky_toggle();
            _delay_ms(250);
        }*/

    gpio_setup_pin(GPIO_BUTTON_ANIMATION, GPIO_PIN_INPUT);
    gpio_setup_pin(GPIO_BUTTON_BRIGHTNESS, GPIO_PIN_INPUT);
    gpio_setup_pin(GPIO_BUTTON_MODE, GPIO_PIN_INPUT);

    while (1) {
        const bool button_animation_value =
            gpio_read_pin(GPIO_BUTTON_ANIMATION);
        const bool button_brightness_value =
            gpio_read_pin(GPIO_BUTTON_BRIGHTNESS);
        const bool button_mode_value = gpio_read_pin(GPIO_BUTTON_MODE);

        if (button_mode_value) {
            blinky_pwm(25);
        } else if (button_animation_value) {
            blinky_pwm(125);
        } else if (button_brightness_value) {
            blinky_pwm(255);
        } else {
            blinky_off();
        }
    }
}