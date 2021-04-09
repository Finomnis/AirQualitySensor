#include <Homie.h>

#include "sensor_AM2302.hpp"

void loopHandler()
{
    SensorAM2302::update();
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

    // HOMIE Setup
    Homie.setup();
}

void loop()
{
    Homie.loop();
}
