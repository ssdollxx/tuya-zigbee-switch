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
            tuya.modernExtend.tuyaOnOff({switchType: true, endpoints: ['left', 'right']})
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
        ota: true,
    },
    {
        fingerprint: [
            {modelID: 'TS0001', manufacturerName: '_TZ3000_skueekg3'},
        ],
        model: 'WHD02',
        vendor: 'Tuya',
        whiteLabel: [
            {vendor: 'Tuya', model: 'iHSW02'},
            {vendor: 'Aubess', model: 'TMZ02'},
            tuya.whitelabel('Tuya', 'QS-zigbee-S08-16A-RF', 'Wall switch module', ['_TZ3000_dlhhrhs8']),
        ],
        description: 'Wall switch module',
        extend: [tuya.modernExtend.tuyaOnOff({switchType: true, onOffCountdown: true})],
        configure: async (device, coordinatorEndpoint) => {
            await tuya.configureMagicPacket(device, coordinatorEndpoint);
            const endpoint = device.getEndpoint(1);
            await reporting.bind(endpoint, coordinatorEndpoint, ['genOnOff']);
            await reporting.onOff(endpoint);
        },
        ota: true,
    },
    {
        // TS0002 model with only on/off capability
        fingerprint: tuya.fingerprint('TS0002', [
            '_TZ3000_01gpyda5',
            '_TZ3000_bvrlqyj7',
            '_TZ3000_7ed9cqgi',
            '_TZ3000_zmy4lslw',
            '_TZ3000_ruxexjfz',
            '_TZ3000_4xfqlgqo',
            '_TZ3000_hojntt34',
            '_TZ3000_eei0ubpy',
            '_TZ3000_qaa59zqd',
            '_TZ3000_lmlsduws',
            '_TZ3000_lugaswf8',
            '_TZ3000_fbjdkph9',
        ]),
        model: 'TS0002_basic',
        vendor: 'Tuya',
        description: '2 gang switch module',
        whiteLabel: [
            {vendor: 'OXT', model: 'SWTZ22'},
            {vendor: 'Moes', model: 'ZM-104B-M'},
            tuya.whitelabel('pcblab.io', 'RR620ZB', '2 gang Zigbee switch module', ['_TZ3000_4xfqlgqo']),
            tuya.whitelabel('Nous', 'L13Z', '2 gang switch', ['_TZ3000_ruxexjfz', '_TZ3000_hojntt34']),
            tuya.whitelabel('Tuya', 'ZG-2002-RF', 'Three mode Zigbee Switch', ['_TZ3000_lugaswf8']),
            tuya.whitelabel('Mercator Ikuü', 'SSW02', '2 gang switch', ['_TZ3000_fbjdkph9']),
        ],
        extend: [tuya.modernExtend.tuyaOnOff({switchType: true, endpoints: ['l1', 'l2']})],
        endpoint: (device) => {
            return {l1: 1, l2: 2};
        },
        meta: {multiEndpoint: true},
        configure: async (device, coordinatorEndpoint) => {
            await tuya.configureMagicPacket(device, coordinatorEndpoint);
            await reporting.bind(device.getEndpoint(1), coordinatorEndpoint, ['genOnOff']);
            await reporting.bind(device.getEndpoint(2), coordinatorEndpoint, ['genOnOff']);
        },
        ota: true,
    },
    {
        fingerprint: [
            {modelID: 'TS0011', manufacturerName: '_TZ3000_qmi1cfuq'},
            {modelID: 'TS0011', manufacturerName: '_TZ3000_txpirhfq'},
            {modelID: 'TS0011', manufacturerName: '_TZ3000_ji4araar'},
        ],
        model: 'TS0011_switch_module',
        vendor: 'Tuya',
        description: '1 gang switch module - (without neutral)',
        extend: [tuya.modernExtend.tuyaOnOff({switchType: true})],
        whiteLabel: [
            {vendor: 'AVATTO', model: '1gang N-ZLWSM01'},
            {vendor: 'SMATRUL', model: 'TMZ02L-16A-W'},
            {vendor: 'Aubess', model: 'TMZ02L-16A-B'},
        ],
        configure: async (device, coordinatorEndpoint) => {
            await tuya.configureMagicPacket(device, coordinatorEndpoint);
            await reporting.bind(device.getEndpoint(1), coordinatorEndpoint, ['genOnOff']);
            device.powerSource = 'Mains (single phase)';
            device.save();
        },
        ota: true,
    },
];

module.exports = definitions;
