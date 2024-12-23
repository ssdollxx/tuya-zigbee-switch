const tuya = require("zigbee-herdsman-converters/lib/tuya");

const {
    numeric,
} = require("zigbee-herdsman-converters/lib/modernExtend");
const exposes = require("zigbee-herdsman-converters/lib/exposes");
const reporting = require("zigbee-herdsman-converters/lib/reporting");
const ota = require("zigbee-herdsman-converters/lib/ota");

const definitions = [
    {
        fingerprint: [
            {modelID: 'TS0012', manufacturerName: '_TZ3000_jl7qyupf'},
            {modelID: 'TS0012', manufacturerName: '_TZ3000_nPGIPl5D'},
            {modelID: 'TS0012', manufacturerName: '_TZ3000_kpatq5pq'},
            {modelID: 'TS0012', manufacturerName: '_TZ3000_ljhbw1c9'},
            {modelID: 'TS0012', manufacturerName: '_TZ3000_4zf0crgo'},
        ],
        model: 'TS0012_switch_module',
        vendor: 'Tuya',
        description: '2 gang switch module - (without neutral)',
        whiteLabel: [
            {vendor: 'AVATTO', model: '2gang N-ZLWSM01'},
            tuya.whitelabel('AVATTO', 'LZWSM16-2', '2 gang switch module - (without neutral)', ['_TZ3000_kpatq5pq', '_TZ3000_ljhbw1c9']),
        ],
        extend: [
            tuya.modernExtend.tuyaOnOff({switchType: true, endpoints: ['left', 'right']}),
            numeric({
                name: "image_type",
                cluster: "genOta",
                attribute: { ID: 0x0008, type: 0x21 }, // uint16
                description: "Image type",
            })
        ],
        endpoint: (device) => {
            return {left: 1, right: 2};
        },
        meta: {multiEndpoint: true},
        configure: async (device, coordinatorEndpoint) => {
            await tuya.configureMagicPacket(device, coordinatorEndpoint);
            await reporting.bind(device.getEndpoint(1), coordinatorEndpoint, ['genOnOff']);
            await reporting.bind(device.getEndpoint(2), coordinatorEndpoint, ['genOnOff']);
            device.powerSource = 'Mains (single phase)';
            device.save();
        },
        ota: ota.zigbeeOTA,
    }
];

module.exports = definitions;
