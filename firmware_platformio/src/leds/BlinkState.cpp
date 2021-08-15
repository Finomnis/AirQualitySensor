#include "BlinkState.hpp"

BlinkState::BlinkState(uint32_t blink_rate_ms)
    : blink_state_value{true},
      blink_state_start{0},
      blink_rate_ms{blink_rate_ms}
{
    reset_blink_state();
}

Value<bool> &BlinkState::get_blink_value()
{
    return blink_state_value;
}

void BlinkState::reset_blink_state(bool target_state)
{
    if (target_state)
    {
        blink_state_start = millis();
    }
    else
    {
        blink_state_start = millis() - blink_rate_ms / 2;
    }
    update();
}

void BlinkState::update()
{
    uint32_t phase = (millis() - blink_state_start) % blink_rate_ms;
    bool blink_value = phase < blink_rate_ms / 2;
    if (blink_state_value.get() != blink_value)
    {
        blink_state_value.update(blink_value);
    }
}
