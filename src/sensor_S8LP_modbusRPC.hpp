#pragma once

#include <Arduino.h>

class SensorS8LP_ModbusRPC
{
public:
    static constexpr size_t MESSAGE_MAX_SIZE = 39;
    static constexpr unsigned long RPC_TIMEOUT = 500;

public:
    SensorS8LP_ModbusRPC(HardwareSerial *sender, HardwareSerial *receiver,
                         uint8_t function_code, uint8_t *request_data, size_t request_size,
                         size_t response_size);
    void update();
    void wait_for_finished();
    bool finished() { return _finished; }
    bool success() { return _success; }
    const uint8_t *response_payload() { return &_response[2]; }

private:
    bool sendModbusRequest(uint8_t function_code, const uint8_t *data, size_t data_len);
    void updateReceiveModbusResponse();
    void finish_error(uint8_t error_code = 0x0);
    void finish_success();

private: // Constant values determined at constructor time
    const uint8_t _function_code;
    const size_t _response_size;
    const size_t _response_payload_size;
    HardwareSerial *const _sender;
    HardwareSerial *const _receiver;
    const unsigned long _timeout;

private: // Values that reflect the result state
    bool _finished{false};
    bool _success{false};
    uint8_t _error_code{0x0};

private: // Internal state required to receive non-blocking
    uint8_t _response[MESSAGE_MAX_SIZE];
    size_t _response_num_received{0};
};
