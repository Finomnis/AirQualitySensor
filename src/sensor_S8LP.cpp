
#include "sensor_S8LP.hpp"

#include <Homie.h>

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

    // Uncomment to run a sensor calibration
    //runBackgroundCalibration();

    Homie.getLogger().println("  Input Registers:");
    bool responsive = true;
    for (uint16_t i = 0; i < 32 && responsive; i++)
    {
        uint16_t value;
        bool success = readIRegisters(i, 1, &value);
        if (success)
        {
            Homie.getLogger().print("    IR");
            Homie.getLogger().print(i + 1, DEC);
            Homie.getLogger().print(": 0x");
            Homie.getLogger().println(value, HEX);
        }
        else if (i == 0)
        {
            responsive = false;
            Homie.getLogger().println("Sensor S8LP doesn't seem to be connected.");
        }
    }

    Homie.getLogger().println("   Holding Registers:");
    for (uint16_t i = 0; i < 32 && responsive; i++)
    {
        uint16_t value;
        bool success = readHRegisters(i, 1, &value);
        if (success)
        {
            Homie.getLogger().print("    HR");
            Homie.getLogger().print(i + 1, DEC);
            Homie.getLogger().print(": 0x");
            Homie.getLogger().println(value, HEX);
        }
    }

    // Initialize homie
    homieNode.advertise("co2").setName("CO2").setDatatype("float").setUnit("ppm");
    homieNode.advertise("calibration").setName("CO2 Calibration State").setDatatype("string");
    homieNode.advertise("calibrate")
        .setName("Start CO2 Calibration")
        .setDatatype("boolean")
        .settable([this](const HomieRange &range, const String &value) {
            if (value != "true")
            {
                Homie.getLogger().println("Invalid value received for 'calibrate'. Only 'true' is valid.");
                return false;
            }

            if (calibration_state != CALIBRATION_IDLE)
            {
                Homie.getLogger().println("Can't start calibration, not in IDLE state.");
                return false;
            }

            setCalibrationState(CALIBRATION_REQUESTED);
            return true;
        });

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

namespace
{
    uint16_t initCRC()
    {
        return -1;
    }
    void updateCRC(uint16_t &crc, uint8_t data)
    {
        crc ^= uint16_t(data);

        for (int i = 8; i != 0; i--)
        {
            if ((crc & 0x0001) != 0)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
}

bool SensorS8LP::sendModbusRequest(uint8_t function_code, const uint8_t *data, size_t data_len)
{
    uint8_t message[MESSAGE_MAX_SIZE];
    size_t message_len = data_len + 4;

    if (message_len > MESSAGE_MAX_SIZE)
    {
        Homie.getLogger().println("ERROR: sendModbusRequest data was too large!");
        return false;
    }

    uint16_t crc = initCRC();

    message[0] = '\xfe';
    updateCRC(crc, '\xfe');

    message[1] = function_code;
    updateCRC(crc, function_code);

    for (size_t i = 0; i < data_len; i++)
    {
        uint8_t value = data[i];
        updateCRC(crc, value);
        message[2 + i] = value;
    }

    message[data_len + 2] = uint8_t(crc & 0xff);
    message[data_len + 3] = uint8_t((crc >> 8) & 0xff);

    /*
    Homie.getLogger().print("Sending ");
    for (size_t i = 0; i < message_len; i++)
    {
        Homie.getLogger().print(message[i], HEX);
        Homie.getLogger().print(" ");
    }
    Homie.getLogger().println("...");
    */

    // Clear receiver buffer
    while (receiver->available() > 0)
    {
        receiver->read();
    }
    // Send
    size_t bytes_written = sender->write(message, message_len);

    if (bytes_written != message_len)
    {
        Homie.getLogger().println(F("SensorS8LP: Error: sender->write failed!"));
        return false;
    }

    return true;
}

bool SensorS8LP::receiveModbusResponse(uint8_t function_code, uint8_t *data, size_t data_len)
{
    constexpr size_t PAYLOAD_MAX_SIZE = MESSAGE_MAX_SIZE - 2;
    uint8_t payload[PAYLOAD_MAX_SIZE];
    size_t payload_len = data_len + 2;

    if (payload_len > PAYLOAD_MAX_SIZE)
    {
        Homie.getLogger().println(F("ERROR: receiveModbusResponse data was too large!"));
        return false;
    }

    uint8_t address;
    if (1 != receiver->readBytes(&address, 1))
    {
        Homie.getLogger().println(F("SensorS8LP: Error: Sensor did not respond!"));
        return false;
    }
    if (address != uint8_t('\xfe'))
    {
        Homie.getLogger().print(F("SensorS8LP: Error: Invalid message address: "));
        Homie.getLogger().println(address, HEX);
        return false;
    }

    uint8_t actual_function_code;
    if (1 != receiver->readBytes(&actual_function_code, 1))
    {
        Homie.getLogger().println(F("SensorS8LP: Error: Unable to read function code!"));
        return false;
    }
    if (actual_function_code != function_code)
    {
        if (actual_function_code == (function_code | uint8_t(0x80)))
        {
            uint8_t dummy[3];
            // read three more to leave the bus in a clean state
            receiver->readBytes(dummy, 3);
            return false;
        }

        Homie.getLogger().print(F("SensorS8LP: Error: Function code didn't match: "));
        Homie.getLogger().print(actual_function_code, HEX);
        Homie.getLogger().print(F(" != "));
        Homie.getLogger().println(function_code, HEX);
        return false;
    }

    size_t bytes_read = receiver->readBytes(payload, payload_len);

    if (bytes_read != payload_len)
    {
        Homie.getLogger().println(F("SensorS8LP: Error: receiver->readBytes failed!"));
        return false;
    }

    uint16_t crc = initCRC();
    updateCRC(crc, address);
    updateCRC(crc, actual_function_code);
    for (size_t i = 0; i < payload_len - 2; i++)
    {
        updateCRC(crc, payload[i]);
    }

    uint16_t actual_crc = (uint16_t(payload[payload_len - 1]) << 8) + uint16_t(payload[payload_len - 2]);

    if (crc != actual_crc)
    {
        Homie.getLogger().print(F("SensorS8LP: Error: CRC didn't match: "));
        Homie.getLogger().print(actual_crc, HEX);
        Homie.getLogger().print(F(" != "));
        Homie.getLogger().println(crc, HEX);
        return false;
    }

    memcpy(data, payload, data_len);
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

    constexpr size_t RESPONSE_MAX_SIZE = MESSAGE_MAX_SIZE - 4;
    uint8_t response_data[RESPONSE_MAX_SIZE];
    size_t response_size = 2 * num_registers + 1;

    if (response_size > RESPONSE_MAX_SIZE)
    {
        Homie.getLogger().println(F("ERROR: cannot read that many registers at once!"));
        return false;
    }

    if (!sendModbusRequest(function_code, request_data, 4))
    {
        return false;
    }

    if (!receiveModbusResponse(function_code, response_data, response_size))
    {
        return false;
    }

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
