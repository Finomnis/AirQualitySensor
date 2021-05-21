
#include "sensor_S8LP.hpp"

#include "sensor_S8LP_modbusRPC.hpp"

#include <Homie.hpp>

constexpr size_t MESSAGE_MAX_SIZE = 39;

SensorS8LP::SensorS8LP(HardwareSerial *sender, HardwareSerial *receiver, const char *id, const char *name)
    : homieNode{id, name, "airsensor"},
      sender{sender},
      receiver{receiver},
      delayMS{4000},
      nextUpdate{0},
      calibration_state{CALIBRATION_BOOTUP},
      last_calibration{0},
      ledStatus{StatusLEDs::ERROR}
{
}

void SensorS8LP::setup()
{
    // Initialize device.
    Homie.getLogger().println(F("Initializing SenseAir S8 ..."));

    // Initialize homie
    homieNode.advertise("co2").setName("CO2").setDatatype("float").setUnit("ppm");
    // homieNode.advertise("calibration").setName("CO2 Calibration State").setDatatype("string");
    // homieNode.advertise("calibrate")
    //     .setName("Start CO2 Calibration")
    //     .setDatatype("boolean")
    //     .settable([this](const HomieRange &range, const String &value) {
    //         if (value != "true")
    //         {
    //             Homie.getLogger().println("Invalid value received for 'calibrate'. Only 'true' is valid.");
    //             return false;
    //         }

    //         if (calibration_state != CALIBRATION_IDLE)
    //         {
    //             Homie.getLogger().println("Can't start calibration, not in IDLE state.");
    //             return false;
    //         }

    //         setCalibrationState(CALIBRATION_REQUESTED);
    //         return true;
    //     });

    Homie.getLogger().println("SenseAir S8 initialization finished.");
}

StatusLEDs::Status SensorS8LP::update()
{
    if (updateCalibration())
        return ledStatus;

    if (millis() > nextUpdate)
    {
        nextUpdate = millis() + delayMS;

        uint16_t data[4];
        if (!readIRegisters(0x0, 4, data))
        {
            ledStatus = StatusLEDs::ERROR;
        }
        else
        {
            if (data[0] != 0)
            {
                ledStatus = StatusLEDs::ERROR;
                Homie.getLogger().print("Sensair S8 is in an error state: 0x");
                Homie.getLogger().print(data[0]);
                Homie.getLogger().print(" 0x");
                Homie.getLogger().print(data[1]);
                Homie.getLogger().print(" 0x");
                Homie.getLogger().println(data[2]);
            }
            else
            {
                // Homie.getLogger().print("CO2: ");
                // Homie.getLogger().print(data[3], DEC);
                // Homie.getLogger().println(" ppm");
                float value = data[3];

                if (value > 1450.0f)
                {
                    ledStatus = StatusLEDs::WARNING_STRONG;
                }
                else if (value > 1000.0f)
                {
                    ledStatus = StatusLEDs::WARNING_WEAK;
                }
                else if (value > 600.0f)
                {
                    ledStatus = StatusLEDs::OK;
                }
                else
                {
                    ledStatus = StatusLEDs::EXCELLENT;
                }

                if (Homie.isConnected())
                {
                    homieNode.setProperty("co2").send(String(value));
                }

                lastCO2 = data[3];
            }
        }
    }

    return ledStatus;
}

bool SensorS8LP::updateCalibration()
{
    if (!Homie.isConnected())
    {
        return false;
    }

    switch (calibration_state)
    {
    case CALIBRATION_REQUESTED:
        setCalibrationState(CALIBRATION_RUNNING);
        return true;
    case CALIBRATION_RUNNING:
        if (runBackgroundCalibration())
        {
            setCalibrationState(CALIBRATION_SUCCEEDED);
        }
        else
        {
            setCalibrationState(CALIBRATION_FAILED);
        }
        last_calibration = millis();
        return true;
    case CALIBRATION_IDLE:
        return false;
    case CALIBRATION_SUCCEEDED:
    case CALIBRATION_FAILED:
        if (millis() > last_calibration + 10000)
        {
            setCalibrationState(CALIBRATION_IDLE);
        }
        return false;
    case CALIBRATION_BOOTUP:
        setCalibrationState(CALIBRATION_IDLE);
        return false;
    }

    return false;
}

bool SensorS8LP::runBackgroundCalibration()
{
    Homie.getLogger().println("Starting Sensair S8 Calibration routine ...");

    if (!sendModbusRequest(0x06, (const uint8_t *)"\x00\x00\x00\x00", 4))
    {
        Homie.getLogger().println("Resetting calibration register failed!");
        return false;
    }

    uint8_t response[4];
    if (!receiveModbusResponse(0x06, response, 4))
    {
        Homie.getLogger().println("Resetting calibration register was not responded!");
        return false;
    }
    if (response[0] != 0 || response[1] != 0 || response[2] != 0 || response[3] != 0)
    {
        Homie.getLogger().println("Resetting calibration register was not successful!");
        return false;
    }

    if (!sendModbusRequest(0x06, (const uint8_t *)"\x00\x01\x7c\x06", 4))
    {
        Homie.getLogger().println("Sending calibration request failed!");
        return false;
    }
    if (!receiveModbusResponse(0x06, response, 4))
    {
        Homie.getLogger().println("Sending calibration request was not responded!");
        return false;
    }
    if (response[0] != 0x00 || response[1] != 0x01 || response[2] != 0x7c || response[3] != 0x06)
    {
        Homie.getLogger().println("Sending calibration request was not successful!");
        return false;
    }

    Homie.getLogger().println("Waiting for calibration to finish ...");
    delay(delayMS);

    uint16_t registerContent;
    if (!readHRegisters(0x00, 1, &registerContent))
    {
        Homie.getLogger().println("Unable to read calibration state!");
        return false;
    }

    if (registerContent != 0x0020)
    {
        Homie.getLogger().println("Calibration did not succeed.");
        return false;
    }

    Homie.getLogger().println("Calibration was successful.");
    return true;
}

bool SensorS8LP::readHRegisters(uint16_t start_addr, uint16_t num_registers, uint16_t *output)
{
    return readRegisters(0x03, start_addr, num_registers, output);
}

bool SensorS8LP::readIRegisters(uint16_t start_addr, uint16_t num_registers, uint16_t *output)
{
    return readRegisters(0x04, start_addr, num_registers, output);
}

bool SensorS8LP::readRegisters(uint8_t function_code, uint16_t start_addr, uint16_t num_registers, uint16_t *output)
{

    uint8_t request_data[4];
    request_data[0] = (start_addr >> 8) & 0xff;
    request_data[1] = start_addr & 0xff;
    request_data[2] = (num_registers >> 8) & 0xff;
    request_data[3] = num_registers & 0xff;

    size_t response_size = 2 * num_registers + 1;

    SensorS8LP_ModbusRPC readRegisterRPC(sender, receiver, function_code, request_data, 4, response_size);
    readRegisterRPC.wait_for_finished();
    if (!readRegisterRPC.success())
    {
        return false;
    }

    const uint8_t *response_data = readRegisterRPC.response_payload();

    for (int i = 0; i < num_registers; i++)
    {
        output[i] = uint16_t(response_data[2 * i + 1] << 8) + response_data[2 * i + 2];
    }

    return true;
}

void SensorS8LP::setCalibrationState(SensorS8LP::CalibrationState state)
{
    calibration_state = state;

    bool calibrating = false;
    const char *calibration_description = "Unknown";

    switch (calibration_state)
    {
    case CALIBRATION_IDLE:
        calibrating = false;
        calibration_description = "Idle";
        break;
    case CALIBRATION_REQUESTED:
        calibrating = true;
        calibration_description = "Requested";
        break;
    case CALIBRATION_RUNNING:
        calibrating = true;
        calibration_description = "Running";
        break;
    case CALIBRATION_FAILED:
        calibrating = false;
        calibration_description = "Failed";
        break;
    case CALIBRATION_SUCCEEDED:
        calibrating = false;
        calibration_description = "Succeeded";
        break;
    case CALIBRATION_BOOTUP:
        calibrating = false;
        calibration_description = "Bootup";
        break;
    }

    homieNode.setProperty("calibrate").send(calibrating ? "true" : "false");
    homieNode.setProperty("calibration").send(calibration_description);
}

uint16_t SensorS8LP::getLastCO2()
{
    return lastCO2;
}
