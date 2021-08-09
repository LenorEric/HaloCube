//
// Created by Lenor on 2021/7/25.
//

#include "Renderers.h"

#define SET_HIGH(x, y) OLEDTemp[(y)/8][(x)] |= 1<< ((y)%8)

extern DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
extern uint8_t OLEDBuffer[8][128];
extern uint8_t GLOBAL_PAGE_INDICATOR;
extern uint8_t GLOBAL_SELECT_FLAG;
extern uint8_t GLOBAL_INITED_FLAG;
extern Page PageList[31];

///In case of Refreshing bug
uint8_t OLEDTemp[8][128];

void updateFromTemp();

inline void updateFromTemp() {
    HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream0, HAL_DMA_FULL_TRANSFER, 2000);
    HAL_DMA_Start(&hdma_memtomem_dma2_stream0, (uint32_t) OLEDTemp[0], (uint32_t) OLEDBuffer, 1024);
}

uint16_t GLOBAL_FRAME_INDICATOR;

void PrintLetter(uint8_t letter, uint8_t xpos, uint8_t ypos) {
    uint8_t char2print;
    if (letter >= 'a' && letter <= 'z')
        char2print = letter - 'a';
    else if (letter >= '0' && letter <= '9')
        char2print = letter - '0' + 26;
    else if (letter >= 0 && letter <= 9)
        char2print = letter + 26;
    else if (letter == ' ')
        return;
    else
        char2print = 36;
    for (uint8_t y = 0; y < 9; y++) {
        for (uint8_t x = 0; x < 7; x++) {
            if (OLED_letter[char2print] & ((uint64_t) 1 << (62 - y * 7 - x))) {
                SET_HIGH(xpos + x, ypos + y);
            }
        }
    }
}

uint8_t PrintString(char *s2p, uint8_t xpos, uint8_t ypos) {
    uint8_t x = xpos;
    for (uint8_t i = 0; i < strlen(s2p); i++) {
        PrintLetter(s2p[i], x, ypos);
        switch (s2p[i]) {
            case 'm':
                x += 7;
                break;
            case 'w':
                x += 7;
                break;
            case 'i':
                x += 4;
                break;
            case 'j':
                x += 5;
                break;
            case '1':
                x += 5;
                break;
            default:
                x += 6;
                break;
        }
    }
    return x;
}

void DrawRect(uint8_t sx, uint8_t sy, uint8_t ex, uint8_t ey){
    for (uint8_t x=sx;x<ex;x++)
        for (uint8_t y=sy;y<ey;y++)
            SET_HIGH(x,y);
}

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


///main
uint8_t RENDER_MainPage() {
    if (GLOBAL_SELECT_FLAG)
        return 1;
    uint8_t BTST = Battery_Stage;
    memcpy(OLEDTemp, IMG_timeBackground, 1024);
    const uint8_t StartPoint[4] = {16, 37, 73, 94};
    uint8_t char2print[4] = {GLOBAL_TIME_INDICATOR.hour / 10 + 26, GLOBAL_TIME_INDICATOR.hour % 10 + 26,
                             GLOBAL_TIME_INDICATOR.min / 10 + 26, GLOBAL_TIME_INDICATOR.min % 10 + 26};
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t y = 0; y < 9; y++) {
            for (uint8_t x = 0; x < 7; x++) {
                if (OLED_letter[char2print[i]] & ((uint64_t) 1 << (62 - y * 7 - x))) {
                    for (uint8_t sx = 0; sx < 3; sx++) {
                        for (uint8_t sy = 0; sy < 3; sy++) {
                            SET_HIGH(StartPoint[i] + x * 3 + sx, 19 + y * 3 + sy);
                        }
                    }
                }
            }
        }
    }
    OLEDTemp[0][107] = 0x18;
    OLEDTemp[0][108] = 0x18;
    OLEDTemp[0][109] = 0xff;
    OLEDTemp[0][110] = 0x81;
    for (uint8_t i = 111; i < 124; i++)
        OLEDTemp[0][i] = BTST > i - 111 ? 0xBD : 0x81;
    OLEDTemp[0][124] = 0x81;
    OLEDTemp[0][125] = 0xff;
    updateFromTemp();
    return 0;
}

///ShortCut
uint8_t RENDER_ShortCutPage() {
    static uint8_t INITED_FLAG = 0;
    if (!INITED_FLAG) {
        INITED_FLAG = 1;
        PageList[1].selectIcon[2] = GetBulbStatus() ? ICON16_Blub_OFF : ICON16_Bulb_ON;
    }
    const uint8_t *icons[4];
    const uint8_t StartPoint[4][2] = {
            {96, 7},
            {16, 7},
            {16, 41},
            {96, 41}
    };
    if (GLOBAL_SELECT_FLAG)
        return 1;
    uint8_t bit;
    icons[1] = GLOBAL_PC_ON_FLAG ? ICON16_PC_ON : ICON16_PC_OFF;
    icons[2] = ReturnBulbStatus() ? ICON16_Bulb_ON : ICON16_Blub_OFF;
    icons[3] = ICON16_Return;
    memset(OLEDTemp, 0, sizeof(OLEDTemp));
    for (uint8_t i = 1; i < 4; i++) {
        for (uint8_t x = 0; x < 16; x++) {
            for (uint8_t y = 0; y < 16; y++) {
                bit = icons[i][(x * 16 + y) / 8] & (1 << (7 - (x * 16 + y) % 8));
                if (bit)
                    SET_HIGH(y + StartPoint[i][0], x + StartPoint[i][1]);
            }
        }
    }
    updateFromTemp();
    return 0;
}

uint8_t RENDER_TestPage() {
    if (GLOBAL_SELECT_FLAG)
        return 1;
    memset(OLEDTemp, 0, sizeof(OLEDTemp));
    for (uint16_t i = 0; i < GLOBAL_FRAME_INDICATOR; i++) {
        SET_HIGH(i % 128, i / 128);
    }
    updateFromTemp();
    return 0;
}

/*
uint8_t RENDER_BulbPage() {
    if (GLOBAL_SELECT_FLAG)
        return 1;
    HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream0, HAL_DMA_FULL_TRANSFER, 2000);
    HAL_DMA_Start(&hdma_memtomem_dma2_stream0, (uint32_t) IMG_timeDemo,
                  (uint32_t) OLEDBuffer, 1024);
    return 0;
}
*/

uint8_t RENDER_SelectingUI() {
    const uint8_t StartPoint[4][2] = {
            {96, 7},
            {16, 7},
            {16, 41},
            {96, 41}
    };
    if (!GLOBAL_SELECT_FLAG)
        return 1;
    uint8_t bit;
    Page currPage = getPage();
    memcpy(OLEDTemp, IMG_selecting, 1024);
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t x = 0; x < 16; x++) {
            for (uint8_t y = 0; y < 16; y++) {
                bit = currPage.selectIcon[i][(x * 16 + y) / 8] & (1 << (7 - (x * 16 + y) % 8));
                if (bit)
                    SET_HIGH(y + StartPoint[i][0], x + StartPoint[i][1]);
            }
        }
    }
    updateFromTemp();
    return 0;
}


uint8_t RENDER_StatisticPage() {
    if (GLOBAL_SELECT_FLAG)
        return 1;
    memset(OLEDTemp, 0, sizeof(OLEDTemp));
    const char CR[] = "consumption ranking";
    PrintString(CR, 0, 0);
    char rankNames[3][8];
    uint32_t rankConsump[3];
    uint8_t maxLen = 0, tmpLen = 0;
    memset(rankNames, 0, sizeof(rankNames));
    memset(rankConsump, 0, sizeof(rankConsump));
    Power_GiveTopThreeConsumption(rankNames, rankConsump);
    for (uint8_t i = 0; i < 3; i++) {
        tmpLen = PrintString(rankNames[i], 2, 12 + 14 * i);
        if (tmpLen > maxLen)
            maxLen = tmpLen;
    }
    if (rankConsump[0] == 0)
        goto NO_CONSUMPTION;
    DrawRect(maxLen + 2, 13, 105, 18);
    NO_CONSUMPTION:
    updateFromTemp();
    return 0;
}
