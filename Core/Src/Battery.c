//
// Created by Lenor on 2021/8/6.
//

#include "Battery.h"

extern ADC_HandleTypeDef hadc3;

uint8_t GLOBAL_BATTERY_INDICATOR = 0;

uint8_t HCB_GetBattery(){
    uint8_t GetBattery = 0;
    HAL_ADC_Start(&hadc3);
    HAL_ADC_PollForConversion(&hadc3, 10);
    GetBattery = HAL_ADC_GetValue(&hadc3);
    printf("Battery Got, %d\r\n", GetBattery);
    if (GetBattery) GLOBAL_BATTERY_INDICATOR = GetBattery;
    return GetBattery;
}