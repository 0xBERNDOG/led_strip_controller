#include "board_config.h"
#include "io/blinky.h"
#include <util/delay.h>

int main(void) {
    blinky_init();

    while (1) {
        blinky_toggle();
        _delay_ms(100);
    }
}