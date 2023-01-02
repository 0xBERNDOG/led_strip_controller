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

I use the [Pololu USB AVR Programmer v2.1](https://www.pololu.com/product/3172). You may need to modify the build pipeline as appropriate for your set of tools.

```
./build.sh
./flash.sh
```
