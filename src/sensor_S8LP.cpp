
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
    // Initialize homie
    homieNode.advertise("co2").setName("CO2").setDatatype("float").setUnit("ppm");

    Serial.println("SenseAir S8 initialization finished.");
}

StatusLEDs::Status SensorS8LP::update()
{
    if (millis() > nextUpdate)
    {
        nextUpdate = millis() + delayMS;

        /*if (!sendModbusRequest('\x04', (const uint8_t *)"\x00\x00\x00\x04", 4))
        {
            ledStatus = StatusLEDs::ERROR;
            return ledStatus;
        }

        uint8_t response[9];
        if (!receiveModbusResponse('\x04', response, 9))
        {
            ledStatus = StatusLEDs::ERROR;
            return ledStatus;
        }

        Serial.print("Response:");
        for (size_t i = 0; i < 9; i++)
        {
            Serial.print(" ");
            Serial.print(response[i], HEX);
        }
        Serial.println();*/

        uint16_t sensor_value;
        if (readIRegisters(0x3, 1, &sensor_value))
        {
            Serial.print("CO2: ");
            Serial.print(sensor_value, DEC);
            Serial.println(" ppm");
            ledStatus = StatusLEDs::EXCELLENT;
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

    Serial.print("Sending ");
    for (size_t i = 0; i < message_len; i++)
    {
        Serial.print(message[i], HEX);
        Serial.print(" ");
    }
    Serial.println("...");

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
    uint8_t message[MESSAGE_MAX_SIZE];
    size_t message_len = data_len + 4;

    if (message_len > MESSAGE_MAX_SIZE)
    {
        Serial.println("ERROR: receiveModbusResponse data was too large!");
        return false;
    }

    size_t bytes_read = Serial.readBytes(message, message_len);
    if (bytes_read != message_len)
    {
        Serial.println("SensorS8LP: Error: Serial.readBytes failed!");
        return false;
    }

    if (message[0] != '\xfe')
    {
        Serial.println("SensorS8LP: Error: Invalid message address!");
        return false;
    }

    if (message[1] != function_code)
    {
        Serial.print("SensorS8LP: Error: Function code didn't match: ");
        Serial.print(message[1], HEX);
        Serial.print(" != ");
        Serial.println(function_code, HEX);
        return false;
    }

    uint16_t crc = initCRC();
    for (size_t i = 0; i < data_len + 2; i++)
    {
        updateCRC(crc, message[i]);
    }

    uint16_t actual_crc = (uint16_t(message[data_len + 3]) << 8) + uint16_t(message[data_len + 2]);

    if (crc != actual_crc)
    {
        Serial.println("SensorS8LP: Error: CRC didn't match!");
        return false;
    }

    memcpy(data, &message[2], data_len);
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
    request_data[0] = start_addr & 0xff;
    request_data[1] = (start_addr >> 8) & 0xff;
    request_data[2] = num_registers & 0xff;
    request_data[3] = (num_registers >> 8) & 0xff;

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
        output[i] = uint16_t(response_data[i + 1] << 8) + response_data[i + 2];
    }

    return true;
}
