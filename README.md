# LED Strip Controller (Firmware)

[Development journal](https://projects.stevenburnett.co.uk/led_controller/)

## Hardware

Hardware revision targets are defined in `/targets/revXX.cmake`, starting from revision 3.0

[Hardware info](docs/hardware.md)

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
