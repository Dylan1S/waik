#ifndef BOARD_H
#define BOARD_H

#include <driver/gpio.h>


//These are the pin definitions for the INMP441 Microphone
#define MIC_I2S_BCK_PIN     GPIO_NUM_8
#define MIC_I2S_WS_PIN      GPIO_NUM_21
#define MIC_I2S_DATA_PIN    GPIO_NUM_47


//These are the pin definitions for the MAX98357A amp
#define SPK_I2S_BCK_PIN    GPIO_NUM_38
#define SPK_I2S_LRC_PIN    GPIO_NUM_38
#define SPK_I2S_DATA_PIN   GPIO_NUM_1


#endif