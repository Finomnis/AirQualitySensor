#pragma once

#include "../../utils/NoMoveNoCopy.hpp"
#include "../../utils/ValueWatcher.hpp"

#include <Arduino.h>

class SensorS8LP_t : NoMoveNoCopy
{
private:
    static constexpr size_t MESSAGE_MAX_SIZE = 39;
    static constexpr uint32_t RPC_TIMEOUT = 100;
    static constexpr uint32_t UPDATE_PERIOD = 5000;

public:
    SensorS8LP_t();
    void init();
    void update();

    Value<uint16_t> const &get_co2_value() { return co2_value; };

private:
    bool sendModbusRequest(uint8_t function_code, const uint8_t *data, size_t data_len);
    bool receiveModbusResponse(uint8_t function_code, uint8_t *data, size_t data_len);
    bool readHRegisters(uint16_t start_addr, uint16_t num_registers, uint16_t *output);
    bool readIRegisters(uint16_t start_addr, uint16_t num_registers, uint16_t *output);
    bool readRegisters(uint8_t function_code, uint16_t start_addr, uint16_t num_registers, uint16_t *output);

private:
    HardwareSerial *sender;
    HardwareSerial *receiver;

    Value<uint16_t> co2_value{0, false};

    uint32_t next_update{0};
};

extern SensorS8LP_t SensorS8LP;
