//
// Created by Lenor on 2021/7/25.
//

#include "Renderers.h"
#include "OLED_Data.h"

#define SET_HIGH(x, y) OLEDTemp[(y)/8][(x)] |= 1<< ((y)%8)

extern DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
extern DMA_HandleTypeDef hdma_memtomem_dma2_stream1;
extern uint8_t OLEDBuffer[8][128];
extern uint8_t GLOBAL_PAGE_INDICATOR;
extern uint8_t GLOBAL_SELECT_FLAG;
extern uint8_t GLOBAL_INITED_FLAG;

uint16_t GLOBAL_FRAME_INDICATOR;

uint8_t openScreenAnimation() {
    if (GLOBAL_FRAME_INDICATOR > 100 && GLOBAL_INITED_FLAG) {
        return 1;
    }
    HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream0, HAL_DMA_FULL_TRANSFER, 2000);
    HAL_DMA_Start(&hdma_memtomem_dma2_stream0,
                  (uint32_t) HaloCubeAnimation[GLOBAL_FRAME_INDICATOR < 51 ? GLOBAL_FRAME_INDICATOR : 50][0],
                  (uint32_t) OLEDBuffer, 1024);
    return 0;
}


//main
uint8_t RENDER_mainPage() {
    if (GLOBAL_SELECT_FLAG)
        return 1;
    uint8_t OLEDTemp[8][128];
    const uint8_t StartPoint[4][2] = {
            {98, 9},
            {18, 9},
            {18, 43},
            {98, 43}
    };
    uint8_t char2print[4] = {GLOBAL_TIME_INDICATOR.hour / 10 + 26, GLOBAL_TIME_INDICATOR.hour % 10 + 26,
                             GLOBAL_TIME_INDICATOR.sec / 10 + 26, GLOBAL_TIME_INDICATOR.hour % 10 + 26};
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t x = 0; x < 7; x++) {
            for (uint8_t y = 0; y < 9; y++) {
                if (OLED_letter[char2print[i]] & (1<<(63-y*7-x))){
                    for (uint8_t sx = 0;sx<3;sx++){
                        for (uint8_t sy = 0;sy<3;sy++){
                            
                        }
                    }
                }
            }
        }
    }
    memcpy(OLEDTemp, IMG_timeBackground, 1024);
    HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream0, HAL_DMA_FULL_TRANSFER, 2000);
    HAL_DMA_Start(&hdma_memtomem_dma2_stream0, (uint32_t) OLEDTemp,
                  (uint32_t) OLEDBuffer, 1024);
    return 0;
}

uint8_t RENDER_selectingUI() {
    const uint8_t StartPoint[4][2] = {
            {98, 9},
            {18, 9},
            {18, 43},
            {98, 43}
    };
    if (!GLOBAL_SELECT_FLAG)
        return 1;
    uint8_t OLEDTemp[8][128], bit;
    Page currPage = getPage();
    memcpy(OLEDTemp, IMG_selecting, 1024);
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t x = 0; x < 12; x++) {
            for (uint8_t y = 0; y < 12; y++) {
                bit = currPage.selectIcon[i][(x * 12 + y) / 8] & (1 << (7 - (x * 12 + y) % 8));
                if (bit)
                    SET_HIGH(y + StartPoint[i][0],x + StartPoint[i][1]);
            }
        }
    }
    HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream0, HAL_DMA_FULL_TRANSFER, 2000);
    HAL_DMA_Start(&hdma_memtomem_dma2_stream0, (uint32_t) OLEDTemp[0], (uint32_t) OLEDBuffer, 1024);
    return 0;
}