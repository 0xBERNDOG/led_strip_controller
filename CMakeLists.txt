cmake_minimum_required(VERSION 3.16)

project(led_controller LANGUAGES C)

# load target-specific config
set(TARGET $ENV{TARGET})
include(targets/${TARGET}.cmake)
configure_file(targets/board_config.h.in ${PROJECT_BINARY_DIR}/board_config.h)

string(TOLOWER ${MCU} MCU_LOWERCASE)
string(TOLOWER ${EEPROM_TYPE} EEPROM_TYPE_LOWERCASE)
string(TOLOWER ${GASGAUGE_TYPE} GASGAUGE_TYPE_LOWERCASE)
string(TOLOWER ${IMU_TYPE} IMU_TYPE_LOWERCASE)

# common files
set(SRC_FILES
    src/main.c

    # io
    src/${MCU_LOWERCASE}/i2c.c
#    src/targets/${TARGET}/gpio.c
    src/${MCU_LOWERCASE}/gpio/gpio.c
    src/io/blinky.c

    # peripherals
    src/eeprom/${EEPROM_TYPE_LOWERCASE}.c
    src/gas_gauge/${GASGAUGE_TYPE_LOWERCASE}.c
    src/imu/${IMU_TYPE_LOWERCASE}.c
)

add_executable(${PROJECT_NAME} ${SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.elf)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        src
        ${CMAKE_BINARY_DIR}
        src/${MCU_LOWERCASE}
        src/targets/${TARGET}
)

# compiler/linker flags
target_compile_options(${PROJECT_NAME} PRIVATE -g -Wall -Wextra -std=c99 -pedantic-errors -mcall-prologues -mmcu=${MCU_LOWERCASE} -Os)
target_link_options(${PROJECT_NAME} PRIVATE -Wl,-gc-sections -Wl,-relax -mmcu=${MCU_LOWERCASE} -Os)

add_custom_target(hex ALL avr-objcopy -R .eeprom -O ihex ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
add_custom_target(upload sudo avrdude  -c ${PROGRAMMER} -P $ENV{PORT} -p ${MCU_LOWERCASE} -U flash:w:${PROJECT_NAME}.hex:i DEPENDS hex)
add_custom_target(memory_usage avr-size -C --mcu=${MCU_LOWERCASE} ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf DEPENDS hex)