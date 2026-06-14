#ifndef SPK_H
#define SPK_H

#include "esp_err.h"
#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

esp_err_t spk_init(void);
esp_err_t spk_enable(void);
esp_err_t spk_write(int16_t *buf, size_t bytes_to_write, size_t *bytes_read, uint32_t timeout_ms);
esp_err_t spk_disable(void);
esp_err_t spk_deinit(void);

#ifdef __cplusplus
}
#endif

#endif