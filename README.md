# LED Strip Controller (Firmware)

[Development journal](https://projects.stevenburnett.co.uk/led_controller/)

## Hardware

Hardware revision targets are defined in `/targets/revXX.cmake`, starting from revision 3.0

| Revision   | Target   | MCU        |
| ---------- | -------- | ---------- |
| 3.0        | `rev30`  | ATMEGA32U4 |
| 3.1        | `rev31`  | ATMEGA32U4 |

### Rev 3.1 Hardware (latest)

| Function   | Part              |
| ---------- | ----------------- |
| MCU        | ATMEGA32U4        |
| USB-Serial | CP1202N (28 pin)  |
| EEPROM     | CAT24C128 (128KB) |
| Gas Gauge  | MAX17048G         |
| IMU        | MMA8652           |

## Initial Setup

Modify `env_setup.sh`
Change `TARGET`, `PORT` as appropriate
Then:

```
source env_setup.sh
```

## Build and Upload

```
./build.sh
./flash.sh
```
