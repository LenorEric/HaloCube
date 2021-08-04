//
// Created by Lenor on 2021/8/5.
//

#include "PageFunction.h"

void PF_Restart(){
    __set_FAULTMASK(1);
    HAL_NVIC_SystemReset();
}