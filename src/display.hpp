#pragma once

#include "status_leds.hpp"

#include <Adafruit_SSD1306.h>

class Display
{
public:
    Display();
    void setup();
    void update(float temperature, float humidity, uint16_t co2, StatusLEDs::Status temp_humidity_status, StatusLEDs::Status co2_status);

private:
    Adafruit_SSD1306 display;

    float displayedTemperature;
    float displayedHumidity;
    uint16_t displayedCO2;
};
