#pragma once

namespace StatusLEDs
{
    enum Status
    {
        OK = 1,
        WARNING_WEAK = 2,
        WARNING_STRONG = 3,
        ERROR = 4,
    };

    void setup();
    void startUpdate();
    void addStatus(Status status);
    void finishUpdate();
}