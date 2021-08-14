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
    PF_GotoPage0();
}

void PF_PC() {
    if (ReturnPCStatus()){
        PageList[1].selectIcon[1] = ICON16_PC_ON;
        TurnOffPC();
    }
    else{
        PageList[1].selectIcon[1] = ICON16_PC_OFF;
        TurnOnPC();
    }
    GotoPage(0);
}

void PF_LightBulb() {
    if (ReturnBulbStatus()){
        PageList[1].selectIcon[2] = ICON16_Bulb_ON;
        TurnOffBulb();
    }
    else{
        PageList[1].selectIcon[2] = ICON16_Blub_OFF;
        TurnOnBulb();
    }
    GotoPage(0);
}

void PF_GotoPage0(){
    GotoPage(0);
}

void PF_GotoPage1() {
    GotoPage(1);
}

void PF_GotoPage2() {
    GotoPage(2);
}

void PF_GotoPage3() {
    GotoPage(3);
}

void PF_GotoPage4() {
    GotoPage(3);
}