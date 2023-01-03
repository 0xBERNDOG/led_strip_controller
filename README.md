# LED Strip Controller (Firmware)

<p align="center">
![Build status](https://img.shields.io/github/actions/workflow/status/texruska/led_strip_controller/build.yml?style=flat-square)
![Firmware version](https://img.shields.io/badge/fw%20version-pre--alpha-blue?style=flat-square)
![Hardware revision](https://img.shields.io/badge/hw%20revision-3.1-red?style=flat-square)
</p>

This is the firmware for my addressable LED strip controller project. The aim is to control a strip of WS2812Bs with a set of pre-loaded animations.

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
