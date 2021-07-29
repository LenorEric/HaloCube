//
// Created by Lenor on 2021/7/23.
//

#ifndef HALOCUBE_DEV_HALOCUBE_OLED_H
#define HALOCUBE_DEV_HALOCUBE_OLED_H

#include "OLED_Data.h"
#include "GLOBAL_INCLUDE.h"

#define OLED_ADDRESS 0x78
#define SetPosComp 3

void OLED_I2C_Write_DMA(uint16_t MemAddress, uint8_t *pData, uint16_t Size);

void OLED_Init(void);

void OLED_Clear(void);

void OLED_Refresh(void);

void OLED_Auto_Refresh(void);

void OLED_On(void);

#endif //HALOCUBE_DEV_HALOCUBE_OLED_H
