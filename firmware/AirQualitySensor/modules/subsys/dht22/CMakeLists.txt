
zephyr_library_named(dht22)
zephyr_library_sources_ifdef(CONFIG_DHT22 dht22.c)
zephyr_include_directories(.)
