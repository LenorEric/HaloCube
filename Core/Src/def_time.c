//
// Created by Lenor on 2021/8/5.
//

#include "def_time.h"

GL_TM GLOBAL_TIME_INDICATOR;

void getTime(uint64_t Net_timeStamp) {
    if (Net_timeStamp){
        GLOBAL_TIME_INDICATOR.timestampOfSec = (Net_timeStamp >> 32) + 3600 * 8;
    }
    Net_timeStamp = (uint64_t)GLOBAL_TIME_INDICATOR.timestampOfSec - 2208988800;
    struct tm *time = localtime(&Net_timeStamp);
    GLOBAL_TIME_INDICATOR.sec = GLOBAL_TIME_INDICATOR.timestampOfSec % 60;
    GLOBAL_TIME_INDICATOR.min = GLOBAL_TIME_INDICATOR.timestampOfSec / 60 % 60;
    GLOBAL_TIME_INDICATOR.hour = GLOBAL_TIME_INDICATOR.timestampOfSec / 3600 % 24;
    GLOBAL_TIME_INDICATOR.day = time->tm_mday;
    GLOBAL_TIME_INDICATOR.w_day = time->tm_wday;
    if (!GLOBAL_TIME_INDICATOR.w_day) GLOBAL_TIME_INDICATOR.w_day=7;
    GLOBAL_TIME_INDICATOR.mon = time->tm_mon +1;
    GLOBAL_TIME_INDICATOR.year = time->tm_year + 1900;
    /// todo: Date calc
}

///Function shell
void SHELL_HCB_GetBattery() {
    HCB_GetBattery();
}

void SHELL_getTime() {
    getTime(getTimeStamp());
}

void goSec() {
    GLOBAL_TIME_INDICATOR.timestampOfSec++;
    GLOBAL_TIME_INDICATOR.sec++;
    /// Each sec
    BT_Push(goPowerSec);
    if (GLOBAL_TIME_INDICATOR.sec >= 60) {
        GLOBAL_TIME_INDICATOR.sec = 0;
        GLOBAL_TIME_INDICATOR.min++;
        /// Each min
        BT_Push(SHELL_HCB_GetBattery);
        BT_Push(OLED_Refresh);
        BT_Push(EEPROM_Data_Save_Task);
        if (GLOBAL_TIME_INDICATOR.min >= 60) {
            GLOBAL_TIME_INDICATOR.min = 0;
            GLOBAL_TIME_INDICATOR.hour++;
            /// Each hour
            BT_Push(SHELL_getTime);
            if (GLOBAL_TIME_INDICATOR.hour >= 24) {
                GLOBAL_TIME_INDICATOR.hour = 0;
                ///Each day
                /// todo: Date calc
            }
        }
    }
}