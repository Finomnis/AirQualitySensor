#include <Homie.h>

#include "sensor_AM2302.hpp"
#include "status_leds.hpp"

void loopHandler()
{
    StatusLEDs::startUpdate();
    StatusLEDs::addStatus(SensorAM2302::update());
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
    SensorAM2302::setup();
    StatusLEDs::setup();

    // HOMIE Setup
    Homie.setup();
}

void loop()
{
    Homie.loop();
}
