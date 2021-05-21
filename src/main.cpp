#include <Homie.hpp>

#include "sensor_AM2302.hpp"
#include "sensor_S8LP.hpp"
#include "status_leds.hpp"
#include "connection_state.hpp"
#include "pinout.hpp"
#include "display.hpp"

SensorAM2302 am2302{PINS::AM2302, "am2302", "AM2302"};
SensorS8LP s8lp{PINS::S8LP_WRITE, PINS::S8LP_READ, "s8lp", "SenseAir S8 LP"};
Display display{};
StatusLEDs leds{3};
ConnectionState connection_state{};

void setup()
{
    // Initialize pins
    PINS::setup();

    // Initialize Homie
    Homie_setFirmware("finomnis-air-sensor", "0.0.1");
    Homie.setLedPin(PINS::LED_HOMIE, HIGH);

    // Setup Temperature/Humidity sensor
    am2302.setup();

    // Setup CO2 sensor
    s8lp.setup();

    // Setup LEDs
    leds.setup();

    // Setup Display
    display.setup();

    // Setup connection state
    connection_state.setup();

    // Setup Homie
    Homie.setup();
}

void loop()
{
    // Update Homie
    Homie.loop();

    // Update sensors
    StatusLEDs::Status am2302_status = am2302.update();
    StatusLEDs::Status s8lp_status = s8lp.update();

    // Update LEDs
    leds.startUpdate();
    leds.addStatus(2, s8lp_status);
    leds.addStatus(1, am2302_status);
    leds.addStatus(0, connection_state.update());
    leds.finishUpdate();

    // Update displays
    display.update(am2302.getLastTemperature(),
                   am2302.getLastHumidity(),
                   s8lp.getLastCO2(),
                   am2302_status,
                   s8lp_status);
}
