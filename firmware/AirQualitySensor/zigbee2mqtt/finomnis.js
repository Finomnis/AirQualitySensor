const exposes = require('zigbee-herdsman-converters/lib/exposes');
const fz = require('zigbee-herdsman-converters/converters/fromZigbee');
const reporting = require('zigbee-herdsman-converters/lib/reporting');
const e = exposes.presets;


module.exports = [
    {
        zigbeeModel: ['AirQualitySensor-1'],
        model: 'AirQualitySensor-1',
        vendor: 'Finomnis',
        description: 'Room CO2, humidity & temperature sensor',
        fromZigbee: [fz.humidity, fz.temperature, fz.co2],
        exposes: [e.humidity(), e.temperature(), e.co2()],
        toZigbee: [],
        configure: async (device, coordinatorEndpoint, logger) => {
            const endpoint = device.getEndpoint(1);
            const clusters = ['msTemperatureMeasurement', 'msRelativeHumidity', 'msCO2'];
            await reporting.bind(endpoint, coordinatorEndpoint, clusters);
            for (const cluster of clusters) {
                await endpoint.configureReporting(cluster, [
                    {attribute: 'measuredValue', minimumReportInterval: 0, maximumReportInterval: 10, reportableChange: 0},
                ]);
            }
        },
    },
];

