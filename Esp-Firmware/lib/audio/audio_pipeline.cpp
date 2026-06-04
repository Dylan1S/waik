#include "audio_pipeline.h"
#include "board.h"
#include "app_config.h"
#include "speaker.h"
#include "mic.h"
#include "esp_aec.h"
#include "esp_afe_config.h"
#include "esp_afe_sr_iface.h"

namespace{


}

esp_err_t AudioPipeline::init()
{
    srmodel_list_t* models = esp_srmodel_init("model");
    afe_config_t* afe_config = afe_config_init("MR", models, AFE_TYPE_SR, AFE_MODE_HIGH_PERF);  

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