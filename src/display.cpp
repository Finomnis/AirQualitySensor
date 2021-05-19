#include "display.hpp"

#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSansOblique9pt7b.h>

Display::Display()
{
}

namespace
{
    constexpr size_t DISPLAY_WIDTH = 128;
    constexpr size_t DISPLAY_HEIGHT = 32;
    constexpr size_t STRINGBUFFER_SIZE = 128;
    char stringBuffer[STRINGBUFFER_SIZE];
}

namespace
{
    enum VerticalAlign
    {
        V_BOTTOM,
        V_CENTER,
        V_TOP
    };
    enum HorizontalAlign
    {
        H_LEFT,
        H_CENTER,
        H_RIGHT
    };
    void drawText(Adafruit_SSD1306 &display,
                  const char *text,
                  int pos_x, int pos_y,
                  HorizontalAlign align_h, VerticalAlign align_v,
                  int *rendered_width = nullptr, int *rendered_height = nullptr)
    {
        int16_t start_x, start_y;
        uint16_t width, height;
        display.getTextBounds(stringBuffer, 0, 0, &start_x, &start_y, &width, &height);

        int absolute_top{0};
        int absolute_left{0};

        switch (align_h)
        {
        case HorizontalAlign::H_LEFT:
            absolute_left = pos_x;
            break;
        case HorizontalAlign::H_CENTER:
            absolute_left = pos_x - width / 2;
            break;
        case HorizontalAlign::H_RIGHT:
            absolute_left = pos_x - width + 1;
            break;
        }
        switch (align_v)
        {
        case VerticalAlign::V_TOP:
            absolute_top = pos_y;
            break;
        case VerticalAlign::V_CENTER:
            absolute_top = pos_y - height / 2;
            break;
        case VerticalAlign::V_BOTTOM:
            absolute_top = pos_y - height + 1;
            break;
        }

        int cursor_x = absolute_left - start_x;
        int cursor_y = absolute_top - start_y;

        display.setCursor(cursor_x, cursor_y);
        display.print(stringBuffer);

        if (rendered_height != nullptr)
            *rendered_height = height;
        if (rendered_width != nullptr)
            *rendered_width = width;
    }
}

void Display::setup()
{
    display = Adafruit_SSD1306(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.dim(true);

    display.clearDisplay();
    drawText(display, "Initializing ...", DISPLAY_WIDTH / 2, DISPLAY_WIDTH / 2, H_CENTER, V_CENTER);
    display.display(); // actually display all of the above
}

void Display::update(float temperature, float humidity, uint16_t co2, StatusLEDs::Status temp_humidity_status, StatusLEDs::Status co2_status)
{
    bool changed = (displayedTemperature != temperature) ||
                   (displayedHumidity != humidity) ||
                   (displayedCO2 != co2) ||
                   (displayedTempHumidityStatus != temp_humidity_status) ||
                   (displayedCO2Status != co2_status);
    if (changed)
    {
        displayedTemperature = temperature;
        displayedHumidity = humidity;
        displayedCO2 = co2;
        displayedTempHumidityStatus = temp_humidity_status;
        displayedCO2Status = co2_status;

        display.clearDisplay();

        // Render CO2
        if (displayedCO2Status == StatusLEDs::ERROR ||
            0 > snprintf(stringBuffer, STRINGBUFFER_SIZE, "%d", displayedCO2))
        {
            stringBuffer[0] = '-';
            stringBuffer[1] = '-';
            stringBuffer[2] = '\0';
        }
        display.setFont(&FreeSans18pt7b);
        drawText(display, stringBuffer, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT / 2, H_RIGHT, V_CENTER);

        // Render Temperature
        if (displayedTempHumidityStatus == StatusLEDs::ERROR ||
            0 > snprintf(stringBuffer, STRINGBUFFER_SIZE, "%.0f C", displayedTemperature))
        {
            stringBuffer[0] = '-';
            stringBuffer[1] = ' ';
            stringBuffer[2] = 'C';
            stringBuffer[3] = '\0';
        }
        display.setFont(&FreeSansOblique9pt7b);
        int temperature_width{0};
        drawText(display, stringBuffer, 0, 0, H_LEFT, V_TOP, &temperature_width);
        // Draw degree symbol. Sadly not part of the font.
        display.drawFastHLine(temperature_width - 15, 0, 2, SSD1306_WHITE);
        display.drawFastHLine(temperature_width - 15, 3, 2, SSD1306_WHITE);
        display.drawFastVLine(temperature_width - 16, 1, 2, SSD1306_WHITE);
        display.drawFastVLine(temperature_width - 13, 1, 2, SSD1306_WHITE);

        // Render Humidity
        if (displayedTempHumidityStatus == StatusLEDs::ERROR ||
            0 > snprintf(stringBuffer, STRINGBUFFER_SIZE, "%.0f%%", displayedHumidity))
        {
            stringBuffer[0] = '-';
            stringBuffer[1] = '%';
            stringBuffer[2] = '\0';
        }
        display.setFont(&FreeSansOblique9pt7b);
        drawText(display, stringBuffer, 0, DISPLAY_HEIGHT - 1, H_LEFT, V_BOTTOM);

        display.display(); // actually display all of the above
    }
}
