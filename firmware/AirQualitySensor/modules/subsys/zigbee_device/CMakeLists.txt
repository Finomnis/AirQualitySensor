
zephyr_library_named(subsys_zigbee_device)
zephyr_library_sources_ifdef(CONFIG_SUBSYS_ZIGBEE_DEVICE
    zigbee_device.c
    publish_values.c
    zb_airquality_sensor/co2_cluster.c
    zb_airquality_sensor/device.c
)
zephyr_include_directories(include)

zephyr_library_link_libraries(zboss zigbee)
