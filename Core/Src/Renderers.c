//
// Created by Lenor on 2021/7/25.
//

#include "Renderers.h"

#define SET_HIGH(x, y) OLEDTemp[(y)/8][(x)] |= 1<< ((y)%8)
#define sign(x) x==0?0:(x>0?1:-1)

extern DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
extern uint8_t OLEDBuffer[8][128];
extern uint8_t GLOBAL_PAGE_INDICATOR;
extern uint8_t GLOBAL_SELECT_FLAG;
extern uint8_t GLOBAL_INITED_FLAG;
extern Page PageList[31];
extern char GLOBAL_INIT_STATE_INDICATOR[20];

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
    else if (letter >= 'A' && letter <= 'Z')
        char2print = letter - 'A' + 37;
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
            case 'i':
            case 'l':
            case '!':
                x += 2;
                break;
            case '1':
            case '.':
                x += 3;
                break;
            case 'j':
            case 'r':
            case 'I':
            case '(':
            case ')':
            case ',':
            case ' ':
                x += 4;
                break;
            case 'v':
            case 'A':
            case 'C':
            case 'D':
            case 'G':
            case 'H':
            case 'N':
            case 'O':
            case 'Q':
            case 'T':
            case 'U':
            case 'V':
            case 'X':
            case 'Y':
            case '4':
            case '-':
            case '^':
            case '+':
            case '=':
                x += 6;
                break;
            case '#':
            case '&':
            case '~':
                x += 7;
                break;
            case 'm':
            case 'w':
            case 'M':
            case 'W':
            case '%':
            case '@':
                x += 8;
                break;
            default:
                x += 5;
                break;
        }
    }
    return x;
}

void DrawRect(uint8_t sx, uint8_t sy, uint8_t ex, uint8_t ey) {
    for (uint8_t x = sx; x < ex; x++)
        for (uint8_t y = sy; y < ey; y++)
            SET_HIGH(x, y);
}

void DrawLine(uint8_t sx, uint8_t sy, uint8_t ex, uint8_t ey) {
#if DebugLevel >= DL_DEBUG
    printf("DrawLine from (%d,%d) to (%d,%d)\r\n", sx, sy, ex, ey);
#endif
    float rate;
    uint8_t temp, cx, cy;
    int8_t dx = abs(sx - ex), dy = abs(sy - ey);
    float cE;
    if (dx > dy) {
        if (sx > ex) {
            temp = sx;
            sx = ex;
            ex = temp;
            temp = sy;
            sy = ey;
            ey = temp;
        }
        dx = ex - sx;
        dy = ey - sy;
        cx = sx;
        cy = sy;
        cE = cy;
        SET_HIGH(cx, cy);
        rate = (float) dy / (float) dx;
        while (cx <= ex) {
            cx++;
            cE += rate;
            if (cE - cy >= 1)
                cy+=sign(rate);
            SET_HIGH(cx, cy);
        }
    } else {
        if (sy > ey) {
            temp = sx;
            sx = ex;
            ex = temp;
            temp = sy;
            sy = ey;
            ey = temp;
        }
        dx = ex - sx;
        dy = ey - sy;
        cx = sx;
        cy = sy;
        cE = cx;
        SET_HIGH(cx, cy);
        rate = (float) dx / (float) dy;
        while (cy <= ey) {
            cy++;
            cE += rate;
            if (fabsf(cE - cx) >= 1)
                cx += sign(rate);
            SET_HIGH(cx, cy);
        }
    }
}

uint8_t openScreenAnimation() {
    if (GLOBAL_FRAME_INDICATOR > 100 && GLOBAL_INITED_FLAG) {
        return 1;
    }
    memcpy(OLEDTemp, HaloCubeAnimation[GLOBAL_FRAME_INDICATOR < 51 ? GLOBAL_FRAME_INDICATOR : 50][0], 1024);
    PrintString(GLOBAL_INIT_STATE_INDICATOR, 0, 0);
    updateFromTemp();
    return 0;
}


///main
uint8_t RENDER_MainPage() {
    if (GLOBAL_SELECT_FLAG)
        return 1;
    uint8_t BTST = Battery_Stage;
    memcpy(OLEDTemp, IMG_timeBackground, 1024);
    uint8_t StartPoint[4] = {16, 37, 73, 94};
    uint8_t char2print[4] = {GLOBAL_TIME_INDICATOR.hour / 10 + 26, GLOBAL_TIME_INDICATOR.hour % 10 + 26,
                             GLOBAL_TIME_INDICATOR.min / 10 + 26, GLOBAL_TIME_INDICATOR.min % 10 + 26};
    for (uint8_t i = 0; i < 4; i++) {
        if (char2print[i] == 27)
            StartPoint[i] += 6;
        else
            StartPoint[i] += 3;
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
    icons[1] = ReturnPCStatus() ? ICON16_PC_ON : ICON16_PC_OFF;
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

///Statistic
uint8_t RENDER_StatisticPage() {
    if (GLOBAL_SELECT_FLAG)
        return 1;
    uint8_t i = 0, j = 0;
    memset(OLEDTemp, 0, sizeof(OLEDTemp));
    const char CR[] = "Consumption Ranking";
    PrintString(CR, 0, 0);
    char rankNames[3][8];
    uint32_t rankConsump[3];
    uint8_t maxLen = 0, tmpLen = 0;
    memset(rankNames, 0, sizeof(rankNames));
    memset(rankConsump, 0, sizeof(rankConsump));
    Power_GiveTopThreeConsumption(rankNames, rankConsump);
    for (i = 0; i < 3 && rankConsump[i]; i++) {
        tmpLen = PrintString(rankNames[i], 2, 12 + 14 * i);
        if (tmpLen > maxLen)
            maxLen = tmpLen;
    }
    if (rankConsump[0] == 0)
        goto NO_CONSUMPTION;
    DrawRect(maxLen + 2, 13, 105, 19);
    DrawRect(maxLen + 2, 27, maxLen + 2 + rankConsump[1] * (103 - maxLen) / rankConsump[0], 33);
    DrawRect(maxLen + 2, 46, maxLen + 2 + rankConsump[2] * (103 - maxLen) / rankConsump[0], 52);
    char csp[] = "999";
    for (i = 0; i < 3; i++) {
        if (!rankConsump[i])
            break;
        rankConsump[i] /= 3600;
        if (rankConsump[i] >= 1000) {
            if (rankConsump[i] >= 10000) {
                csp[0] = '0' + rankConsump[i] / 10000;
                csp[1] = '0' + rankConsump[i] / 1000 % 10;
                csp[2] = 'k';
                for (j = 0; j < 3; j++)
                    PrintLetter(csp[j], 108 + j * 7, 12 + 14 * i);

            } else {
                csp[0] = '0' + rankConsump[i] / 1000;
                csp[1] = '0' + rankConsump[i] / 100 % 10;
                csp[2] = 'k';
                for (j = 0; j < 3; j++)
                    PrintLetter(csp[j], 108 + j * 7, 12 + 14 * i);
                SET_HIGH(114, 19 + 14 * i);
            }
        } else {
            csp[0] = '0' + rankConsump[i] / 100;
            csp[1] = '0' + rankConsump[i] / 10 % 10;
            csp[2] = '0' + rankConsump[i] % 10;
            for (j = 0; j < 3; j++)
                PrintLetter(csp[j], 108 + j * 7, 12 + 14 * i);
        }
    }
    NO_CONSUMPTION:
    updateFromTemp();
    return 0;
}

///3days
uint8_t RENDER_3daysPage() {
    if (GLOBAL_SELECT_FLAG)
        return 1;
    memset(OLEDTemp, 0, sizeof(OLEDTemp));
    if (GLOBAL_SELECT_FLAG)
        return 1;
    uint8_t i = 0, j = 0;
    memset(OLEDTemp, 0, sizeof(OLEDTemp));
    const char CR[] = "3Days Consumption Ranking";
    PrintString(CR, 0, 0);
    char rankNames[3][8];
    uint32_t rankConsump[3];
    uint8_t maxLen = 0, tmpLen = 0;
    memset(rankNames, 0, sizeof(rankNames));
    memset(rankConsump, 0, sizeof(rankConsump));
    Power_GiveTopThreeConsumptionOf3days(rankNames, rankConsump);
    for (i = 0; i < 3 && rankConsump[i]; i++) {
        tmpLen = PrintString(rankNames[i], 2, 12 + 14 * i);
        if (tmpLen > maxLen)
            maxLen = tmpLen;
    }
    if (rankConsump[0] == 0)
        goto NO_CONSUMPTION;
    DrawRect(maxLen + 2, 13, 105, 19);
    DrawRect(maxLen + 2, 27, maxLen + 2 + rankConsump[1] * (103 - maxLen) / rankConsump[0], 33);
    DrawRect(maxLen + 2, 46, maxLen + 2 + rankConsump[2] * (103 - maxLen) / rankConsump[0], 52);
    char csp[] = "999";
    for (i = 0; i < 3; i++) {
        if (!rankConsump[i])
            break;
        rankConsump[i] /= 3600;
        if (rankConsump[i] >= 1000) {
            if (rankConsump[i] >= 10000) {
                csp[0] = '0' + rankConsump[i] / 10000;
                csp[1] = '0' + rankConsump[i] / 1000 % 10;
                csp[2] = 'k';
                for (j = 0; j < 3; j++)
                    PrintLetter(csp[j], 108 + j * 7, 12 + 14 * i);

            } else {
                csp[0] = '0' + rankConsump[i] / 1000;
                csp[1] = '0' + rankConsump[i] / 100 % 10;
                csp[2] = 'k';
                for (j = 0; j < 3; j++)
                    PrintLetter(csp[j], 108 + j * 7, 12 + 14 * i);
                SET_HIGH(114, 19 + 14 * i);
            }
        } else {
            csp[0] = '0' + rankConsump[i] / 100;
            csp[1] = '0' + rankConsump[i] / 10 % 10;
            csp[2] = '0' + rankConsump[i] % 10;
            for (j = 0; j < 3; j++)
                PrintLetter(csp[j], 108 + j * 7, 12 + 14 * i);
        }
    }
    NO_CONSUMPTION:
    updateFromTemp();
    return 0;
}

///30days
uint8_t RENDER_30daysPage() {
    if (GLOBAL_SELECT_FLAG)
        return 1;
    uint8_t i;
    memset(OLEDTemp, 0, sizeof(OLEDTemp));
    const char CR[] = "30Days Consumption Curve";
    PrintString(CR, 0, 0);
    uint32_t consumption[30], max = 0;
    Power_Give30DaysConsumption(consumption);
    for (i = 0; i < 30; i++)
        if (consumption[i] > max)
            max = consumption[i];
    for (i = 0; i < 29; i++) {
        SET_HIGH(6 + i * 4, 56 - consumption[i] * 44 / max);
        DrawLine(6 + i * 4, 56 - consumption[i] * 44 / max, 10 + i * 4, 56 - consumption[i + 1] * 44 / max);
    }
    updateFromTemp();
    return 0;
}
