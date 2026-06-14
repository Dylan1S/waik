#include "audio_pipeline.h"
#include "board.h"
#include "app_config.h"
#include "speaker.h"
#include "mic.h"

#include "esp_aec.h"
#include "esp_afe_config.h"
#include "esp_afe_sr_iface.h"
#include "esp_afe_aec.h"
#include "esp_afe_sr_models.h"
#include "esp_vad.h"
#include "esp_log.h"


namespace{
    const char* TAG {"AudioPipeline"};

    void configure_vad(afe_config_t* config)
    {
        if(config == nullptr)
        {
            ESP_LOGE(TAG, "can't configure VAD, afe_config is nullptr");
            return;
        }
        config->vad_init = true;
        config->vad_min_noise_ms = 1024;
        config->vad_min_speech_ms = 128;
        config->vad_delay_ms = 128;
        config->vad_mode = VAD_MODE_1;
    }

    void configure_aec(afe_config_t* config)
    {
        if(config == nullptr)
        {
            ESP_LOGE(TAG, "can't configure AEC, afe_config is nullptr");
            return;
        }
    }




}

esp_err_t AudioPipeline::init()
{
    srmodel_list_t* models = esp_srmodel_init("model");
    if(!models){
         ESP_LOGE(TAG, "Failed to load SR models");
         return ESP_FAIL;
    }

    afe_config_t* afe_config = afe_config_init("MR", models, AFE_TYPE_SR, AFE_MODE_HIGH_PERF);
    if(!afe_config){
        ESP_LOGE(TAG, "Failed to create AFE config");
        return ESP_FAIL;
    }

    configure_vad(afe_config);

    const esp_afe_sr_iface_t *afe_handle = esp_afe_handle_from_config(afe_config);
    if(!afe_handle){
        ESP_LOGE(TAG, "Failed to create AFE handle from AFE config");
        return ESP_FAIL;
    }
    
    esp_afe_sr_data_t *afe_data = afe_handle->create_from_config(afe_config);
    

    return ESP_OK;
}

esp_err_t AudioPipeline::begin(void)
{


    return ESP_OK;
}


esp_err_t AudioPipeline::reset(void)
{

    return ESP_OK;
}

esp_err_t AudioPipeline::disable(void)
{

    return ESP_OK;
}