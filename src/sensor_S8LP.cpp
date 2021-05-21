
#include "sensor_S8LP.hpp"

#include "sensor_S8LP_modbusRPC.hpp"

#include <Homie.hpp>

constexpr size_t MESSAGE_MAX_SIZE = 39;

SensorS8LP::SensorS8LP(HardwareSerial *sender, HardwareSerial *receiver, const char *id, const char *name)
    : homieNode{id, name, "airsensor"},
      sender{sender},
      receiver{receiver},
      delayMS{4000},
      nextUpdate{0},
      ledStatus{StatusLEDs::ERROR}
{
}

void SensorS8LP::setup()
{
    // Initialize device.
    Homie.getLogger().println(F("Initializing SenseAir S8 ..."));

    // Initialize homie
    homieNode.advertise("co2").setName("CO2").setDatatype("float").setUnit("ppm");

    Homie.getLogger().println("SenseAir S8 initialization finished.");
}

StatusLEDs::Status SensorS8LP::update()
{
    if (millis() > nextUpdate)
    {
        nextUpdate = millis() + delayMS;

        uint16_t data[4];
        if (!readIRegisters(0x0, 4, data))
        {
            ledStatus = StatusLEDs::ERROR;
        }
        else
        {
            if (data[0] != 0)
            {
                ledStatus = StatusLEDs::ERROR;
                Homie.getLogger().print("Sensair S8 is in an error state: 0x");
                Homie.getLogger().print(data[0]);
                Homie.getLogger().print(" 0x");
                Homie.getLogger().print(data[1]);
                Homie.getLogger().print(" 0x");
                Homie.getLogger().println(data[2]);
            }
            else
            {
                // Homie.getLogger().print("CO2: ");
                // Homie.getLogger().print(data[3], DEC);
                // Homie.getLogger().println(" ppm");
                float value = data[3];

                if (value > 1450.0f)
                {
                    ledStatus = StatusLEDs::WARNING_STRONG;
                }
                else if (value > 1000.0f)
                {
                    ledStatus = StatusLEDs::WARNING_WEAK;
                }
                else if (value > 600.0f)
                {
                    ledStatus = StatusLEDs::OK;
                }
                else
                {
                    ledStatus = StatusLEDs::EXCELLENT;
                }

                if (Homie.isConnected())
                {
                    homieNode.setProperty("co2").send(String(value));
                }

                lastCO2 = data[3];
            }
        }
    }

    return ledStatus;
}

bool SensorS8LP::readHRegisters(uint16_t start_addr, uint16_t num_registers, uint16_t *output)
{
    return readRegisters(0x03, start_addr, num_registers, output);
}

bool SensorS8LP::readIRegisters(uint16_t start_addr, uint16_t num_registers, uint16_t *output)
{
    return readRegisters(0x04, start_addr, num_registers, output);
}

bool SensorS8LP::readRegisters(uint8_t function_code, uint16_t start_addr, uint16_t num_registers, uint16_t *output)
{

    uint8_t request_data[4];
    request_data[0] = (start_addr >> 8) & 0xff;
    request_data[1] = start_addr & 0xff;
    request_data[2] = (num_registers >> 8) & 0xff;
    request_data[3] = num_registers & 0xff;

    size_t response_size = 2 * num_registers + 1;

    SensorS8LP_ModbusRPC readRegisterRPC(sender, receiver, function_code, request_data, 4, response_size);
    readRegisterRPC.wait_for_finished();
    if (!readRegisterRPC.success())
    {
        return false;
    }

    const uint8_t *response_data = readRegisterRPC.response_payload();

    for (int i = 0; i < num_registers; i++)
    {
        output[i] = uint16_t(response_data[2 * i + 1] << 8) + response_data[2 * i + 2];
    }

    return true;
}

uint16_t SensorS8LP::getLastCO2()
{
    return lastCO2;
}
