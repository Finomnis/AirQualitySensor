#define DT_DRV_COMPAT sensair_s8

#include <device.h>
#include <zephyr.h>
#include <drivers/sensor.h>
#include <modbus/modbus.h>

#include <logging/log.h>

#include "sensair_s8.h"

LOG_MODULE_REGISTER(sensair_s8, CONFIG_SENSOR_LOG_LEVEL);

const static struct modbus_iface_param modbus_params = {
    .mode = MODBUS_MODE_RTU,
    .rx_timeout = 50000,
    .serial = {
        .baud = 9600,
        .parity = UART_CFG_PARITY_NONE,
    },
};

#define SENSOR_TYPE_SENSAIR_S8_LP 0x10e
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

static int sensair_s8_read_sample(struct sensair_s8_data *drv_data,
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

static int sensair_s8_init(const struct device *dev)
{
    struct sensair_s8_data *drv_data = dev->data;
    int ret = 0;

    *drv_data = (struct sensair_s8_data){
        .modbus_iface = modbus_iface_get_by_name(CONFIG_SENSAIR_S8_MODBUS_DEV_NAME),
        .modbus_sensor_address = 0xFE,
        .sample = {0},
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
    if (sensor_type != SENSOR_TYPE_SENSAIR_S8_LP)
    {
        LOG_ERR("Sensor is not a Sensair S8 LP!");
        return -EINVAL;
    }

    return ret;
}

static int sensair_s8_sample_fetch(const struct device *dev,
                                   enum sensor_channel chan)
{
    struct sensair_s8_data *drv_data = dev->data;
    struct sensair_s8_sample_type *sample = &drv_data->sample;
    // const uint8_t cmd = CCS811_REG_ALG_RESULT_DATA;
    // int rc;
    // uint16_t buf[4] = {0};
    // unsigned int status;

    // set_wake(drv_data, true);
    // rc = i2c_write_read(drv_data->i2c, DT_INST_REG_ADDR(0),
    //                     &cmd, sizeof(cmd),
    //                     (uint8_t *)buf, sizeof(buf));
    // set_wake(drv_data, false);
    // if (rc < 0)
    // {
    //     return -EIO;
    // }

    // rp->co2 = sys_be16_to_cpu(buf[0]);
    // rp->voc = sys_be16_to_cpu(buf[1]);
    // status = sys_le16_to_cpu(buf[2]); /* sic */
    // rp->status = status;
    // rp->error = error_from_status(status);
    // rp->raw = sys_be16_to_cpu(buf[3]);

    // /* APP FW 1.1 does not set DATA_READY, but it does set CO2 to
    //  * zero while it's starting up.  Assume a non-zero CO2 with
    //  * old firmware is valid for the purposes of claiming the
    //  * fetch was fresh.
    //  */
    // if ((drv_data->app_fw_ver <= 0x11) && (rp->co2 != 0))
    // {
    //     status |= CCS811_STATUS_DATA_READY;
    // }
    //return (status & CCS811_STATUS_DATA_READY) ? 0 : -EAGAIN;
    return 0;
}

static int sensair_s8_channel_get(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val)
{
    struct sensair_s8_data *drv_data = dev->data;
    const struct sensair_s8_sample_type *sample = &drv_data->sample;
    uint32_t uval;

    switch (chan)
    {
    case SENSOR_CHAN_CO2:
        val->val1 = 123;
        val->val2 = 45;

        break;
    default:
        return -ENOTSUP;
    }

    return 0;
}

static const struct sensor_driver_api sensair_s8_driver_api = {
    .sample_fetch = sensair_s8_sample_fetch,
    .channel_get = sensair_s8_channel_get,
};

static struct sensair_s8_data sensair_s8_data;

DEVICE_DT_INST_DEFINE(0, sensair_s8_init, NULL,
                      &sensair_s8_data, NULL,
                      POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY,
                      &sensair_s8_driver_api);
