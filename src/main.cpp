#include <Homie.h>

#include "sensor_AM2302.hpp"
#include "sensor_S8LP.hpp"
#include "status_leds.hpp"
#include "pinout.hpp"

SensorAM2302 am2302{PINS::AM2302, "am2302", "AM2302"};
SensorS8LP s8lp{&Serial1, &Serial, "s8lp", "SenseAir S8 LP"};

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);

    Serial << endl
           << endl;

    Homie_setFirmware("finomnis-air-sensor", "0.0.1");
    Homie.setLedPin(PINS::LED_HOMIE, HIGH);

    // Setup Temperature sensor
    am2302.setup();
    s8lp.setup();
    StatusLEDs::setup();

    // HOMIE Setup
    Homie.setup();
}

void loop()
{
    Homie.loop();
    StatusLEDs::startUpdate();
    StatusLEDs::addStatus(am2302.update());
    StatusLEDs::addStatus(s8lp.update());
    StatusLEDs::finishUpdate();
}
