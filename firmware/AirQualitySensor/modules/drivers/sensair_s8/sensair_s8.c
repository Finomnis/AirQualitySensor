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

#define INPUT_REGISTER_SENSOR_ID 29
#define INPUT_REGISTER_SENSOR_ID_LEN 2

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

    uint16_t sensor_id_raw[INPUT_REGISTER_SENSOR_ID_LEN];
    ret = modbus_read_input_regs(
        drv_data->modbus_iface,
        drv_data->modbus_sensor_address,
        INPUT_REGISTER_SENSOR_ID,
        sensor_id_raw,
        INPUT_REGISTER_SENSOR_ID_LEN);
    if (ret)
    {
        LOG_ERR("Unable to read sensor id!");
        return ret;
    }

    LOG_INF("Sensor found! (ID: 0x%04x%04x)", sensor_id_raw[0], sensor_id_raw[1]);

    //     drv_data->i2c = device_get_binding(DT_INST_BUS_LABEL(0));
    //     if (drv_data->i2c == NULL)
    //     {
    //         LOG_ERR("Failed to get pointer to %s device!",
    //                 DT_INST_BUS_LABEL(0));
    //         return -EINVAL;
    //     }

    // #if DT_INST_NODE_HAS_PROP(0, wake_gpios)
    //     drv_data->wake_gpio = device_get_binding(DT_INST_GPIO_LABEL(0, wake_gpios));
    //     if (drv_data->wake_gpio == NULL)
    //     {
    //         LOG_ERR("Failed to get pointer to WAKE device: %s",
    //                 DT_INST_GPIO_LABEL(0, wake_gpios));
    //         return -EINVAL;
    //     }

    //     /*
    // 	 * Wakeup pin should be pulled low before initiating
    // 	 * any I2C transfer.  If it has been tied to GND by
    // 	 * default, skip this part.
    // 	 */
    //     gpio_pin_configure(drv_data->wake_gpio, WAKE_PIN,
    //                        GPIO_OUTPUT_INACTIVE | DT_INST_GPIO_FLAGS(0, wake_gpios));

    //     set_wake(drv_data, true);
    //     k_msleep(1);
    // #endif
    // #if DT_INST_NODE_HAS_PROP(0, reset_gpios)
    //     drv_data->reset_gpio = device_get_binding(DT_INST_GPIO_LABEL(0, reset_gpios));
    //     if (drv_data->reset_gpio == NULL)
    //     {
    //         LOG_ERR("Failed to get pointer to RESET device: %s",
    //                 DT_INST_GPIO_LABEL(0, reset_gpios));
    //         return -EINVAL;
    //     }
    //     gpio_pin_configure(drv_data->reset_gpio, RESET_PIN,
    //                        GPIO_OUTPUT_ACTIVE | DT_INST_GPIO_FLAGS(0, reset_gpios));

    //     k_msleep(1);
    // #endif

    // #if DT_INST_NODE_HAS_PROP(0, irq_gpios)
    //     drv_data->irq_gpio = device_get_binding(DT_INST_GPIO_LABEL(0, irq_gpios));
    //     if (drv_data->irq_gpio == NULL)
    //     {
    //         LOG_ERR("Failed to get pointer to INT device: %s",
    //                 DT_INST_GPIO_LABEL(0, irq_gpios));
    //         return -EINVAL;
    //     }
    // #endif

    //     k_msleep(20); /* t_START assuming recent power-on */

    //     /* Reset the device.  This saves having to deal with detecting
    // 	 * and validating any errors or configuration inconsistencies
    // 	 * after a reset that left the device running.
    // 	 */
    // #if DT_INST_NODE_HAS_PROP(0, reset_gpios)
    //     gpio_pin_set(drv_data->reset_gpio, RESET_PIN, 1);
    //     k_busy_wait(15); /* t_RESET */
    //     gpio_pin_set(drv_data->reset_gpio, RESET_PIN, 0);
    // #else
    //     {
    //         static uint8_t const reset_seq[] = {
    //             0xFF,
    //             0x11,
    //             0xE5,
    //             0x72,
    //             0x8A,
    //         };

    //         if (i2c_write(drv_data->i2c, reset_seq, sizeof(reset_seq),
    //                       DT_INST_REG_ADDR(0)) < 0)
    //         {
    //             LOG_ERR("Failed to issue SW reset");
    //             ret = -EIO;
    //             goto out;
    //         }
    //     }
    // #endif
    //     k_msleep(2); /* t_START after reset */

    //     /* Switch device to application mode */
    //     ret = switch_to_app_mode(drv_data->i2c);
    //     if (ret)
    //     {
    //         goto out;
    //     }

    //     /* Check Hardware ID */
    //     if (i2c_reg_read_byte(drv_data->i2c, DT_INST_REG_ADDR(0),
    //                           CCS811_REG_HW_ID, &hw_id) < 0)
    //     {
    //         LOG_ERR("Failed to read Hardware ID register");
    //         ret = -EIO;
    //         goto out;
    //     }

    //     if (hw_id != CCS881_HW_ID)
    //     {
    //         LOG_ERR("Hardware ID mismatch!");
    //         ret = -EINVAL;
    //         goto out;
    //     }

    //     /* Check application firmware version (first byte) */
    //     cmd = CCS811_REG_FW_APP_VERSION;
    //     if (i2c_write_read(drv_data->i2c, DT_INST_REG_ADDR(0),
    //                        &cmd, sizeof(cmd),
    //                        &fw_ver, sizeof(fw_ver)) < 0)
    //     {
    //         LOG_ERR("Failed to read App Firmware Version register");
    //         ret = -EIO;
    //         goto out;
    //     }
    //     fw_ver = sys_be16_to_cpu(fw_ver);
    //     LOG_INF("App FW %04x", fw_ver);
    //     drv_data->app_fw_ver = fw_ver >> 8U;

    //     /* Configure measurement mode */
    //     uint8_t meas_mode = CCS811_MODE_IDLE;
    // #ifdef CONFIG_CCS811_DRIVE_MODE_1
    //     meas_mode = CCS811_MODE_IAQ_1SEC;
    // #elif defined(CONFIG_CCS811_DRIVE_MODE_2)
    //     meas_mode = CCS811_MODE_IAQ_10SEC;
    // #elif defined(CONFIG_CCS811_DRIVE_MODE_3)
    //     meas_mode = CCS811_MODE_IAQ_60SEC;
    // #elif defined(CONFIG_CCS811_DRIVE_MODE_4)
    //     meas_mode = CCS811_MODE_IAQ_250MSEC;
    // #endif
    //     if (i2c_reg_write_byte(drv_data->i2c, DT_INST_REG_ADDR(0),
    //                            CCS811_REG_MEAS_MODE,
    //                            meas_mode) < 0)
    //     {
    //         LOG_ERR("Failed to set Measurement mode");
    //         ret = -EIO;
    //         goto out;
    //     }
    //     drv_data->mode = meas_mode;

    //     /* Check for error */
    //     status = fetch_status(drv_data->i2c);
    //     if (status < 0)
    //     {
    //         ret = -EIO;
    //         goto out;
    //     }

    //     if (status & CCS811_STATUS_ERROR)
    //     {
    //         LOG_ERR("CCS811 Error %02x during sensor configuration",
    //                 error_from_status(status));
    //         ret = -EINVAL;
    //         goto out;
    //     }

    // #ifdef CONFIG_CCS811_TRIGGER
    //     ret = ccs811_init_interrupt(dev);
    //     LOG_DBG("CCS811 interrupt init got %d", ret);
    // #endif

    // out:
    //     set_wake(drv_data, false);
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
