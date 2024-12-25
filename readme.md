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

## Building

Only on linux:

```
make install
make
```

## Flashing device

Firmware can be [flashed via OTA](./docs/ota_flash.md)

To flash via wire, follow [the instruction](./docs/ts0012_flashing_via_wire.md)


## Acknowledgements

- https://github.com/pvvx/ZigbeeTLc (firmware for telink based ATC) as this was base for this project
- https://github.com/doctor64/tuyaZigbee (firmware for some other Tuya Zigbee devices) for some helpful examples
- https://medium.com/@omaslyuchenko for **Hello Zigbee World** series, that contain very usefull reference on how to implement own Zigbee device 
