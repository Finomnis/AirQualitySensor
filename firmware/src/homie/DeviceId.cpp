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
            snprintf(value, sizeof(value), "air-sensor-%012llx", chipid);
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
