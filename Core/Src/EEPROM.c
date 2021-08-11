//
// Created by Lenor on 2021/8/11.
//

#include "EEPROM.h"

extern I2C_HandleTypeDef hi2c3;
extern uint32_t EnergyRecord[32][32];

void EEPROM_I2C_Write_DMA(uint16_t MemAddress, uint8_t *pData, uint16_t Size) {
    while ((&hi2c3)->State != HAL_I2C_STATE_READY);
    HAL_I2C_Mem_Write_DMA(&hi2c3, EEPROM_ADDRESS, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, Size);
}

HAL_StatusTypeDef EEPROM_I2C_Read(uint16_t MemAddress, uint8_t *pData, uint16_t Size) {
    while ((&hi2c3)->State != HAL_I2C_STATE_READY);
    return HAL_I2C_Mem_Read(&hi2c3, EEPROM_ADDRESS | 1, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, Size, 100);
}

uint8_t EEPROM_init() {
    uint32_t timeStamp;
    EEPROM_Read_Data(0, &timeStamp, sizeof(timeStamp));
    GLOBAL_TIME_INDICATOR.timestampOfSec = timeStamp;
    EEPROM_Read_Data(1 + DT_TS_TODAY * 32, EnergyRecord[DT_TS_TODAY], sizeof(EnergyRecord[0]));
    return 0;
}

void EEPROM_Save_Data(uint16_t pos, void* pData, uint16_t Size) {
    if (pos >= 1024)
        return;
    uint8_t verify[Size];
    do {
        EEPROM_I2C_Write_DMA(pos * 4, pData, Size);
        HAL_Delay(5);
        EEPROM_Read_Data(pos, verify, Size);
    } while (memcmp(pData, verify, Size));
}

void EEPROM_Read_Data(uint16_t pos, void *pData, uint16_t Size) {
    if (pos >= 1024)
        return;
    uint8_t try = 245;
    while (EEPROM_I2C_Read(pos * 4, pData, Size) && try++)
        HAL_Delay(5);
}

void EEPROM_Data_Save_Task() {
    if (GLOBAL_TIME_INDICATOR.timestampOfSec)
        EEPROM_Save_Data(0, &GLOBAL_TIME_INDICATOR.timestampOfSec, 4);
    EEPROM_Save_Data(1 + DT_TS_TODAY * 32, EnergyRecord[DT_TS_TODAY], sizeof(EnergyRecord[0]));
}