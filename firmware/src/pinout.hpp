#pragma once

#include <Arduino.h>

namespace PINS
{
#ifdef ESP32
    constexpr uint8_t AM2302 = 32;
    constexpr HardwareSerial *S8LP_WRITE = &Serial2;
    constexpr HardwareSerial *S8LP_READ = &Serial2;
    constexpr uint8_t DISPLAY_CS = 5;
    constexpr uint8_t DISPLAY_DC = 22;
    constexpr uint8_t DISPLAY_RESET = 21;
#endif

    inline void setup()
    {
#ifdef ESP32
        Serial.begin(9600);
        Serial.println();
        Serial.println();

        Serial2.begin(9600);
#endif
    }
}
