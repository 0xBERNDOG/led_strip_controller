// todo: add retries if eeprom is busy
#include "board_config.h"
#include "eeprom/eeprom.h"
#include "io/i2c.h"
#include "util/math.h"

#include <stddef.h>
#include <stdint.h>
#include <util/delay.h>

typedef struct {
    uint8_t page;
    uint8_t offset;
} eeprom_address;

typedef struct {
    uint8_t first_byte;
    uint8_t second_byte;
} eeprom_address_bytes;

static const uint8_t i2c_address = 0x50;
static const size_t num_pages = 256;
static const size_t page_size = 64;  // bytes
static const uint8_t max_num_retries = 5;

void eeprom_init() { i2c_init(); }

bool eeprom_is_ready() {
    /* sends a read request with no data
    EEPROM will NACK if not ready */
    i2c_result result = i2c_write(i2c_address, NULL, 0, true);
    return result == SUCCESS;
}

static eeprom_address convert_to_local(const uint16_t address) {
    /* convert a 16-bit address to an eeprom_address */
    return (eeprom_address){.page = address / page_size,
                            .offset = address % page_size};
}

static eeprom_address_bytes convert_to_address_bytes(
    const eeprom_address address) {
    /* convert a local eeprom_address to 16-bit address bits
    XXPPPPPP PPOOOOOO
    X = don't care
    P = page number (8 bits)
    O = offset within page (6 bits) */
    uint8_t first_byte = 0x3F & (address.page >> 2);  // 0x3F = 0b00111111
    uint8_t second_byte =
        (0x3F & address.offset) | (address.page << 6);  // 0x3F = 0b00111111

    return (eeprom_address_bytes){.first_byte = first_byte,
                                  .second_byte = second_byte};
}

static bool address_in_bounds(const eeprom_address address) {
    return (address.page < num_pages && address.offset < page_size);
}

static bool wait_for_eeprom_ready() {
    for (uint8_t i = 0; i < max_num_retries; i++) {
        if (eeprom_is_ready()) {
            return true;
        }
        _delay_ms(1);
    }

    // operation failed, too many retries
    return false;
}

static bool write_address(const eeprom_address address) {
    /* writes eeprom_address to eeprom in a separate transaction, used for
     * reading data */
    uint8_t address_buffer[2];
    const eeprom_address_bytes address_bytes =
        convert_to_address_bytes(address);

    address_buffer[0] = address_bytes.first_byte;
    address_buffer[1] = address_bytes.second_byte;

    i2c_result result = i2c_write(i2c_address, address_buffer, 2, false);
    return (result == SUCCESS);
}

static bool read_bytes(uint8_t *buffer, const size_t num_bytes) {
    /* reads bytes from eeprom into buffer from the address already stored in
     * the eeprom */
    i2c_result result = i2c_read(i2c_address, buffer, num_bytes, true);
    return (result == SUCCESS);
}

static bool write_page(const eeprom_address address, uint8_t const *buffer,
                       const size_t num_bytes, const bool send_stop) {
    /* writes up to a full page of bytes, from address to address + num_bytes
    returns true/false on success/failure */

    // copy address into first 2 bytes
    uint8_t buffer_with_address[num_bytes + 2];
    const eeprom_address_bytes eeprom_address_bytes =
        convert_to_address_bytes(address);

    buffer_with_address[0] = eeprom_address_bytes.first_byte;
    buffer_with_address[1] = eeprom_address_bytes.second_byte;

    // copy data into remaining bytes (up to an entire page)
    for (uint8_t i = 0; i < num_bytes; i++) {
        buffer_with_address[i + 2] = buffer[i];
    }

    i2c_result result =
        i2c_write(i2c_address, buffer_with_address, num_bytes + 2, send_stop);
    return (result == SUCCESS);
}

size_t eeprom_write_bytes(const uint16_t address, uint8_t const *buffer,
                          const size_t num_bytes) {
    /* writes num_bytes from buffer into EEPROM in a continuous block (across
    pages)
    will not wrap around the end of the EEPROM
    returns number of bytes written */
    if (buffer == NULL) {
        return 0;
    }

    if (num_bytes < 1) {
        return 0;
    }

    const eeprom_address start_address = convert_to_local(address);
    const eeprom_address end_address =
        convert_to_local(address + num_bytes - 1);
    if (!address_in_bounds(start_address) || !address_in_bounds(end_address)) {
        return 0;
    }

    if (!wait_for_eeprom_ready()) {
        // operation timed out
        return 0;
    }

    size_t num_bytes_written = 0;
    const size_t num_pages = MAX(num_bytes / page_size, 1);
    eeprom_address cur_address = start_address;

    bool is_last_page = false;
    for (size_t i = 0; i < num_pages; i++) {
        if (i == num_pages - 1) {
            is_last_page = true;
        }

        const size_t num_bytes_remaining = num_bytes - num_bytes_written;
        const size_t num_bytes_in_page =
            MIN(page_size - cur_address.offset, num_bytes_remaining);

        const bool result = write_page(cur_address, &buffer[i],
                                       num_bytes_in_page, is_last_page);

        if (!result) {
            /* something went wrong with the write */
            return num_bytes_written;
        }

        num_bytes_written += num_bytes_in_page;
        cur_address.page += 1;
        cur_address.offset = 0;
    }

    return num_bytes_written;
}

size_t eeprom_read_bytes(const uint16_t address, uint8_t *buffer,
                         const size_t num_bytes) {
    /* reads num_bytes from EEPROM into buffer
    will not wrap around the end of the eeprom
    returns number of bytes read */
    if (buffer == NULL) {
        return 0;
    }

    if (num_bytes < 1) {
        return 0;
    }

    const eeprom_address start_address = convert_to_local(address);
    const eeprom_address end_address =
        convert_to_local(address + num_bytes - 1);
    if (!address_in_bounds(start_address) || !address_in_bounds(end_address)) {
        return 0;
    }

    if (!wait_for_eeprom_ready()) {
        // operation timed out
        return 0;
    }

    eeprom_address cur_address = start_address;

    if (write_address(cur_address) && read_bytes(buffer, num_bytes)) {
        return num_bytes;
    }

    return 0;
}