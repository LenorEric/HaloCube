//
// Created by Lenor on 2021/7/26.
//

#ifndef HALOCUBE_DEV_GLOBAL_INCLUDE_H
#define HALOCUBE_DEV_GLOBAL_INCLUDE_H

#include "stm32f4xx_hal.h"

//#define MPU6050_IGNORE_SELF_TEST
//#define MPU6050_DMP_ON

#define THROW_ERROR while(1) HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_10)

#endif //HALOCUBE_DEV_GLOBAL_INCLUDE_H
