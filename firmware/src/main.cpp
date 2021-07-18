#include <Arduino.h>

#include "pinout.hpp"
#include "display/Display.hpp"

#include "utils/TextHelper/U8g2.hpp"

#include "homie/HomieDevice.hpp"

void setup()
{
    PINS::setup();

    Display.begin();

    HomieDevice.init();
}

void loop()
{
    /*Display.clearBuffer();

    Display.setFont(u8g2_font_ncenB14_tr);
    TextHelper::drawText(Display, "Hello World!",
                         Display.getWidth() / 2,
                         Display.getHeight() / 2,
                         TextHelper::H_CENTER,
                         TextHelper::V_CENTER);

    Display.sendBuffer();
    delay(1000);*/

    HomieDevice.update();
}
