//
// Created by Lenor on 2021/8/11.
//

#include "BackgroundTaskController.h"

BackgroundTaskFunc BT_List[128];
uint8_t left=0, right=0;

void BT_Push(BackgroundTaskFunc func) {
    BT_List[right++] = func;
    right %= 128;
}

void BT_HandleNextTask() {
    if (left == right)
        return;
    BT_List[left]();
    left++;
    left %= 128;
}