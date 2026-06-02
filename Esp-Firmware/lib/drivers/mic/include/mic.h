#ifndef MIC_H
#define MIC_H

#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#include "board.h"
#include "app_config.h"

#ifdef DEBUG
    #include "esp_log.h"
#endif

//function definitions
esp_err_t mic_init(void);
esp_err_t mic_enable(void);
esp_err_t mic_read(int16_t *buf, size_t num_samples, size_t *bytes_read, uint32_t timeout_ms);
esp_err_t mic_disable(void);
esp_err_t mic_deinit(void);


#endif