//
// Created by Lenor on 2021/7/26.
//

#include <FakeMPU6050.h>

extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;

void FGetDirection(uint8_t *direction) {
    HAL_ADC_Start(&hadc2);
    HAL_ADC_Start(&hadc3);
    if (HAL_ADC_PollForConversion(&hadc2, 10)) return;
    if (HAL_ADC_PollForConversion(&hadc3, 10)) return;
    int8_t x;
    int8_t y;
    x = HAL_ADC_GetValue(&hadc2) - 127;
    y = HAL_ADC_GetValue(&hadc3) - 127;
    HAL_ADC_Stop(&hadc2);
    HAL_ADC_Stop(&hadc3);
    direction[1] = sqrt(x * x + y * y) * 7 / 18;
    uint16_t temp = atan((double) y / (double) x)/3.14*180 + 360;
    if (x<0) temp += 180;
    direction[0] = (temp%360)*255/360;
}

