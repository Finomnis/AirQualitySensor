#include <Homie.h>

#include "sensor_AM2302.hpp"
#include "status_leds.hpp"
#include "pinout.hpp"

SensorAM2302 am2302{PINS::AM2302, "am2302", "AM2302"};
SensorAM2302 am2302_2{PINS::AM2302_2, "am2302_2", "AM2302 #2"};

void loopHandler()
{
    StatusLEDs::startUpdate();
    StatusLEDs::addStatus(am2302.update());
    StatusLEDs::addStatus(am2302_2.update());
    StatusLEDs::finishUpdate();
}

void setup()
{
    Serial.begin(115200);
    Serial << endl
           << endl;
    Homie_setFirmware("finomnis-air-sensor", "0.0.1");
    Homie.setLoopFunction(loopHandler);

    // Setup Temperature sensor
    am2302.setup();
    am2302_2.setup();
    StatusLEDs::setup();

    // HOMIE Setup
    Homie.setup();
}

void loop()
{
    Homie.loop();
}
