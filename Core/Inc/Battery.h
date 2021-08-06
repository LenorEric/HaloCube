//
// Created by Lenor on 2021/8/6.
//

#ifndef HALOCUBE_BATTERY_H
#define HALOCUBE_BATTERY_H

#include "GLOBAL_INCLUDE.h"
#include "USART_REFACTOR.h"
#include "math.h"

#define VRef 3.3f

#define Battery_Stage (uint8_t)fmax(fmin((GLOBAL_BATTERY_INDICATOR*VRef*0.1699346f-73.667f), 13), 0)

extern uint8_t GLOBAL_BATTERY_INDICATOR;

uint8_t HCB_GetBattery();

#endif //HALOCUBE_BATTERY_H
