#include "audio_pipeline.h"
#include "board.h"
#include "app_config.h"
#include "speaker.h"
#include "mic.h"
#include "esp_afe_config.h"
#include "esp_afe_sr_iface.h"
#include "esp_afe_sr_models.h"
#include "esp_log.h"
#include "led_strip.h"

namespace {
    const char* TAG {"AudioPipeline"};

    led_strip_handle_t      rgb_led     = nullptr;
    const esp_afe_sr_iface_t* afe_handle = nullptr;
    esp_afe_sr_data_t*      afe_data    = nullptr;

    esp_err_t init_rgb()
    {
        led_strip_config_t strip_config = {
            .strip_gpio_num = 48,
            .max_leds = 1,
            .led_model = LED_MODEL_WS2812,
            .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
            .flags = { .invert_out = false }
        };
        led_strip_rmt_config_t rmt_config = {
            .clk_src = RMT_CLK_SRC_DEFAULT,
            .resolution_hz = 10 * 1000 * 1000,
            .mem_block_symbols = 64,
            .flags = { .with_dma = false }
        };
        return led_strip_new_rmt_device(&strip_config, &rmt_config, &rgb_led);
    }

    void set_led(uint8_t r, uint8_t g, uint8_t b)
    {
        if (!rgb_led) return;
        led_strip_set_pixel(rgb_led, 0, r, g, b);
        led_strip_refresh(rgb_led);
    }

    void configure_vad(afe_config_t* config)
    {
        if (config == nullptr) {
            ESP_LOGE(TAG, "can't configure VAD, afe_config is nullptr");
            return;
        }
        config->vad_init          = true;
        config->vad_min_noise_ms  = 1024;
        config->vad_min_speech_ms = 128;
        config->vad_delay_ms      = 128;
        config->vad_mode          = VAD_MODE_2;
    }

    void fetch_task (void* arg)
    {
        while (true) {
            afe_fetch_result_t* result = afe_handle->fetch(afe_data);
            if (!result) continue;

            if (result->vad_state == vad_state_t::VAD_SPEECH) {
                set_led(0, 22, 1);
            } else {
                set_led(0, 0, 0);
            }
        }
    }

    void feed_task(void* arg)
    {
        int feed_chunksize = afe_handle->get_feed_chunksize(afe_data);
        int feed_channels  = afe_handle->get_feed_channel_num(afe_data);
        int feed_bytes     = feed_chunksize * feed_channels * sizeof(int16_t);

        int16_t* feed_buf = (int16_t*)heap_caps_malloc(feed_bytes, MALLOC_CAP_INTERNAL);
        if (!feed_buf) {
            ESP_LOGE(TAG, "Failed to allocate feed buffer");
            vTaskDelete(nullptr);
            return;
        }

        while (true) {
            size_t bytes_read = 0;
            size_t num_samples = feed_chunksize * feed_channels;
            uint32_t timeout = portMAX_DELAY;
            esp_err_t ret = mic_read(feed_buf, num_samples, &bytes_read, timeout);
            if (ret != ESP_OK || bytes_read == 0) {
                ESP_LOGW(TAG, "mic_read error: %s, halting to feed task", esp_err_to_name(ret));
                vTaskDelete(nullptr);
                return;
            }
            if (bytes_read == 0) continue;
            afe_handle->feed(afe_data, feed_buf);
        }
    }
}

esp_err_t AudioPipeline::init()
{
    esp_err_t ret = init_rgb();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to init RGB LED");
        return ret;
    }

    ret = mic_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to init mic");
        return ret;
    }

    ret = mic_enable();
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to enable mic");
        return ret;
    }

    srmodel_list_t* models = esp_srmodel_init("model");
    if (!models) {
        ESP_LOGE(TAG, "Failed to load SR models");
        return ESP_FAIL;
    }

    afe_config_t* afe_config = afe_config_init("MR", models, AFE_TYPE_SR, AFE_MODE_HIGH_PERF);
    if (!afe_config) {
        ESP_LOGE(TAG, "Failed to create AFE config");
        return ESP_FAIL;
    }

    configure_vad(afe_config);

    afe_handle = esp_afe_handle_from_config(afe_config);
    if (!afe_handle) {
        ESP_LOGE(TAG, "Failed to create AFE handle");
        return ESP_FAIL;
    }

    afe_data = afe_handle->create_from_config(afe_config);
    if (!afe_data) {
        ESP_LOGE(TAG, "Failed to create AFE data");
        return ESP_FAIL;
    }

    afe_config_free(afe_config);
    return ESP_OK;
}
esp_err_t AudioPipeline::begin(void)
{
    if (!afe_handle || !afe_data) {
        ESP_LOGE(TAG, "Pipeline not initialised, call init() first");
        return ESP_ERR_INVALID_STATE;
    }

    xTaskCreatePinnedToCore(feed_task,  "afe_feed",  4096, nullptr, 5, nullptr, 0);
    xTaskCreatePinnedToCore(fetch_task, "afe_fetch", 4096, nullptr, 6, nullptr, 1);

    return ESP_OK;
}

esp_err_t AudioPipeline::reset(void)
{
    return ESP_OK;
}

esp_err_t AudioPipeline::disable(void)
{
    set_led(0, 0, 0);
    return ESP_OK;
}