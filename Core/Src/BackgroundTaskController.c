//
// Created by Lenor on 2021/8/11.
//

#include "BackgroundTaskController.h"

BackgroundTaskFunc BT_List[256];
uint8_t left = 0, right = 0;

void BT_Push(BackgroundTaskFunc func) {
    if (right+1 == left) {
        printf("!!Fault!!: BT overflow\r\n");
        __set_FAULTMASK(1);
        HAL_NVIC_SystemReset();
        return;
    }
    BT_List[right++] = func;
}


void BT_HandleNextTask() {
    if (left == right){
        return;
    }
    BT_List[left]();
    left++;
}