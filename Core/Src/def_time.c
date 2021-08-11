//
// Created by Lenor on 2021/8/5.
//

#include "def_time.h"

GL_TM GLOBAL_TIME_INDICATOR;

void getTime(uint64_t Net_timeStamp) {
    if (Net_timeStamp)
        GLOBAL_TIME_INDICATOR.timeStampOfSec = (Net_timeStamp >> 32) + 3600 * 8;
    GLOBAL_TIME_INDICATOR.sec = GLOBAL_TIME_INDICATOR.timeStampOfSec % 60;
    GLOBAL_TIME_INDICATOR.min = GLOBAL_TIME_INDICATOR.timeStampOfSec / 60 % 60;
    GLOBAL_TIME_INDICATOR.hour = GLOBAL_TIME_INDICATOR.timeStampOfSec / 3600 % 24;
    /// todo: Date calc
}

///Function shell
void SHELL_HCB_GetBattery(){
    HCB_GetBattery();
}

void SHELL_getTime(){
    getTime(getTimeStamp());
}

void goSec() {
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
                /// todo: Date calc
            }
        }
    }
}