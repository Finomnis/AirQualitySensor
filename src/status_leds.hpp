#pragma once

namespace StatusLEDs
{
    enum Status
    {
        EXCELLENT = 1,
        OK = 2,
        WARNING_WEAK = 3,
        WARNING_STRONG = 4,
        ERROR = 5,
    };

    void setup();
    void startUpdate();
    void addStatus(Status status);
    void finishUpdate();
}