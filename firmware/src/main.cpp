#include <Arduino.h>

#include "pinout.hpp"
#include "display/Display.hpp"

void setup()
{
    PINS::setup();

    Display.begin();
}

void loop()
{
    Display.clearBuffer();
    Display.setFont(u8g2_font_ncenB14_tr);
    Display.drawStr(0, 20, "Hello World!");
    Display.sendBuffer();

    delay(1000);
}
