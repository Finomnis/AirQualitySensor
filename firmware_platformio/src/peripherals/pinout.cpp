#include "pinout.hpp"

#ifdef ESP32
const uint8_t PINS::AM2302 = 32;
HardwareSerial *const PINS::S8LP_WRITE = &Serial2;
HardwareSerial *const PINS::S8LP_READ = &Serial2;
const uint8_t PINS::DISPLAY_CS = 5;
const uint8_t PINS::DISPLAY_DC = 22;
const uint8_t PINS::DISPLAY_RESET = 21;
const uint8_t PINS::LED_GREEN = 2;
const uint8_t PINS::LED_YELLOW = 15;
const uint8_t PINS::LED_RED = 4;
#endif

namespace Peripherals
{
    void setup()
    {
#ifdef ESP32
        Serial.begin(9600);
        Serial.println();
        Serial.println();

        Serial2.begin(9600);
#endif
    }

#ifdef ESP32
    static U8G2_ST7565_EA_DOGM132_F_4W_HW_SPI DisplayInst(U8G2_R0, PINS::DISPLAY_CS, PINS::DISPLAY_DC, PINS::DISPLAY_RESET);
#endif

    U8G2 &display{DisplayInst};
}
