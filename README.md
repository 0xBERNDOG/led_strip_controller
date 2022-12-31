# LED Strip Controller (Firmware)

[Development journal](https://projects.stevenburnett.co.uk/led_controller/)

## Hardware

Hardware revision targets are defined in `/targets/revXX.cmake`, starting from revision 3.0

| Revision   | Target   | MCU        |
| ---------- | -------- | ---------- |
| 3.0        | `rev30`  | ATMEGA32U4 |
| 3.1        | `rev31`  | ATMEGA32U4 |

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