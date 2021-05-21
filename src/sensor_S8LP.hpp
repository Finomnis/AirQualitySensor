#pragma once

#include <Homie.hpp>

#include "status_leds.hpp"

#include <atomic>

class SensorS8LP_ModbusRPC;

class SensorS8LP
{
public:
    SensorS8LP(HardwareSerial *sender, HardwareSerial *receiver, const char *id, const char *name);
    void setup();
    StatusLEDs::Status update();
    uint16_t getLastCO2();

private:
    bool readHRegisters(uint16_t start_addr, uint16_t num_registers, uint16_t *output);
    bool readIRegisters(uint16_t start_addr, uint16_t num_registers, uint16_t *output);
    bool readRegisters(uint8_t function_code, uint16_t start_addr, uint16_t num_registers, uint16_t *output);

private:
    HomieNode homieNode;
    HardwareSerial *sender;
    HardwareSerial *receiver;

    unsigned long delayMS;
    unsigned long nextUpdate;

    uint16_t lastCO2{0};

    std::atomic<StatusLEDs::Status> ledStatus;

    SensorS8LP_ModbusRPC *modbusRPC{nullptr};
};
