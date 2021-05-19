#include <Homie.h>

#include "sensor_AM2302.hpp"
#include "sensor_S8LP.hpp"
#include "status_leds.hpp"
#include "pinout.hpp"
#include "display.hpp"

SensorAM2302 am2302{PINS::AM2302, "am2302", "AM2302"};
SensorS8LP s8lp{PINS::S8LP_WRITE, PINS::S8LP_READ, "s8lp", "SenseAir S8 LP"};
Display display{};

void setup()
{
    // Initialize pins
    PINS::setup();

    Homie_setFirmware("finomnis-air-sensor", "0.0.1");
    Homie.setLedPin(PINS::LED_HOMIE, HIGH);

    // Setup Temperature sensor
    am2302.setup();
    s8lp.setup();
    //StatusLEDs::setup();
    display.setup();

    // HOMIE Setup
    Homie.setup();
}

void loop()
{
    Homie.loop();
    StatusLEDs::startUpdate();
    StatusLEDs::Status am2302_status = am2302.update();
    StatusLEDs::Status s8lp_status = s8lp.update();
    //StatusLEDs::finishUpdate();

    display.update(am2302.getLastTemperature(),
                   am2302.getLastHumidity(),
                   s8lp.getLastCO2(),
                   am2302_status,
                   s8lp_status);
}
