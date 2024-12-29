# Custom firmware for Tuya switch

A custom firmware for Tuya telink based switch module. Code is based on pvvx's [ZigbeeTLc](https://github.com/pvvx/ZigbeeTLc) firmware, huge thanks!

## Supported device

Currently only [TS0012](https://www.zigbee2mqtt.io/devices/TS0012_switch_module.html) is supported. 

## Why?

The main driver for this project was the following factory firmware bug: if one button is pressed, the device ignores clicks to other buttons for ~0.5 seconds. The most frustrating consequence is that pressing both buttons at the same time turns only one relay on.

## Features

- Detached mode, e.g. switch doesn't trigger relay but only generates events via Zigbee
- Long press for momentary switches
- Router mode (TS0012 is no neutral switch so factory firmware works as end device)
- Super fast reaction time (compared to the factory firmware)
- 5 quick presses to reset the device

## Building

Only on linux:

```
make install
make
```

## Flashing device

Firmware can be [flashed via OTA](./docs/ota_flash.md)

To switch between End Device and Router follow [this guide](./docs/change_device_type.md)

To flash via wire, follow [the instruction](./docs/ts0012_flashing_via_wire.md)

## Changelog

### v1.0.2

- Added way to reset the device by pressing any switch button 5 times in a row 
- Fixed support for ON_OFF, OFF_ON actions


### v1.0.3

- Added support of statup behaviour: ON, OFF, TOGGLE, PREVIOUS
- Added support of button actions: 'released', 'press', 'long_press'. This is only useful for momentary (doorbell-like) switches.


### v1.0.4

- Fixed bug that caused report to be sent every second


## Acknowledgements

- https://github.com/pvvx/ZigbeeTLc (firmware for telink based ATC) as this was base for this project
- https://github.com/doctor64/tuyaZigbee (firmware for some other Tuya Zigbee devices) for some helpful examples
- https://medium.com/@omaslyuchenko for **Hello Zigbee World** series, that contain very usefull reference on how to implement own Zigbee device 
