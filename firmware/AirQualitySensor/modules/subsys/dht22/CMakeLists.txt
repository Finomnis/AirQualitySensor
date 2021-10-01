
zephyr_library_named(subsys_dht22)
zephyr_library_sources_ifdef(CONFIG_SUBSYS_DHT22 dht22.c)
zephyr_include_directories(.)
