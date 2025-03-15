# How to Flash via OTA in ZHA  

**IMPORTANT**  
ZHA flashing support is experimental and hasn't been properly tested. There was a report that it was done successfully, but I haven't tested it yet. If unsure, please wait until it's fully ready. If you already have a flashed device, adding a custom quirk is completely safe.  

**IMPORTANT**  
This process may brick your device, as it has not been extensively tested. Be prepared to use a hardware flasher to restore your device if needed. Although it worked fine for me, I want to warn you about the potential risks.  

These instructions are for **ZHA**. If you're using **zigbee2mqtt**, please follow [this guide](./ota_flash.md).  

### Step 1: Download the necessary files  

- [Custom quirk for the new device](https://github.com/romasku/tuya-zigbee-switch/raw/refs/heads/main/zha/switch_quirk.py)  

Place this file somewhere inside your HA config directory, for example, in `./custom_zha_quirks`.  

### Step 2: Update the Configuration  

Choose whether you want to use the Router or EndDevice version of the firmware. A Router device responds faster to requests, can increase Zigbee network strength, but may be unstable on a no-neutral device as it consumes more power. You can try both options, but switching requires [a special OTA update](./change_device_type.md).  

Add the following entry to your `configuration.yaml` in Home Assistant:  

```yaml
zha:
  enable_quirks: true
  custom_quirks_path: ./custom_zha_quirks/
  zigpy_config:
    ota:
      extra_providers:
        - type: z2m
          url: https://raw.githubusercontent.com/romasku/tuya-zigbee-switch/refs/heads/main/zigbee2mqtt/ota/index_router.json
```

This adds OTA support (both to flash the stock device and to update the flashed device) and a custom quirk for proper support of the custom firmware. If you decide to use EndDevice firmware, use this link instead:  

`https://raw.githubusercontent.com/romasku/tuya-zigbee-switch/refs/heads/main/zigbee2mqtt/ota/index_end_device.json`  

### Step 3: OTA Flash  

After some time, an "update available" message should appear. Confirm the OTA update, and after a while, it should complete. The device should rejoin automatically, but if that doesn’t happen, reset it by pressing any switch 5+ times quickly, then rejoin it to your ZHA network.  
