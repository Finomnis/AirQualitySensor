#pragma once

#include "Homie.hpp"

namespace PINS
{
#ifdef ESP8266
    constexpr uint8_t AM2302 = 4;
    constexpr uint8_t LED_HOMIE = 0;
    constexpr uint8_t LED_RED = 12;
    constexpr uint8_t LED_YELLOW = 14;
    constexpr uint8_t LED_GREEN = 16;
    constexpr HardwareSerial *S8LP_WRITE = &Serial1;
    constexpr HardwareSerial *S8LP_READ = &Serial;
#endif
#ifdef ESP32
    constexpr uint8_t AM2302 = 32;
    constexpr uint8_t LED_HOMIE = 0;
    constexpr uint8_t LED_RED = 12;
    constexpr uint8_t LED_YELLOW = 14;
    constexpr uint8_t LED_GREEN = 16;
    constexpr HardwareSerial *S8LP_WRITE = &Serial2;
    constexpr HardwareSerial *S8LP_READ = &Serial2;
#endif

    inline void setup()
    {
        Serial.begin(9600);

#ifdef ESP8266
        Serial1.begin(9600);
#endif
#ifdef ESP32
        Serial2.begin(9600);
#endif

        Serial << endl
               << endl;
    }
}
