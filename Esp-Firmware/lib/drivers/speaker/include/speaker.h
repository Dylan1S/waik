#ifndef SPK_H
#define SPK_H

#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#include "board.h"
#include "app_config.h"

#ifdef DEBUG
    #include "esp_log.h"
#endif 

//function definitions
esp_err_t spk_init(void);
esp_err_t spk_enable(void);
esp_err_t spk_write(int16_t *buf, size_t bytes_to_write, size_t *bytes_read, uint32_t timeout_ms);
esp_err_t spk_disable(void);
esp_err_t spk_deinit(void);

#endif