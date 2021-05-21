#pragma once

#include <Arduino.h>

class SensorS8LP_ModbusRPC
{
public:
    static constexpr size_t MESSAGE_MAX_SIZE = 39;

public:
    SensorS8LP_ModbusRPC(HardwareSerial *sender, HardwareSerial *receiver,
                         uint8_t function_code, uint8_t *request_data, size_t request_size,
                         size_t response_size);
    void update();

private:
    bool sendModbusRequest(uint8_t function_code, const uint8_t *data, size_t data_len);

private: // Constant values determined at constructor time
    const uint8_t _function_code;
    const size_t _response_size;
    HardwareSerial *const _sender;
    HardwareSerial *const _receiver;

private: // Values that reflect the result state
    bool _finished{false};
    bool _success{false};
    uint8_t _error_code{0x0};

private: // Internal state required to receive non-blocking
    uint8_t _response[MESSAGE_MAX_SIZE];
    size_t _response_size{0};
};
