#ifndef SPKR_H
#define SPKR_H

#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#include "board.h"
#include "app_config.h"

#ifdef DEBUG
    #include "esp_log.h"
#endif 

//function definitions
esp_err_t spkr_init(void);
esp_err_t spkr_enable(void);
esp_err_t spkr_write(int16_t *buf, size_t bytes_to_write, size_t *bytes_read, uint32_t timeout_ms);
esp_err_t spkr_disable(void);
esp_err_t sprk_deinit(void);

#endif