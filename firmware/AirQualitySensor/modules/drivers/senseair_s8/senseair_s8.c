#define DT_DRV_COMPAT senseair_s8

#include <device.h>
#include <zephyr.h>
#include <drivers/sensor.h>
#include <modbus/modbus.h>

#include <logging/log.h>

#include "senseair_s8.h"

LOG_MODULE_REGISTER(senseair_s8_driver, CONFIG_SENSOR_LOG_LEVEL);

const static struct modbus_iface_param modbus_params = {
    .mode = MODBUS_MODE_RTU,
    .rx_timeout = 50000,
    .serial = {
        .baud = 9600,
        .parity = UART_CFG_PARITY_NONE,
    },
};

#define SENSOR_TYPE_SENSEAIR_S8_LP 0x10e
#define INPUT_REGISTER_DeviceStatus 0
#define INPUT_REGISTER_DeviceStatus_LEN 4
#define INPUT_REGISTER_DeviceStatus_Error 0
#define INPUT_REGISTER_DeviceStatus_CO2 3
#define INPUT_REGISTER_SensorInfo 25
#define INPUT_REGISTER_SensorInfo_LEN 6
#define INPUT_REGISTER_SensorInfo_SensorTypeID_hi 0
#define INPUT_REGISTER_SensorInfo_SensorTypeID_lo 1
#define INPUT_REGISTER_SensorInfo_MemoryMapVersion 2
#define INPUT_REGISTER_SensorInfo_FirmwareVersion 3
#define INPUT_REGISTER_SensorInfo_SensorID_hi 4
#define INPUT_REGISTER_SensorInfo_SensorID_lo 5

static int senseair_s8_read_sample(struct senseair_s8_data *drv_data,
                                   uint16_t *error,
                                   uint16_t *co2_value)
{
    uint16_t device_status[INPUT_REGISTER_DeviceStatus_LEN];

    int ret = modbus_read_input_regs(
        drv_data->modbus_iface,
        drv_data->modbus_sensor_address,
        INPUT_REGISTER_DeviceStatus,
        device_status,
        INPUT_REGISTER_DeviceStatus_LEN);

    if (!ret)
    {
        *error = device_status[INPUT_REGISTER_DeviceStatus_Error];
        *co2_value = device_status[INPUT_REGISTER_DeviceStatus_CO2];
    }

    return ret;
}

static int senseair_s8_init(const struct device *dev)
{
    struct senseair_s8_data *drv_data = dev->data;
    int ret = 0;

    *drv_data = (struct senseair_s8_data){
        .modbus_iface = modbus_iface_get_by_name(CONFIG_SENSEAIR_S8_MODBUS_DEV_NAME),
        .modbus_sensor_address = 0xFE,
        .sample = 0,
    };

    // Initialize modbus
    ret = modbus_init_client(drv_data->modbus_iface, modbus_params);
    if (ret)
    {
        LOG_ERR("Unable to initialize modbus!");
        return ret;
    }

    // Query sensor version information, to make sure
    uint16_t sensor_info[INPUT_REGISTER_SensorInfo_LEN];
    ret = modbus_read_input_regs(
        drv_data->modbus_iface,
        drv_data->modbus_sensor_address,
        INPUT_REGISTER_SensorInfo,
        sensor_info,
        INPUT_REGISTER_SensorInfo_LEN);
    if (ret)
    {
        LOG_ERR("Unable to read sensor info!");
        return ret;
    }

    uint32_t sensor_type =
        (((uint32_t)sensor_info[INPUT_REGISTER_SensorInfo_SensorTypeID_hi]) << 2) +
        ((uint32_t)sensor_info[INPUT_REGISTER_SensorInfo_SensorTypeID_lo]);

    LOG_INF("Sensor connection established!");
    LOG_INF("    Type ID: 0x%x", sensor_type);
    LOG_INF("    Memory Map Version: 0x%x",
            sensor_info[INPUT_REGISTER_SensorInfo_MemoryMapVersion]);
    LOG_INF("    Firmware Version: %d.%d",
            (sensor_info[INPUT_REGISTER_SensorInfo_FirmwareVersion] >> 1) & 0xff,
            sensor_info[INPUT_REGISTER_SensorInfo_FirmwareVersion] & 0xff);
    LOG_INF("    ID: 0x%04x%04x",
            sensor_info[INPUT_REGISTER_SensorInfo_SensorID_hi],
            sensor_info[INPUT_REGISTER_SensorInfo_SensorID_lo]);

    // Check for correct sensor type.
    // Extend this, if other sensors shall be supported.
    // (If they have the same register layout, of course)
    if (sensor_type != SENSOR_TYPE_SENSEAIR_S8_LP)
    {
        LOG_ERR("Sensor is not a Senseair S8 LP!");
        return -EINVAL;
    }

    return ret;
}

static int senseair_s8_sample_fetch(const struct device *dev,
                                    enum sensor_channel chan)
{
    struct senseair_s8_data *drv_data = dev->data;

    uint16_t error;
    uint16_t co2_value;
    int ret = senseair_s8_read_sample(drv_data, &error, &co2_value);
    if (ret)
    {
        return -EIO;
    }

    if (error)
    {
        LOG_ERR("Sensor is in an error state: 0x%04x", error);
        return -EIO;
    }

    drv_data->sample = co2_value;

    return 0;
}

static int senseair_s8_channel_get(const struct device *dev,
                                   enum sensor_channel chan,
                                   struct sensor_value *val)
{
    struct senseair_s8_data *drv_data = dev->data;

    switch (chan)
    {
    case SENSOR_CHAN_CO2:
        val->val1 = 0;
        val->val2 = drv_data->sample;
        break;
    default:
        return -ENOTSUP;
    }

    return 0;
}

static const struct sensor_driver_api senseair_s8_driver_api = {
    .sample_fetch = senseair_s8_sample_fetch,
    .channel_get = senseair_s8_channel_get,
};

static struct senseair_s8_data senseair_s8_data;

DEVICE_DT_INST_DEFINE(0, senseair_s8_init, NULL,
                      &senseair_s8_data, NULL,
                      POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY,
                      &senseair_s8_driver_api);
