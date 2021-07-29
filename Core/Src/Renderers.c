//
// Created by Lenor on 2021/7/25.
//

#include "Renderers.h"
#include "OLED_Data.h"

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
    HAL_DMA_Start(&hdma_memtomem_dma2_stream0, (uint32_t) HaloCubeAnimation[GLOBAL_FRAME_INDICATOR < 51 ? GLOBAL_FRAME_INDICATOR : 50][0],
                  (uint32_t) OLEDBuffer, 1024);
    return 0;
}


//main
uint8_t RENDER_mainPage() {
    if (GLOBAL_SELECT_FLAG)
        return 1;
    HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream0, HAL_DMA_FULL_TRANSFER, 2000);
    HAL_DMA_Start(&hdma_memtomem_dma2_stream0, (uint32_t) IMG_timePresent,
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
                    OLEDTemp[(x + StartPoint[i][1]) / 8][y + StartPoint[i][0]] |= 1 << ((x + StartPoint[i][1]) % 8);
            }
        }
    }
    HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream0, HAL_DMA_FULL_TRANSFER, 2000);
    HAL_DMA_Start(&hdma_memtomem_dma2_stream0, (uint32_t) OLEDTemp[0], (uint32_t) OLEDBuffer, 1024);
    return 0;
}