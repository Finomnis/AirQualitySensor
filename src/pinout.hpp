#pragma once

#include "Homie.hpp"

namespace PINS
{
#ifdef ESP8266
    constexpr uint8_t AM2302 = 12;
    constexpr uint8_t LED_HOMIE = 0;
    constexpr HardwareSerial *S8LP_WRITE = &Serial1;
    constexpr HardwareSerial *S8LP_READ = &Serial;
#endif
#ifdef ESP32
    constexpr uint8_t AM2302 = 32;
    constexpr uint8_t LED_HOMIE = 0;
    constexpr HardwareSerial *S8LP_WRITE = &Serial2;
    constexpr HardwareSerial *S8LP_READ = &Serial2;
#endif

    inline void setup()
    {
        Serial.begin(9600);

#ifdef ESP8266
        // Swap the default serial to alternate pins
        // TX: GPIO15
        // RX: GPIO13
        // Further, connect GPIO15 to GPIO01 (TXD0) with a 10k resistor
        // to forward logging info back to UART.
        // Then, connect S8LP to GPIO13 and GPIO2
        Serial.swap();

        Serial1.begin(9600);
#endif
#ifdef ESP32
        Serial2.begin(9600);
#endif

        Serial << endl
               << endl;
    }
}
