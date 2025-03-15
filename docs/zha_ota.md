# How to Flash via OTA in ZHA

**IMPORTANT**

ZHA flashing support is experimentat and wasn't properly tested. There was a report that it was done succesfully, but I didn't tested it yet. If unsure, please wait a bit until it's ready. If you already have flashed device, adding custom quirk is completly safe.

**IMPORTANT**  
This process may brick your device, as it has not been extensively tested. Be prepared to use a hardware flasher to restore your device if needed. Although it worked fine for me, I want to warn you about the potential risks.  

To follow these instructions is for **ZHA**. If you're using **zigbee2mqtt**, please follow [this guide](./ota_flash.md)

### Step 1: Download the necessary files

- [Custom quirk for new device](https://github.com/romasku/tuya-zigbee-switch/raw/refs/heads/main/zha/switch_quirk.py)

Please this file somewhere inside your HA config directory, for example to `custom_zha_quirks`.

### Step 2: Update the Configuration  


Choose if you want to use Router of EndDevice version of the firmware. Router device responds faster to requests, can increase Zigbee network strength, but can be unstable on no neutral device as it consumes more power. You can try both options, but it requires [a special OTA update](./change_device_type.md) to change the type. 

Add following entry to your `configuration.yaml` of Home Assistant:

```
zha:
  enable_quirks: true
  custom_quirks_path: ./custom_zha_quirks/
  zigpy_config:
    ota:
      extra_providers:
        - type: z2m
          url: https://raw.githubusercontent.com/romasku/tuya-zigbee-switch/refs/heads/main/zigbee2mqtt/ota/index_router.json
```

This adds OTA support (both to flash the stock device and to update flashed device) and custom quirk to add proper support of
custom firmware. If you decided to use EndDevice firmware, please use this link instead:

`https://raw.githubusercontent.com/romasku/tuya-zigbee-switch/refs/heads/main/zigbee2mqtt/ota/index_end_device.json`


### Step 3: OTA flash

After some time, and "update available" message should appear. Confirm OTA update, and after some time it should update. Device should rejoin automatically, but if it doesn't happen, reset it by pressing any switch 5+ times fast and the re-join it to your ZHA network. 

