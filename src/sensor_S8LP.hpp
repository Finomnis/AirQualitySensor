#pragma once

#include <Homie.hpp>

#include "status_leds.hpp"

class SensorS8LP
{
public:
    SensorS8LP(HardwareSerial *sender, HardwareSerial *receiver, const char *id, const char *name);
    void setup();
    StatusLEDs::Status update();

private:
    bool sendModbusRequest(uint8_t function_code, const uint8_t *data, size_t data_len);
    bool receiveModbusResponse(uint8_t function_code, uint8_t *data, size_t data_len);
    bool readHRegisters(uint16_t start_addr, uint16_t num_registers, uint16_t *output);
    bool readIRegisters(uint16_t start_addr, uint16_t num_registers, uint16_t *output);
    bool readRegisters(uint8_t function_code, uint16_t start_addr, uint16_t num_registers, uint16_t *output);

private:
    HomieNode homieNode;
    HardwareSerial *sender;
    HardwareSerial *receiver;

    unsigned long delayMS;
    unsigned long nextUpdate;

    StatusLEDs::Status ledStatus;
};
