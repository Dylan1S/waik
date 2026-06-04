#include "mic.h"

static i2s_chan_handle_t rx_handle;
static const char *MIC_TAG = "Microphone:";

//function definitions
esp_err_t mic_init(void)
{
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    //Registering I2S channel
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, NULL, &rx_handle));

    //setting up config for mic I2S channel
    i2s_std_config_t std_cfg = {
        .clk_cfg ={
            .sample_rate_hz = SAMPLE_RATE,
            .clk_src        = I2S_CLK_SRC_PLL_160M,
            .mclk_multiple  = I2S_MCLK_MULTIPLE_256
        },
        .slot_cfg = I2S_STD_PHILIP_SLOT_DEFAULT_CONFIG(
            I2S_DATA_BIT_WIDTH_16BIT,
            I2S_SLOT_MODE_MONO
        ),
        .gpio_cfg = {
            .mclk   = I2S_GPIO_UNUSED,
            .bclk   = MIC_I2S_BCK_PIN,
            .ws     = MIC_I2S_WS_PIN,
            .dout   = I2S_GPIO_UNUSED,
            .din    = MIC_I2S_DATA_PIN,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false
            }
        }
    };

    //initializing I2S channel
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(rx_handle, &std_cfg));
    #ifdef DEBUG
        ESP_LOGI(MIC_TAG, "I2S initialized - BCK: %d, WS: %d, DIN %D",
                MIC_I2S_BCK_PIN, MIC_I2S_WS_PIN, MIC_I2S_DATA_PIN);
    #endif
    return ESP_OK;
}

/*Enable the mic's I2S Channel. Mic must be initialized*/
esp_err_t mic_enable(void)
{
    ESP_ERROR_CHECK(i2s_channel_enable(rx_handle));
    return ESP_OK;
}
/*Disable the mic but don't delete it's I2S Channel*/
esp_err_t mic_disable(void)
{
    ESP_ERROR_CHECK(i2s_channel_disable(rx_handle));

    return ESP_OK;
}

/*Delete the mic's I2S Channel*/
esp_err_t mic_deinit(void)
{
    ESP_ERROR_CHECK(i2s_del_channel(rx_handle));

    return ESP_OK;
}

esp_err_t mic_read(int16_t *buf, size_t num_samples, size_t *bytes_read, uint32_t timeout_ms)
{
    esp_err_t ret = i2s_channel_read(
        rx_handle,
        buf,
        sizeof(buf),
        bytes_read,
        pdMS_TO_TICKS(timeout_ms)
    );
    if (ret == ESP_OK && bytes_read > 0) {
        size_t samples_read = *bytes_read/sizeof(int16_t);
        #ifdef DEBUG
            ESP_LOGW(MIC_TAG, "I2S read failed or timed out: %s", esp_err_to_name(ret));
        #endif
    }

    return ESP_OK;
}

