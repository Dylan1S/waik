#include "speaker.h"

#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#include "board.h"
#include "app_config.h"

#ifdef DEBUG
    #include "esp_log.h"
#endif 


static i2s_chan_handle_t tx_handle;
static const char *SPK_TAG = "Speaker: ";

//function implementations

esp_err_t spk_init(void)
{
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_1, I2S_ROLE_SLAVE);

    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &tx_handle, NULL));

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(SAMPLE_RATE),
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(
            I2S_DATA_BIT_WIDTH_16BIT,
            I2S_SLOT_MODE_MONO
        ),
        .gpio_cfg = {
            .mclk   = I2S_GPIO_UNUSED,
            .bclk   = SPK_I2S_BCK_PIN,
            .ws     = SPK_I2S_LRC_PIN,
            .dout   = I2S_GPIO_UNUSED,
            .din    = SPK_I2S_DATA_PIN,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false
            }
        }
    };

    ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_handle, &std_cfg));
    #ifdef DEBUG
        ESP_LOGI(SPK_TAG, "I2S initialized - BCK: %d, WS: %d, DOUT: %d",
        SPK_I2S_BCK_PIN, SPK_I2S_LRC_PIN, SPK_I2S_DATA_PIN);
    #endif

    return ESP_OK;
}


/*Enable the mic's I2S Channel. Mic must be initialized*/
esp_err_t spk_enable(void)
{
    ESP_ERROR_CHECK(i2s_channel_enable(tx_handle));
    
    return ESP_OK;
}
/*Disable the mic but don't delete it's I2S Channel*/
esp_err_t spk_disable(void)
{
    ESP_ERROR_CHECK(i2s_channel_disable(tx_handle));
    
    return ESP_OK;
}

/*Delete the mic's I2S Channel*/
esp_err_t spk_deinit(void)
{
    ESP_ERROR_CHECK(i2s_del_channel(tx_handle));
    
    return ESP_OK;
}

esp_err_t spk_write(int16_t *buf, size_t bytes_to_write, size_t *bytes_read, uint32_t timeout_ms)
{
    esp_err_t ret = i2s_channel_write(
        tx_handle,
        buf,
        sizeof(buf),
        bytes_read,
        pdMS_TO_TICKS(timeout_ms)
    );
    if (ret == ESP_OK && bytes_read > 0) {
        size_t samples_read = *bytes_read/sizeof(int16_t);
        #ifdef DEBUG
            ESP_LOGW(SPK_TAG, "I2S write failed or timed out: %s", esp_err_to_name(ret));
        #endif
    }
    
    return ESP_OK;
}