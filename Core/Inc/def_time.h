//
// Created by Lenor on 2021/8/5.
//

#ifndef HALOCUBE_DEF_TIME_H
#define HALOCUBE_DEF_TIME_H


#include "GLOBAL_INCLUDE.h"
#include "ESP8266_WiFi.h"
#include "Battery.h"
#include "HaloCube_OLED.h"
#include "PowerController.h"
#include "EEPROM.h"
#include "BackgroundTaskController.h"
#include "time.h"

#define DT_TS_TODAY GLOBAL_TIME_INDICATOR.timestampOfSec/86400%30

typedef struct {
    uint16_t year;
    uint8_t mon;
    uint8_t day;
    uint8_t w_day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint32_t timestampOfSec;
} GL_TM;

extern GL_TM GLOBAL_TIME_INDICATOR;

void getTime(uint64_t Net_timeStamp);
void goSec();

#endif //HALOCUBE_DEF_TIME_H
