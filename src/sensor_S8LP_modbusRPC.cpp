
#include "sensor_S8LP_ModbusRPC.hpp"

#include <Homie.hpp>

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

SensorS8LP_ModbusRPC::SensorS8LP_ModbusRPC(HardwareSerial *sender, HardwareSerial *receiver,
                                           uint8_t function_code, uint8_t *request_data, size_t request_size,
                                           size_t response_size)
    : _function_code{function_code},
      _response_size{response_size},
      _sender{sender},
      _receiver{receiver}
{
    if (!sendModbusRequest(function_code, request_data, request_size))
    {
        _finished = true;
        _success = false;
    }
}

void SensorS8LP_ModbusRPC::update()
{
    if (_finished)
    {
        return;
    }
}

bool SensorS8LP_ModbusRPC::sendModbusRequest(uint8_t function_code, const uint8_t *data, size_t data_len)
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

    // Clear receiver buffer
    while (_receiver->available() > 0)
    {
        _receiver->read();
    }

    // Send
    size_t bytes_written = _sender->write(message, message_len);

    if (bytes_written != message_len)
    {
        Homie.getLogger().println(F("SensorS8LP: Error: sender->write failed!"));
        return false;
    }

    return true;
}

// bool SensorS8LP::receiveModbusResponse(uint8_t function_code, uint8_t *data, size_t data_len)
// {
//     constexpr size_t PAYLOAD_MAX_SIZE = MESSAGE_MAX_SIZE - 2;
//     uint8_t payload[PAYLOAD_MAX_SIZE];
//     size_t payload_len = data_len + 2;

//     if (payload_len > PAYLOAD_MAX_SIZE)
//     {
//         Homie.getLogger().println(F("ERROR: receiveModbusResponse data was too large!"));
//         return false;
//     }

//     uint8_t address;
//     if (1 != receiver->readBytes(&address, 1))
//     {
//         Homie.getLogger().println(F("SensorS8LP: Error: Sensor did not respond!"));
//         return false;
//     }
//     if (address != uint8_t('\xfe'))
//     {
//         Homie.getLogger().print(F("SensorS8LP: Error: Invalid message address: "));
//         Homie.getLogger().println(address, HEX);
//         return false;
//     }

//     uint8_t actual_function_code;
//     if (1 != receiver->readBytes(&actual_function_code, 1))
//     {
//         Homie.getLogger().println(F("SensorS8LP: Error: Unable to read function code!"));
//         return false;
//     }
//     if (actual_function_code != function_code)
//     {
//         if (actual_function_code == (function_code | uint8_t(0x80)))
//         {
//             uint8_t dummy[3];
//             // read three more to leave the bus in a clean state
//             receiver->readBytes(dummy, 3);
//             return false;
//         }

//         Homie.getLogger().print(F("SensorS8LP: Error: Function code didn't match: "));
//         Homie.getLogger().print(actual_function_code, HEX);
//         Homie.getLogger().print(F(" != "));
//         Homie.getLogger().println(function_code, HEX);
//         return false;
//     }

//     size_t bytes_read = receiver->readBytes(payload, payload_len);

//     if (bytes_read != payload_len)
//     {
//         Homie.getLogger().println(F("SensorS8LP: Error: receiver->readBytes failed!"));
//         return false;
//     }

//     uint16_t crc = initCRC();
//     updateCRC(crc, address);
//     updateCRC(crc, actual_function_code);
//     for (size_t i = 0; i < payload_len - 2; i++)
//     {
//         updateCRC(crc, payload[i]);
//     }

//     uint16_t actual_crc = (uint16_t(payload[payload_len - 1]) << 8) + uint16_t(payload[payload_len - 2]);

//     if (crc != actual_crc)
//     {
//         Homie.getLogger().print(F("SensorS8LP: Error: CRC didn't match: "));
//         Homie.getLogger().print(actual_crc, HEX);
//         Homie.getLogger().print(F(" != "));
//         Homie.getLogger().println(crc, HEX);
//         return false;
//     }

//     memcpy(data, payload, data_len);
//     return true;
// }
