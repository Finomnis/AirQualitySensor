#include <Arduino.h>

#include "pinout.hpp"

#include "display/Display.hpp"
#include "homie/HomieDevice.hpp"
#include "sensors/DHT22/DHT22.hpp"
#include "sensors/S8LP/S8LP.hpp"

void setup()
{
    PINS::setup();

    SensorDHT22.init();
    SensorS8LP.init();

    Display.begin();

    HomieDevice.init();
}

void loop()
{
    /*
    #include "utils/TextHelper/U8g2.hpp"

    Display.clearBuffer();

    Display.setFont(u8g2_font_ncenB14_tr);
    TextHelper::drawText(Display, "Hello World!",
                         Display.getWidth() / 2,
                         Display.getHeight() / 2,
                         TextHelper::H_CENTER,
                         TextHelper::V_CENTER);

    Display.sendBuffer();
    delay(1000);
    */

    SensorDHT22.update();
    SensorS8LP.update();

    HomieDevice.update();
}
