#include "DeviceId.hpp"

#include <Arduino.h>

namespace
{
    struct Identifier
    {
    public:
        Identifier()
        {
            uint64_t chipid = ESP.getEfuseMac() & 0xffffffffffff;
            snprintf(value, sizeof(value), "air-sensor-%02x%02x%02x%02x%02x%02x",
                     uint8_t((chipid >> 0) & 0xff),
                     uint8_t((chipid >> 8) & 0xff),
                     uint8_t((chipid >> 16) & 0xff),
                     uint8_t((chipid >> 24) & 0xff),
                     uint8_t((chipid >> 32) & 0xff),
                     uint8_t((chipid >> 40) & 0xff));
        }
        static constexpr size_t ID_MAX_LEN = 128;
        char value[ID_MAX_LEN];
    };
}

const char *get_device_identifier()
{
    static Identifier identifier;
    return identifier.value;
};
