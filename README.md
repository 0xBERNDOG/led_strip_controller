# LED Strip Controller (Firmware)

<div align="center">

[![Build status](https://img.shields.io/github/actions/workflow/status/texruska/led_strip_controller/build.yml?style=flat-square)](https://github.com/texruska/led_strip_controller/actions)
[![Firmware version](https://img.shields.io/badge/fw%20version-pre--alpha-blue?style=flat-square)](https://github.com/texruska/led_strip_controller/releases)
[![Hardware revision](https://img.shields.io/badge/hw%20revision-4.0%20(not%20yet%20built)-red?style=flat-square)](https://projects.stevenburnett.co.uk/led_controller/)

</div>

This is the firmware for my addressable LED strip controller project. The aim is to control a strip of WS2812Bs with a set of pre-loaded animations.

[Development journal](https://projects.stevenburnett.co.uk/led_controller/)

## Coming Soon

The hardware for this project is about to undergo a significant rework, and so the firmware will be changing too. Work on the `3.x` branch has been abandoned.

### Phases

- [x] Exploratory work (nRF52 DK)
- [ ] Schematic
- [ ] PCB design
- [ ] Firmware development

### Planned Features

* LED strip control (WS2812B)
* Bluetooth control
* One-button physical interface
* Appears as USB mass storage for easy animation uploading
    - USB-C
* Li-ion battery charging and monitoring (designed for 18650s)
* Additional software tool for easy animation creation
