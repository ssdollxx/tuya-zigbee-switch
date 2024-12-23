const {
    numeric,
    enumLookup,
    commandsOnOff,
    deviceEndpoints,
    onOff,
} = require("zigbee-herdsman-converters/lib/modernExtend");
const reporting = require("zigbee-herdsman-converters/lib/reporting");
const constants = require("zigbee-herdsman-converters/lib/constants");
const ota = require("zigbee-herdsman-converters/lib/ota");

const romasku = {
    switchMode: (name, endpointName) =>
        enumLookup({
            name,
            endpointName,
            lookup: { toggle: 0, momentary: 1, multifunction: 2 },
            cluster: "genOnOffSwitchCfg",
            attribute: { ID: 0xff00, type: 0x30 }, // Enum8
            description: "Select the type of switch connected to the device",
        }),
    relayMode: (name, endpointName) =>
        enumLookup({
            name,
            endpointName,
            lookup: { detached: 0, raise: 1, long_press: 2 },
            cluster: "genOnOffSwitchCfg",
            attribute: { ID: 0xff01, type: 0x30 }, // Enum8
            description: "When to trigger internal relay",
        }),
    relayIndex: (name, endpointName) =>
        enumLookup({
            name,
            endpointName,
            lookup: { relay_1: 1, relay_2: 2 },
            cluster: "genOnOffSwitchCfg",
            attribute: { ID: 0xff02, type: 0x20 }, // uint8
            description: "Which internal relay it should trigger",
        }),
    longPressDuration: (name, endpointName) =>
        numeric({
            name,
            endpointNames: [endpointName],
            cluster: "genOnOffSwitchCfg",
            attribute: { ID: 0xff03, type: 0x21 }, // uint16
            description: "What duration is considerd to be long press",
        }),
};

const definitions = [
    {
        zigbeeModel: ["TS0042-CUSTOM"],
        model: "TS0042-CUSTOM",
        vendor: "Tuya-CUSTOM",
        description: "Custom switch (romasku)",
        extend: [
            deviceEndpoints({ endpoints: { 1: 1, 2: 2, "left": 3, "right": 4 } }),
            onOff({ powerOnBehavior: false, endpointNames: ["left", "right"] }),
            commandsOnOff({ endpointNames: ["1", "2"] }),
            romasku.switchMode("switch_1_mode", "1"),
            romasku.relayMode("switch_1_relay_mode", "1"),
            romasku.relayIndex("switch_1_relay_index", "1"),
            romasku.longPressDuration("switch_1_long_press_duration", "1"),
            romasku.switchMode("switch_2_mode", "2"),
            romasku.relayMode("switch_2_relay_mode", "2"),
            romasku.relayIndex("switch_2_relay_index", "2"),
            romasku.longPressDuration("switch_2_long_press_duration", "2"),
        ],
        meta: { multiEndpoint: true },
        configure: async (device, coordinatorEndpoint, logger) => {
            const endpoint3 = device.getEndpoint(3);
            await reporting.onOff(endpoint3, {
                min: 0,
                max: constants.repInterval.MINUTE,
                change: 1,
            });
            const endpoint4 = device.getEndpoint(4);
            await reporting.onOff(endpoint4, {
                min: 0,
                max: constants.repInterval.MINUTE,
                change: 1,
            });
        },
        ota: ota.zigbeeOTA,
    },
];

module.exports = definitions;
