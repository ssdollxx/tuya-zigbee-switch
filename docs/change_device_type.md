# How to Change Device Type

This guide explains how to change a device's type between **Router** and **End Device**. It only applies to devices running custom firmware. If you need to update from stock firmware, follow the [flash via OTA](./ota_flash.md) guide.

## Step 1: Set Up the Proper Index File

To flash the device, you'll need a special OTA update that can be applied to the latest firmware version. To register it in Zigbee2MQTT, download the appropriate index file:

  - [To make device a Router](https://github.com/romasku/tuya-zigbee-switch/raw/refs/heads/main/zigbee2mqtt/ota/index_router-FORCE.json)  
  - [To make device an End Device](https://github.com/romasku/tuya-zigbee-switch/raw/refs/heads/main/zigbee2mqtt/ota/index_end_device-FORCE.json)  

Then, update the end of the `configuration.yaml` file in Zigbee2MQTT as follows:

```yaml
ota:
  zigbee_ota_override_index_location: !NAME_OF_DOWNLOADED_INDEX_FILE!
```

Replace `!NAME_OF_DOWNLOADED_INDEX_FILE!` with the name of the index file you downloaded (either `index_TS0012_FORCE.json` or `index_TS0012_END_DEVICE_FORCE.json`).

## Step 2: Initiate the OTA Update

Find your device in the OTA tab, click **Check for New Updates**, and proceed with the update. Wait for the device to update.

## Step 3: Verify the New Device Type

Once the update is complete, re-interview the device by clicking the small "i" icon in the device description. Verify that the device type has changed.
