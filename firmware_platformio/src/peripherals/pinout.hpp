#pragma once

#include <Arduino.h>

#include <U8g2lib.h>

class PINS
{
private:
    PINS() = delete;
    ~PINS() = delete;

public:
    static const uint8_t AM2302;
    static HardwareSerial *const S8LP_WRITE;
    static HardwareSerial *const S8LP_READ;
    static const uint8_t DISPLAY_CS;
    static const uint8_t DISPLAY_DC;
    static const uint8_t DISPLAY_RESET;
    static const uint8_t LED_GREEN;
    static const uint8_t LED_YELLOW;
    static const uint8_t LED_RED;
};

namespace Peripherals
{
    void setup();
    extern U8G2 &display;
}
