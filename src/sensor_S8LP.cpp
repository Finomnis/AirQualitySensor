
#include "sensor_S8LP.hpp"

#include <Homie.h>

constexpr size_t MESSAGE_MAX_SIZE = 39;

SensorS8LP::SensorS8LP(HardwareSerial *sender, HardwareSerial *receiver, const char *id, const char *name)
    : homieNode{id, name, "airsensor"},
      sender{sender},
      receiver{receiver},
      delayMS{4000},
      nextUpdate{0},
      ledStatus{StatusLEDs::ERROR}
{
}

void SensorS8LP::setup()
{
    // Initialize device.
    Serial.println(F("Initializing SenseAir S8 ..."));

    Serial.println("  Input Registers:");
    for (uint16_t i = 0; i < 32; i++)
    {
        uint16_t value;
        bool success = readIRegisters(i, 1, &value);
        if (success)
        {
            Serial.print("    IR");
            Serial.print(i + 1, DEC);
            Serial.print(": 0x");
            Serial.println(value, HEX);
        }
    }

    Serial.println("   Holding Registers:");
    for (uint16_t i = 0; i < 32; i++)
    {
        uint16_t value;
        bool success = readHRegisters(i, 1, &value);
        if (success)
        {
            Serial.print("    HR");
            Serial.print(i + 1, DEC);
            Serial.print(": 0x");
            Serial.println(value, HEX);
        }
    }

    // Initialize homie
    homieNode.advertise("co2").setName("CO2").setDatatype("float").setUnit("ppm");

    Serial.println("SenseAir S8 initialization finished.");
}

StatusLEDs::Status SensorS8LP::update()
{
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
            if (data[0] != 0 || data[1] != 0 || data[2] != 0)
            {
                ledStatus = StatusLEDs::ERROR;
                Serial.print("Sensair S8 is in an error state: 0x");
                Serial.print(data[0]);
                Serial.print(" 0x");
                Serial.print(data[1]);
                Serial.print(" 0x");
                Serial.println(data[2]);
            }
            else
            {
                // Serial.print("CO2: ");
                // Serial.print(data[3], DEC);
                // Serial.println(" ppm");
                homieNode.setProperty("co2").send(String(float(data[3])));
                ledStatus = StatusLEDs::EXCELLENT;
            }
        }
    }

    return ledStatus;
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

bool SensorS8LP::sendModbusRequest(uint8_t function_code, const uint8 *data, size_t data_len)
{
    uint8_t message[MESSAGE_MAX_SIZE];
    size_t message_len = data_len + 4;

    if (message_len > MESSAGE_MAX_SIZE)
    {
        Serial.println("ERROR: sendModbusRequest data was too large!");
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
    Serial.print("Sending ");
    for (size_t i = 0; i < message_len; i++)
    {
        Serial.print(message[i], HEX);
        Serial.print(" ");
    }
    Serial.println("...");
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
        Serial.println("SensorS8LP: Error: Serial.write failed!");
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
        Serial.println("ERROR: receiveModbusResponse data was too large!");
        return false;
    }

    uint8_t address;
    if (1 != Serial.readBytes(&address, 1))
    {
        Serial.println("SensorS8LP: Error: Sensor did not respond!");
        return false;
    }
    if (address != uint8_t('\xfe'))
    {
        Serial.print("SensorS8LP: Error: Invalid message address: ");
        Serial.println(address, HEX);
        return false;
    }

    uint8_t actual_function_code;
    if (1 != Serial.readBytes(&actual_function_code, 1))
    {
        Serial.println("SensorS8LP: Error: Unable to read function code!");
        return false;
    }
    if (actual_function_code != function_code)
    {
        if (actual_function_code == (function_code | uint8_t(0x80)))
        {
            uint8_t dummy[3];
            // read three more to leave the bus in a clean state
            Serial.readBytes(dummy, 3);
            return false;
        }

        Serial.print("SensorS8LP: Error: Function code didn't match: ");
        Serial.print(actual_function_code, HEX);
        Serial.print(" != ");
        Serial.println(function_code, HEX);
        return false;
    }

    size_t bytes_read = Serial.readBytes(payload, payload_len);

    if (bytes_read != payload_len)
    {
        Serial.println("SensorS8LP: Error: Serial.readBytes failed!");
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
        Serial.print("SensorS8LP: Error: CRC didn't match: ");
        Serial.print(actual_crc, HEX);
        Serial.print(" != ");
        Serial.println(crc, HEX);
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
        Serial.println("ERROR: cannot read that many registers at once!");
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
