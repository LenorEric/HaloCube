//
// Created by Lenor on 2021/8/5.
//

#include "PageFunction.h"

extern Page PageList[31];

void PF_Restart() {
    __set_FAULTMASK(1);
    HAL_NVIC_SystemReset();
}

void PF_AirConditioner() {

}

uint8_t PC_ON = 0;

void PF_PC() {
    if (PC_ON){
        PageList[1].selectIcon[1] = ICON16_PC_ON;
        PC_ON = 0;
    }
    else{
        PageList[1].selectIcon[1] = ICON16_PC_OFF;
        PC_ON = 1;
    }
    GotoPage(0);
}

uint8_t Bulb_ON = 0;

void PF_LightBulb() {
    if (Bulb_ON){
        PageList[1].selectIcon[2] = ICON16_Bulb_ON;
        Bulb_ON = 0;
    }
    else{
        PageList[1].selectIcon[2] = ICON16_Blub_OFF;
        Bulb_ON = 1;
    }
    GotoPage(0);
}
