//
// Created by Lenor on 2021/7/23.
//

#include "HaloCube_OLED.h"
#include "stm32f4xx_hal.h"


extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_memtomem_dma2_stream1;
extern DMA_HandleTypeDef hdma_memtomem_dma2_stream0;

void OLED_I2C_Write_DMA(uint16_t MemAddress, uint8_t *pData, uint16_t Size) {
    while ((&hi2c1)->State != HAL_I2C_STATE_READY);
    HAL_I2C_Mem_Write_DMA(&hi2c1, OLED_ADDRESS, MemAddress, I2C_MEMADD_SIZE_8BIT, pData, Size);
}

const uint8_t CMD_Data[] = {
        0xAE, 0x40, 0x81, 0x7F, 0xA1, 0xA6, 0xA8, 0x3F,
        0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,
        0xDB, 0x30, 0x8D, 0x14, 0xAF, 0x20, 0x00
};

uint8_t OLEDBuffer[8][128] = {0x00};
uint8_t LastOLEDBuffer[8][128] = {0x00};


uint8_t POS[] = {
        0xB0, 0x00, 0x10
};

void SetPos(uint8_t page, uint8_t col) {
    POS[0] = 0xB0 + page;
    POS[1] = col & 0x0F;
    POS[2] = ((col >> 4) & 0x7) | 0x10;
    OLED_I2C_Write_DMA(0x00, POS, 3);
}

void OLED_Refresh(void) {
    SetPos(0, 0);
    OLED_I2C_Write_DMA(0x40, OLEDBuffer[0], 1024);
    HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream0, HAL_DMA_FULL_TRANSFER, 2000);
    HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream1, HAL_DMA_FULL_TRANSFER, 20);
    HAL_DMA_Start(&hdma_memtomem_dma2_stream1, (uint32_t) OLEDBuffer[0],
                  (uint32_t) LastOLEDBuffer[0], 1024);
}

void OLED_Init(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
    HAL_Delay(0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
    HAL_Delay(10);
    OLED_I2C_Write_DMA(0x00, CMD_Data, sizeof(CMD_Data));
}

void OLED_Clear(void) {
    uint8_t i, n;
    HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream0, HAL_DMA_FULL_TRANSFER, 2000);
    HAL_DMA_Start(&hdma_memtomem_dma2_stream0, (uint32_t) IMG_dark,
                  (uint32_t) OLEDBuffer, 1024);
}

uint16_t OLED_Auto_Refresh_Comp(uint8_t page, uint8_t col) {
    uint16_t Size = 0, CompSum = 0;
    while (page < 8 && OLEDBuffer[page][col] != LastOLEDBuffer[page][col]) {
        Size++;
        col++;
        if (col == 128) {
            col = 0;
            page++;
        }
    }
    if (page == 8) return Size;
    while (page < 8 && OLEDBuffer[page][col] == LastOLEDBuffer[page][col]) {
        CompSum++;
        if (CompSum > SetPosComp) return Size;
        col++;
        if (col == 128) {
            col = 0;
            page++;
        }
    }
    if (page == 8) return Size;
    Size += CompSum;
    Size += OLED_Auto_Refresh_Comp(page, col);
    return Size;
}

void OLED_Auto_Refresh(void) {
    if ((&hi2c1)->State != HAL_I2C_STATE_READY)
        return;
    HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream0, HAL_DMA_FULL_TRANSFER, 20);
    uint8_t page = 0, col = 0;
    uint16_t Size = 0;
    while (page != 8) {
        while (OLEDBuffer[page][col] == LastOLEDBuffer[page][col]) {
            col++;
            if (col == 128) {
                col = 0;
                page++;
            }
        }
        if (page == 8)
            return;
        Size = OLED_Auto_Refresh_Comp(page, col);
        SetPos(page, col);
        OLED_I2C_Write_DMA(0x40, &OLEDBuffer[page][col], Size);
        HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream1, HAL_DMA_FULL_TRANSFER, 20);
        HAL_DMA_Start(&hdma_memtomem_dma2_stream1, (uint32_t) &OLEDBuffer[page][col],
                      (uint32_t) &LastOLEDBuffer[page][col], Size);
        for (; Size; Size--) {
            col++;
            if (col == 128) {
                col = 0;
                page++;
            }
        }
    }
}


void OLED_On(void) {
    uint8_t i, n;
    for (i = 0; i < 8; i++) {
        for (n = 0; n < 128; n++)
            OLEDBuffer[i][n] = 0xFF;
    }
}
