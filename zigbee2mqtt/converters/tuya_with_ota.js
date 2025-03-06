let tuyaDefinitions = require("zigbee-herdsman-converters/devices/tuya");

// Support Z2M 2.1.3-1
if (tuyaDefinitions.definitions !== undefined) {
    tuyaDefinitions = tuyaDefinitions.definitions;
}


const tuyaModels = [
    "TS0001_switch_module",
    "TS0002_basic",
    "TS0002_limited",
    "TS0004_switch_module_2",
    "TS0011_switch_module",
    "TS0012",
    "TS0012_switch_module",
    "WHD02",
];

const definitions = [];


for (let definition of tuyaDefinitions) {
    if (tuyaModels.includes(definition.model)) {
        definitions.push(
            {
                ...definition,
                ota: true,
            }
        )
    }
}

module.exports = definitions;
