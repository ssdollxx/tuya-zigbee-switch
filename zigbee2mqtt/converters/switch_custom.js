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
    switchAction: (name, endpointName) =>
        enumLookup({
            name,
            endpointName,
            lookup: { on_off: 0, off_on: 1, toggle: 2 },
            cluster: "genOnOffSwitchCfg",
            attribute: "switchActions", // Enum8
            description: `Select how switch should work:
            - on_off: When switch physically moved to position 1 it always generates ON command, and when moved to position 2 it generates OFF command
            - off_on: Same as on_off, but positions are swapped
            - toggle: Generate TOGGLE command for any press of physicall switch`,
        }),
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
            valueMin: 0,
            valueMax: 5000,
        }),
    pressAction: (name, endpointName) =>
        enumLookup({
            name,
            endpointName,
            access: "STATE_GET",
            lookup: { released: 0, press: 1, long_press: 2 },
            cluster: "genMultistateInput",
            attribute: "presentValue",
            description: "Action of the switch: 'released' or 'press' or 'long_press'",
        }),
};

const definitions = [
    {
        zigbeeModel: [
            "TS0042-CUSTOM",
            "TS0012-custom",
        ],
        model: "TS0012-custom",
        vendor: "Tuya-custom",
        description: "Custom switch (https://github.com/romasku/tuya-zigbee-switch)",
        extend: [
            deviceEndpoints({ endpoints: { 1: 1, 2: 2, "left": 3, "right": 4 } }),
            onOff({ endpointNames: ["left", "right"] }),
            romasku.pressAction("switch_1_press_action", "1"),
            romasku.switchMode("switch_1_mode", "1"),
            romasku.switchAction("switch_1_action_mode", "1"),
            romasku.relayMode("switch_1_relay_mode", "1"),
            romasku.relayIndex("switch_1_relay_index", "1"),
            romasku.longPressDuration("switch_1_long_press_duration", "1"),
            romasku.pressAction("switch_2_press_action", "2"),
            romasku.switchMode("switch_2_mode", "2"),
            romasku.switchAction("switch_2_action_mode", "2"),
            romasku.relayMode("switch_2_relay_mode", "2"),
            romasku.relayIndex("switch_2_relay_index", "2"),
            romasku.longPressDuration("switch_2_long_press_duration", "2"),
        ],
        meta: { multiEndpoint: true },
        configure: async (device, coordinatorEndpoint, logger) => {
            await reporting.bind(device.getEndpoint(1), coordinatorEndpoint, ["genMultistateInput"]);
            await reporting.bind(device.getEndpoint(2), coordinatorEndpoint, ["genMultistateInput"]);
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
        ota: true,
    },
    {
        zigbeeModel: ['TS0001-custom'],
        model: 'TS0001-custom',
        vendor: 'Tuya-custom',
        description: "Custom switch (https://github.com/romasku/tuya-zigbee-switch)",
        extend: [
            deviceEndpoints({ endpoints: { 1: 1, "relay": 2} }),
            onOff({ endpointNames: ["relay"] }),
            commandsOnOff({ endpointNames: ["1"] }),
            romasku.pressAction("switch_press_action", "1"),
            romasku.switchMode("switch_mode", "1"),
            romasku.switchAction("switch_action_mode", "1"),
            romasku.relayMode("switch_relay_mode", "1"),
            romasku.longPressDuration("switch_long_press_duration", "1"),
        ],
        meta: { multiEndpoint: true },
        configure: async (device, coordinatorEndpoint, logger) => {
            await reporting.bind(device.getEndpoint(1), coordinatorEndpoint, ["genMultistateInput"]);
            const endpoint2 = device.getEndpoint(2);
            await reporting.onOff(endpoint2, {
                min: 0,
                max: constants.repInterval.MINUTE,
                change: 1,
            });
        },
        ota: true,
    },
    {
        zigbeeModel: [
            "TS0002-custom",
        ],
        model: "TS0002-custom",
        vendor: "Tuya-custom",
        description: "Custom switch (https://github.com/romasku/tuya-zigbee-switch)",
        extend: [
            deviceEndpoints({ endpoints: { 1: 1, 2: 2, "left": 3, "right": 4 } }),
            onOff({ endpointNames: ["left", "right"] }),
            commandsOnOff({ endpointNames: ["1", "2"] }),
            romasku.pressAction("switch_1_press_action", "1"),
            romasku.switchMode("switch_1_mode", "1"),
            romasku.switchAction("switch_1_action_mode", "1"),
            romasku.relayMode("switch_1_relay_mode", "1"),
            romasku.relayIndex("switch_1_relay_index", "1"),
            romasku.longPressDuration("switch_1_long_press_duration", "1"),
            romasku.pressAction("switch_2_press_action", "2"),
            romasku.switchMode("switch_2_mode", "2"),
            romasku.switchAction("switch_2_action_mode", "2"),
            romasku.relayMode("switch_2_relay_mode", "2"),
            romasku.relayIndex("switch_2_relay_index", "2"),
            romasku.longPressDuration("switch_2_long_press_duration", "2"),
        ],
        meta: { multiEndpoint: true },
        configure: async (device, coordinatorEndpoint, logger) => {
            await reporting.bind(device.getEndpoint(1), coordinatorEndpoint, ["genMultistateInput"]);
            await reporting.bind(device.getEndpoint(2), coordinatorEndpoint, ["genMultistateInput"]);
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
        ota: true,
    },
    {
        zigbeeModel: ['TS0011-custom'],
        model: 'TS0011-custom',
        vendor: 'Tuya-custom',
        description: "Custom switch (https://github.com/romasku/tuya-zigbee-switch)",
        extend: [
            deviceEndpoints({ endpoints: { 1: 1, "relay": 2} }),
            onOff({ endpointNames: ["relay"] }),
            commandsOnOff({ endpointNames: ["1"] }),
            romasku.pressAction("switch_press_action", "1"),
            romasku.switchMode("switch_mode", "1"),
            romasku.switchAction("switch_action_mode", "1"),
            romasku.relayMode("switch_relay_mode", "1"),
            romasku.longPressDuration("switch_long_press_duration", "1"),
        ],
        meta: { multiEndpoint: true },
        configure: async (device, coordinatorEndpoint, logger) => {
            await reporting.bind(device.getEndpoint(1), coordinatorEndpoint, ["genMultistateInput"]);
            const endpoint2 = device.getEndpoint(2);
            await reporting.onOff(endpoint2, {
                min: 0,
                max: constants.repInterval.MINUTE,
                change: 1,
            });
        },
        ota: true,
    },
];

module.exports = definitions;
