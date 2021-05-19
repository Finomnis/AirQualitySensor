#include "status_leds.hpp"

#include "pinout.hpp"

namespace StatusLEDs
{
    namespace
    {
        struct LedPinState
        {
            inline LedPinState(bool red_, bool yellow_, bool green_) : red{red_}, yellow{yellow_}, green{green_} {}
            bool red;
            bool yellow;
            bool green;
        };

        Status currentStatus = Status::OK;

        LedPinState previousPinState{false, false, false};
    }

    void setup()
    {
        // pinMode(PINS::LED_RED, OUTPUT);
        // pinMode(PINS::LED_YELLOW, OUTPUT);
        // pinMode(PINS::LED_GREEN, OUTPUT);
        // digitalWrite(PINS::LED_RED, LOW);
        // digitalWrite(PINS::LED_YELLOW, LOW);
        // digitalWrite(PINS::LED_GREEN, LOW);
        previousPinState = {false, false, false};
    }

    void startUpdate()
    {
        currentStatus = Status::EXCELLENT;
    }

    void addStatus(Status status)
    {
        if (currentStatus < status)
        {
            currentStatus = status;
        }
    }

    void finishUpdate()
    {
        LedPinState pinState = {false, false, false};
        bool blinkState = (millis() / 600) % 2 == 0;

        switch (currentStatus)
        {
        case Status::EXCELLENT:
            pinState.green = blinkState;
            break;
        case Status::OK:
            pinState.green = true;
            break;
        case Status::WARNING_WEAK:
            pinState.yellow = true;
            break;
        case Status::WARNING_STRONG:
            pinState.red = true;
            break;
        case Status::ERROR:
            pinState.red = blinkState;
            break;
        }

        if (previousPinState.red != pinState.red ||
            previousPinState.yellow != pinState.yellow ||
            previousPinState.green != pinState.green)
        {
            // digitalWrite(PINS::LED_RED, pinState.red);
            // digitalWrite(PINS::LED_YELLOW, pinState.yellow);
            // digitalWrite(PINS::LED_GREEN, pinState.green);
            previousPinState = pinState;
        }
    }
}
