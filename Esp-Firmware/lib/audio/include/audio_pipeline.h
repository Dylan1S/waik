#ifndef _AUDIO_PIPE_H_
#define _AUDIO_PIPE_H_
#include "board.h"
#include "app_config.h"
#include "speaker.h"
#include "mic.h"
#include "freertos/FreeRTOS.h"

typedef struct audio_pipeline_obj_t* audio_pipeline_handle_t;

esp_err_t audio_pipeline_init(void);

esp_err_t audio_pipeline_begin();

esp_err_t audio_pipeline_reset();

esp_err_t audio_pipeline_disable(void);





#endif