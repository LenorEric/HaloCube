//
// Created by Lenor on 2021/7/26.
//

#ifndef HALOCUBE_DEV_GLOBAL_INCLUDE_H
#define HALOCUBE_DEV_GLOBAL_INCLUDE_H

#include "stm32f4xx_hal.h"

//#define MPU6050_IGNORE_SELF_TEST
//#define MPU6050_DMP_ON

//#define OLED_HOLO_DISPLAY

#define ESP_CONNECT_AP_WHEN_INIT

#define THROW_ERROR while(1) HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_10)

#endif //HALOCUBE_DEV_GLOBAL_INCLUDE_H
