//
// Created by Lenor on 2021/8/5.
//

#include "def_time.h"

GL_TM GLOBAL_TIME_INDICATOR;

void getTime(uint64_t Net_timeStamp) {
    Net_timeStamp >>= 32;
    Net_timeStamp += 3600 * 8; /// Convert timezone
    GLOBAL_TIME_INDICATOR.sec = Net_timeStamp % 60;
    GLOBAL_TIME_INDICATOR.min = Net_timeStamp / 60 % 60;
    GLOBAL_TIME_INDICATOR.hour = Net_timeStamp / 3600 % 24;
    ///todo
}

void goSec() {
    GLOBAL_TIME_INDICATOR.sec++;
    if (GLOBAL_TIME_INDICATOR.sec == 60) {
        GLOBAL_TIME_INDICATOR.sec = 0;
        GLOBAL_TIME_INDICATOR.min++;
        if (GLOBAL_TIME_INDICATOR.min == 60) {
            GLOBAL_TIME_INDICATOR.min = 0;
            GLOBAL_TIME_INDICATOR.hour++;
            getTime(getTimeStamp());
            if (GLOBAL_TIME_INDICATOR.hour == 24) {
                GLOBAL_TIME_INDICATOR.hour = 0;
                /// todo
            }
        }
    }
}