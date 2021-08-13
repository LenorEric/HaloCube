//
// Created by Lenor on 2021/8/11.
//

#ifndef HALOCUBE_EEPROM_H
#define HALOCUBE_EEPROM_H

#include "GLOBAL_INCLUDE.h"
#include "def_time.h"

#define EEPROM_ADDRESS 0xA0u

uint8_t EEPROM_init();
void EEPROM_I2C_Write_DMA(uint16_t MemAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef EEPROM_I2C_Read(uint16_t MemAddress, uint8_t *pData, uint16_t Size);

void EEPROM_Save_Data(uint16_t pos, uint8_t *pData, uint16_t Size);
void EEPROM_Read_Data(uint16_t pos, uint8_t *pData, uint16_t Size);

void EEPROM_Data_Save_Task();

void EEPROM_Clear();

#endif //HALOCUBE_EEPROM_H
