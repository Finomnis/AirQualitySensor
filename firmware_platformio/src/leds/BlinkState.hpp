#pragma once

#include "../utils/ValueWatcher.hpp"
#include "../utils/NoMoveNoCopy.hpp"

class BlinkState : NoMoveNoCopy
{
public:
    BlinkState(uint32_t blink_rate_ms);
    void reset_blink_state(bool target_state = true);
    Value<bool> &get_blink_value();
    void update();

private:
    Value<bool> blink_state_value;
    uint32_t blink_state_start;
    uint32_t blink_rate_ms;
};
