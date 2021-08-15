//
// Created by Lenor on 2021/7/26.
//

#ifndef HALOCUBE_DEV_GLOBAL_INCLUDE_H
#define HALOCUBE_DEV_GLOBAL_INCLUDE_H

#include "stm32f4xx_hal.h"
#include "USART_REFACTOR.h"

//#define MPU6050_IGNORE_SELF_TEST
//#define MPU6050_DMP_ON

//#define OLED_HOLO_DISPLAY

#define DL_NONE 0
#define DL_NORMAL 1
#define DL_ERROR 2
#define DL_WARNING 3
#define DL_DEBUG 4
#define DL_LOG 5

#define DebugLevel DL_NORMAL

//#define ESP_RESTART_WHEN_INIT
//#define ESP_CONNECT_AP_WHEN_INIT

#define THROW_ERROR while(1) HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_10)

#endif //HALOCUBE_DEV_GLOBAL_INCLUDE_H
