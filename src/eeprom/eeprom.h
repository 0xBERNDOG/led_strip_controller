#ifndef EEPROM_HEADER
#define EEPROM_HEADER

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void eeprom_init();

bool eeprom_is_ready();

size_t eeprom_write_bytes(const uint16_t address, uint8_t const *buffer,
                          const size_t num_bytes);

size_t eeprom_read_bytes(const uint16_t address, uint8_t *buffer,
                         const size_t num_bytes);

#endif