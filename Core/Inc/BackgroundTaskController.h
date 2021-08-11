//
// Created by Lenor on 2021/8/11.
//

#ifndef HALOCUBE_BACKGROUNDTASKCONTROLLER_H
#define HALOCUBE_BACKGROUNDTASKCONTROLLER_H

#include "GLOBAL_INCLUDE.h"

typedef void (*BackgroundTaskFunc)(void);

void BT_Push(BackgroundTaskFunc func);

void BT_HandleNextTask();

#endif //HALOCUBE_BACKGROUNDTASKCONTROLLER_H
