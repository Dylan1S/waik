#ifndef MIC_H
#define MIC_H

#include "esp_err.h"


//function definitions
esp_err_t mic_init(void);
esp_err_t mic_enable(void);
esp_err_t mic_read(int16_t *buf, size_t num_samples, size_t *bytes_read, uint32_t timeout_ms);
esp_err_t mic_disable(void);
esp_err_t mic_deinit(void);


#endif