#include "display.hpp"

Display::Display()
{
}

void Display::setup()
{
    display = Adafruit_SSD1306(128, 32, &Wire);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Initializing ...");
    display.display(); // actually display all of the above
    display.dim(true);
}

void Display::update(float temperature, float humidity, uint16_t co2, StatusLEDs::Status temp_humidity_status, StatusLEDs::Status co2_status)
{
    bool changed = (displayedTemperature != temperature) || (displayedHumidity != humidity) || (displayedCO2 != co2);
    if (changed)
    {
        displayedTemperature = temperature;
        displayedHumidity = humidity;
        displayedCO2 = co2;

        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print(" ");
        display.print(int(std::round(displayedTemperature)));
        display.print("C  ");
        display.print(int(std::round(displayedHumidity)));
        display.println("%");
        display.print(" ");
        display.print(displayedCO2);
        display.println(" ppm");
        display.display(); // actually display all of the above
    }
}
