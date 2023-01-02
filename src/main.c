#include "board_config.h"
#include "eeprom/eeprom.h"
#include "io/blinky.h"

#include <stdint.h>
#include <util/delay.h>

int main(void) {
    blinky_init();
    eeprom_init();

    const uint16_t addr = 5;
    uint8_t data_write[] = {0x10, 0x30, 0x70, 0xAA, 0xFF};
    const uint8_t num_write_bytes = sizeof(data_write) / sizeof(uint8_t);
    size_t result_write = eeprom_write_bytes(addr, data_write, num_write_bytes);

    if (result_write != num_write_bytes) {
        // something went wrong
        while (1) {
            blinky_toggle();
            _delay_ms(100);
        }
    }

    const uint8_t num_read_bytes = num_write_bytes;
    uint8_t read_data[num_read_bytes];
    size_t result_read = eeprom_read_bytes(addr, read_data, num_read_bytes);

    if (result_read != num_read_bytes) {
        // something went wrong
        while (1) {
            blinky_toggle();
            _delay_ms(500);
        }
    } else {
        while (1) {
            for (size_t i = 0; i < num_read_bytes; i++) {
                const uint8_t duty_cycle = read_data[i];

                for (size_t j = 0; j < 500; j++) {
                    blinky_pwm(duty_cycle);
                }
            }
        }
    }
}