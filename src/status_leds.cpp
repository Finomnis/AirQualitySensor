#include "status_leds.hpp"

#include "pinout.hpp"

StatusLEDs::StatusLEDs(uint16_t num_leds)
    : leds{num_leds, PINS::STATUS_LEDS, NEO_GRB + NEO_KHZ400}
{
}

void StatusLEDs::setup()
{
    leds.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    leds.show();            // Turn OFF all pixels ASAP
    leds.setBrightness(10); // Set brightness
}

void StatusLEDs::startUpdate()
{
    //    currentStatus = Status::EXCELLENT;
}

void StatusLEDs::addStatus(uint16_t led_id, StatusLEDs::Status status)
{
    // if (currentStatus < status)
    // {
    //     currentStatus = status;
    // }
}

void StatusLEDs::finishUpdate()
{
    // LedPinState pinState = {false, false, false};
    // bool blinkState = (millis() / 600) % 2 == 0;

    // switch (currentStatus)
    // {
    // case Status::EXCELLENT:
    //     pinState.green = blinkState;
    //     break;
    // case Status::OK:
    //     pinState.green = true;
    //     break;
    // case Status::WARNING_WEAK:
    //     pinState.yellow = true;
    //     break;
    // case Status::WARNING_STRONG:
    //     pinState.red = true;
    //     break;
    // case Status::ERROR:
    //     pinState.red = blinkState;
    //     break;
    // }

    // if (previousPinState.red != pinState.red ||
    //     previousPinState.yellow != pinState.yellow ||
    //     previousPinState.green != pinState.green)
    // {
    //     // digitalWrite(PINS::LED_RED, pinState.red);
    //     // digitalWrite(PINS::LED_YELLOW, pinState.yellow);
    //     // digitalWrite(PINS::LED_GREEN, pinState.green);
    //     previousPinState = pinState;
    // }

    // Hue of first pixel runs 5 complete loops through the color wheel.
    // Color wheel has a range of 65536 but it's OK if we roll over, so
    // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
    // means we'll make 5*65536/256 = 1280 passes through this outer loop:
    static long firstPixelHue = 0;
    static unsigned long nextUpdate = 0;
    if (nextUpdate < millis())
    {
        nextUpdate = millis() + 10;
        firstPixelHue += 256;
        if (firstPixelHue >= 5 * 65536)
        {
            firstPixelHue = 0;
        }
        for (int i = 0; i < leds.numPixels(); i++)
        { // For each pixel in strip...
            // Offset pixel hue by an amount to make one full revolution of the
            // color wheel (range of 65536) along the length of the strip
            // (strip.numPixels() steps):
            int pixelHue = firstPixelHue + (i * 65536L / leds.numPixels());
            // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
            // optionally add saturation and value (brightness) (each 0 to 255).
            // Here we're using just the single-argument hue variant. The result
            // is passed through strip.gamma32() to provide 'truer' colors
            // before assigning to each pixel:
            leds.setPixelColor(i, leds.gamma32(leds.ColorHSV(pixelHue)));
        }
        leds.show(); // Update strip with new contents
    }
}
