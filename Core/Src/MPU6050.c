//
// Created by Lenor on 2021/7/25.
//

#include "MPU6050.h"

extern I2C_HandleTypeDef hi2c2;

void MPU6050_I2C_Write_DMA(uint16_t MemAddress, uint8_t *pData, uint16_t Size) {
    while ((&hi2c2)->State != HAL_I2C_STATE_READY);
    HAL_I2C_Mem_Write_DMA(&hi2c2, MPU6050_ADDRESS, MemAddress, I2C_MEMADD_SIZE_8BIT, pData, Size);
}

void GetDirection(uint8_t *direction) {
    direction[0] = 0;
    direction[1] = 0;
}
