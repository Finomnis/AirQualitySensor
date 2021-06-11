from homie.constants import EXT_FW, EXT_STATS
import credentials

###
# Wifi settings
###

WIFI_CREDENTIALS = credentials.WIFI_CREDENTIALS


###
# MQTT settings
###

# Broker IP or DNS Name
MQTT_BROKER = "docker"


###
# Device settings
###

# The device ID for registration at the broker. The device id is also the
# base topic of a device and must be unique and bytestring.
DEVICE_ID = "air-sensor-1"

# Friendly name of the device as bytestring
DEVICE_NAME = "Air Sensor 1"

EXTENSIONS = [
    EXT_FW,
    EXT_STATS,
]
