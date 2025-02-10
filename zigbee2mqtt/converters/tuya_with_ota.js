const tuyaDefinitions = require("zigbee-herdsman-converters/devices/tuya");

const tuyaModels = [
    "TS0002_limited",
    "TS0002_basic",
    "WHD02",
    "TS0011_switch_module",
    "TS0004_switch_module_2",
    "TS0012_switch_module",
    "TS0001_switch_module",
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
