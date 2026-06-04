#ifndef _AUDIO_PIPE_H_
#define _AUDIO_PIPE_H_



class AudioPipeline
{
public:
    esp_err_t init();
    esp_err_t begin();
    esp_err_t reset();
    esp_err_t disable();

private:
    TaskHandle_t    m_capture_task = nullptr;
    QueueHandle_t   m_audio_queue = nullptr;
    bool            m_is_running = false;
};



#endif