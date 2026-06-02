#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "mic_test";

// Pin config — matches your wiring
#define I2S_BCK_PIN     GPIO_NUM_8
#define I2S_WS_PIN      GPIO_NUM_21
#define I2S_DATA_PIN    GPIO_NUM_47

// Audio config
#define SAMPLE_RATE     16000
#define BUFFER_SIZE     512     // samples per read

static i2s_chan_handle_t rx_handle;

static void i2s_init(void)
{
    // Channel config
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(
        I2S_NUM_0,
        I2S_ROLE_MASTER
    );

    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, NULL, &rx_handle));

    // Standard I2S config
    i2s_std_config_t std_cfg = {
        .clk_cfg = {
            .sample_rate_hz = SAMPLE_RATE,
            .clk_src        = I2S_CLK_SRC_PLL_160M,
            .mclk_multiple  = I2S_MCLK_MULTIPLE_256,
        },
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(
            I2S_DATA_BIT_WIDTH_16BIT,
            I2S_SLOT_MODE_MONO
        ),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = I2S_BCK_PIN,
            .ws   = I2S_WS_PIN,
            .dout = I2S_GPIO_UNUSED,
            .din  = I2S_DATA_PIN,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false,
            },
        },
    };

    ESP_ERROR_CHECK(i2s_channel_init_std_mode(rx_handle, &std_cfg));
    ESP_ERROR_CHECK(i2s_channel_enable(rx_handle));

    ESP_LOGI(TAG, "I2S initialised — BCK: %d, WS: %d, DIN: %d",
             I2S_BCK_PIN, I2S_WS_PIN, I2S_DATA_PIN);
}

static void mic_task(void *args)
{
    int16_t buffer[BUFFER_SIZE];
    size_t bytes_read = 0;

    ESP_LOGI(TAG, "Starting mic read loop");

    while (true) {

        esp_err_t ret = i2s_channel_read(
            rx_handle,
            buffer,
            sizeof(buffer),
            &bytes_read,
            pdMS_TO_TICKS(100)
        );

        if (ret == ESP_OK && bytes_read > 0) {
            size_t samples_read = bytes_read / sizeof(int16_t);

            // Print raw sample values over serial
            for (size_t i = 0; i < samples_read; i++) {
                printf("%d\n", buffer[i]);
            }
        } else {
            ESP_LOGW(TAG, "I2S read failed or timed out: %s", esp_err_to_name(ret));
        }
    }
}

extern "C" void app_main(void)
{
    i2s_init();
    xTaskCreatePinnedToCore(mic_task, "mic_task", 4096, NULL, 5, NULL, 0);
}