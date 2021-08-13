//
// Created by Lenor on 2021/8/11.
//

#include "EEPROM.h"

extern I2C_HandleTypeDef hi2c3;
extern DMA_HandleTypeDef hdma_i2c3_tx;
extern uint32_t EnergyRecord[31][32];

void EEPROM_I2C_Write_DMA(uint16_t MemAddress, uint8_t *pData, uint16_t Size) {
    while ((&hi2c3)->State != HAL_I2C_STATE_READY);
    HAL_DMA_PollForTransfer(&hdma_i2c3_tx, HAL_DMA_FULL_TRANSFER, 10000);
    HAL_I2C_Mem_Write_DMA(&hi2c3, EEPROM_ADDRESS, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, Size);
}

HAL_StatusTypeDef EEPROM_I2C_Read(uint16_t MemAddress, uint8_t *pData, uint16_t Size) {
    while ((&hi2c3)->State != HAL_I2C_STATE_READY);
    HAL_DMA_PollForTransfer(&hdma_i2c3_tx, HAL_DMA_FULL_TRANSFER, 10000);
    HAL_StatusTypeDef state =
            HAL_I2C_Mem_Read(&hi2c3, EEPROM_ADDRESS | 1, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, Size, 1000);
    return state;
}

uint8_t EEPROM_init() {
    uint32_t timeStamp=0;
    EEPROM_Read_Data(0, &timeStamp, 4);
    GLOBAL_TIME_INDICATOR.timestampOfSec = timeStamp;
    EEPROM_Read_Data(1, EnergyRecord, sizeof(EnergyRecord));
    for (uint8_t i = 0; i < 32; i++) {
        for (uint8_t j = 0; j < 32; j++) {
            if (EnergyRecord[i][j] == 0xFFFFFFFF || i == (DT_TS_TODAY + 1) % 31)
                EnergyRecord[i][j] = 0;
        }
    }
    return 0;
}

void EEPROM_Save_Data(uint16_t pos, uint8_t *pData, uint16_t Size) {
    if (pos >= 1024)
        return;
    uint16_t iter = 0;
    uint8_t verify[Size - iter > 4 ? 4 : Size - iter];
    while (iter < Size) {
        do {
            EEPROM_I2C_Write_DMA(pos * 4 + iter, pData + iter, Size - iter > 4 ? 4 : Size - iter);
            EEPROM_Read_Data(pos + iter/4, verify, Size - iter > 4 ? 4 : Size - iter);
        } while (memcmp(pData + iter, verify, Size - iter > 4 ? 4 : Size - iter));
        iter += 4;
    }
}

void EEPROM_Read_Data(uint16_t pos, uint8_t *pData, uint16_t Size) {
    if (pos >= 1024)
        return;
    uint8_t try = 245;
    while (EEPROM_I2C_Read(pos * 4, pData, Size) && try++)
        HAL_Delay(5);
}

void EEPROM_Data_Save_Task() {
    if (GLOBAL_TIME_INDICATOR.timestampOfSec)
        EEPROM_Save_Data(0, &GLOBAL_TIME_INDICATOR.timestampOfSec, 4);
    EEPROM_Save_Data(1 + DT_TS_TODAY * 32, EnergyRecord[DT_TS_TODAY], sizeof(EnergyRecord[DT_TS_TODAY]));
}

void EEPROM_Clear() {
    memset(EnergyRecord, 0, sizeof(EnergyRecord));
    EEPROM_Save_Data(1, EnergyRecord, sizeof(EnergyRecord));
}
