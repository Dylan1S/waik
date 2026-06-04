#include "audio_pipeline.h"
#include "board.h"
#include "app_config.h"
#include "speaker.h"
#include "mic.h"
#include "freertos/FreeRTOS.h"
#include "esp_aec.h"
#include "esp_afe_config.h"
#include "esp_afe_sr_iface.h"

namespace{
    const esp_afe_sr_iface_t *afe_handle = NULL;

}

esp_err_t audio_pipeline_init(void)
{
    srmodel_list_t* models = esp_srmodel_init("model");
    afe_config_t* afe_config = afe_config_init("MR", models, AFE_TYPE_SR, AFE_MODE_HIGH_PERF);  
}

esp_err_t audio_pipeline_begin(void)
{

}


esp_err_t audio_pipeline_reset(void)
{

}

esp_err_t audio_pipeline_disable(void)
{

}