#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "audio_pipeline.h"


// Pin config — matches your wiring
#define I2S_BCK_PIN     GPIO_NUM_8
#define I2S_WS_PIN      GPIO_NUM_21
#define I2S_DATA_PIN    GPIO_NUM_47

// Audio config
#define SAMPLE_RATE     16000
#define BUFFER_SIZE     512     // samples per read


extern "C" void app_main(void)
{
    AudioPipeline audio_pipeline;
    audio_pipeline.init();
    audio_pipeline.begin();
}