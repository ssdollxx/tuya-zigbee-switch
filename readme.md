# Custom firmware for Tuya switch

A custom firmware for Tuya telink based switch module. Code is based on pvvx's [ZigbeeTLc](https://github.com/pvvx/ZigbeeTLc) firmware, huge thanks!

## Supported devices

Note that rebranded version may have different internals and may not work. "Zigbee Manufacturer" is more reliable identifier of device.

- [Tuya WHD02](https://www.zigbee2mqtt.io/devices/WHD02.html) is supported. Tested only for `_TZ3000_skueekg3`. 
- [TS0001](https://www.zigbee2mqtt.io/devices/TS0001_switch_module.html) is supported. Should work for `_TZ3000_tqlv4ug4`.
- [TS0002](https://www.zigbee2mqtt.io/devices/TS0001_switch_module.html) is supported. Should work for `_TZ3000_zmy4lslw`.   
- [TS0011](https://www.zigbee2mqtt.io/devices/TS0012_switch_module.html) is supported. 
- [TS0012](https://www.zigbee2mqtt.io/devices/TS0012_switch_module.html) is supported. 

If you device is not supported, but it is some Tuya switch module, please check [the porting guide](./docs/porting_to_new_device.md).

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

Firmware can be [flashed via OTA](./docs/ota_flash.md). If you still use zigbee2mqtt 1.x, use [this guide](./docs/ota_flash_z2m_v1.md)

To switch between End Device and Router follow [this guide](./docs/change_device_type.md)

To flash via wire, follow [this guide](./docs/ts0012_flashing_via_wire.md)

## Changelog

### v1.0.6

- Add way to change device pinout on the fly, to allow easier porting of firmware 

### v1.0.5

- Keep status LED on when device is connected
- Add separate firmwares for End Device/Router
- Improve device boot time significantly by removing unnecessary logs 

### v1.0.4

- Fix bug that caused report to be sent every second

### v1.0.3

- Add support of statup behaviour: ON, OFF, TOGGLE, PREVIOUS
- Add support of button actions: 'released', 'press', 'long_press'. This is only useful for momentary (doorbell-like) switches.

### v1.0.2

- Add way to reset the device by pressing any switch button 5 times in a row 
- Fix support for ON_OFF, OFF_ON actions


## Acknowledgements

- https://github.com/pvvx/ZigbeeTLc (firmware for telink based ATC) as this was base for this project
- https://github.com/doctor64/tuyaZigbee (firmware for some other Tuya Zigbee devices) for some helpful examples
- https://medium.com/@omaslyuchenko for **Hello Zigbee World** series, that contain very usefull reference on how to implement own Zigbee device 
