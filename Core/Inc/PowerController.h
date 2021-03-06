//
// Created by Lenor on 2021/8/9.
//

#ifndef HALOCUBE_POWERCONTROLLER_H
#define HALOCUBE_POWERCONTROLLER_H

#include "GLOBAL_INCLUDE.h"
#include "string.h"
#include "Bulb.h"
#include "PC.h"
#include "def_time.h"

void goPowerSec();
void PowerInit();

void Power_BulbSetStatus(uint8_t status);
void Power_PCSetStatus(uint8_t status);
void Power_ACSetStatus(uint8_t status);
void Power_GiveTopThreeConsumption(char name[3][8], uint32_t consumption[3]);
void Power_GiveTopThreeConsumptionOf3days(char name[3][8], uint32_t consumption[3]);
void Power_Give30DaysConsumption(uint32_t consumption[30]);

#endif //HALOCUBE_POWERCONTROLLER_H
