idf_component_register(
    SRCS "mic.c"
    INCLUDE_DIRS "include"
    REQUIRES config esp_driver_i2s esp_driver_gpio
)