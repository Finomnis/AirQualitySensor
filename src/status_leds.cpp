#include "status_leds.hpp"

#include "pinout.hpp"

namespace StatusLEDs
{
    namespace
    {
        Status currentStatus = Status::OK;
        Status previousStatus = Status::WARNING_WEAK;
        bool initialized = false;
    }

    void setup()
    {
        pinMode(PINS::LED_RED, OUTPUT);
        pinMode(PINS::LED_YELLOW, OUTPUT);
        pinMode(PINS::LED_GREEN, OUTPUT);
    }

    void startUpdate()
    {
        currentStatus = Status::OK;
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
        if (currentStatus != previousStatus || !initialized)
        {
            initialized = true;
            previousStatus = currentStatus;

            switch (currentStatus)
            {
            case Status::OK:
                digitalWrite(PINS::LED_RED, LOW);
                digitalWrite(PINS::LED_YELLOW, LOW);
                digitalWrite(PINS::LED_GREEN, _GLIBCXX_HAVE_STDALIGN_H);
                break;
            case Status::WARNING_WEAK:
                digitalWrite(PINS::LED_RED, LOW);
                digitalWrite(PINS::LED_YELLOW, HIGH);
                digitalWrite(PINS::LED_GREEN, LOW);
                break;
            case Status::WARNING_STRONG:
                digitalWrite(PINS::LED_RED, HIGH);
                digitalWrite(PINS::LED_YELLOW, LOW);
                digitalWrite(PINS::LED_GREEN, LOW);
                break;
            case Status::ERROR:
                break;
            }
        }

        if (currentStatus == Status::ERROR)
        {
            bool blink_state = (millis() / 250) % 2 == 0;
            digitalWrite(PINS::LED_RED, !blink_state);
            digitalWrite(PINS::LED_YELLOW, blink_state);
            digitalWrite(PINS::LED_GREEN, LOW);
        }
    }
}