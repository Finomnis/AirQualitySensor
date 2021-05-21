
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

    bool validate_crc(uint8_t *data, size_t len)
    {
        uint16_t crc = initCRC();

        uint16_t actual_crc = (uint16_t(data[len - 1]) << 8) + uint16_t(data[len - 2]);

        for (size_t i = 0; i < len - 2; i++)
        {
            updateCRC(crc, data[i]);
        }

        if (crc != actual_crc)
        {
            Homie.getLogger().print(F("SensorS8LP: Error: CRC didn't match: "));
            Homie.getLogger().print(actual_crc, HEX);
            Homie.getLogger().print(F(" != "));
            Homie.getLogger().println(crc, HEX);
            return false;
        }

        return true;
    }
}

SensorS8LP_ModbusRPC::SensorS8LP_ModbusRPC(HardwareSerial *sender, HardwareSerial *receiver,
                                           uint8_t function_code, uint8_t *request_data, size_t request_size,
                                           size_t response_size)
    : _function_code{function_code},
      _response_size{response_size},
      _response_payload_size{response_size + 2},
      _sender{sender},
      _receiver{receiver}
{
    if (_response_payload_size + 2 > MESSAGE_MAX_SIZE)
    {
        Homie.getLogger().println(F("ERROR: expected response size is too large!"));
        finish_error();
        return;
    }

    if (!sendModbusRequest(function_code, request_data, request_size))
    {
        finish_error();
        return;
    }
}

void SensorS8LP_ModbusRPC::wait_for_finished()
{
    while (!_finished)
    {
        update();
    }
}

void SensorS8LP_ModbusRPC::finish_error(uint8_t error_code)
{
    _finished = true;
    _success = false;
    _error_code = error_code;
}

void SensorS8LP_ModbusRPC::finish_success()
{
    _finished = true;
    _success = true;
}

void SensorS8LP_ModbusRPC::update()
{
    if (_finished)
    {
        return;
    }

    updateReceiveModbusResponse();
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

void SensorS8LP_ModbusRPC::updateReceiveModbusResponse()
{
    bool new_data_received = false;

    while (_receiver->available() > 0)
    {
        if (_response_num_received >= MESSAGE_MAX_SIZE)
        {
            finish_error();
            return;
        }

        int data = _receiver->read();
        if (data < 0)
        {
            finish_error();
            return;
        }

        _response[_response_num_received++] = data;
        new_data_received = true;
    }

    if (!new_data_received)
    {
        return;
    }

    if (_response_num_received < 5)
        return;

    uint8_t address = _response[0];
    if (address != uint8_t('\xfe'))
    {
        Homie.getLogger().print(F("SensorS8LP: Error: Invalid message address: "));
        Homie.getLogger().println(address, HEX);
        finish_error();
        return;
    }

    uint8_t actual_function_code = _response[1];

    // Error
    if (actual_function_code == (_function_code | uint8_t(0x80)))
    {
        if (!validate_crc(_response, 5))
        {
            finish_error();
            return;
        }

        finish_error(_response[2]);
        return;
    }

    if (actual_function_code != _function_code)
    {
        Homie.getLogger().print(F("SensorS8LP: Error: Function code didn't match: "));
        Homie.getLogger().print(actual_function_code, HEX);
        Homie.getLogger().print(F(" != "));
        Homie.getLogger().println(_function_code, HEX);
        finish_error();
        return;
    }

    if (_response_num_received < _response_payload_size + 2)
        return;

    if (!validate_crc(_response, _response_payload_size + 2))
    {
        finish_error();
        return;
    }

    finish_success();
    return;
}

// bool SensorS8LP::receiveModbusResponse(uint8_t function_code, uint8_t *data, size_t data_len)
// {
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
