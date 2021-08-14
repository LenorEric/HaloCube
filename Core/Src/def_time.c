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
}

///Function shell
void SHELL_HCB_GetBattery() {
    HCB_GetBattery();
}

void SHELL_getTime() {
    getTime(getTimeStamp());
}

void SHELL_UpdateEEPROM(){
    extern uint32_t EnergyRecord[31][32];
    for (uint8_t i = 0; i < 32; i++) {
        for (uint8_t j = 0; j < 32; j++) {
            if (EnergyRecord[i][j] == 0xFFFFFFFF || i == (DT_TS_TODAY + 1) % 32)
                EnergyRecord[i][j] = 0;
        }
    }
    EEPROM_Save_Data(1, EnergyRecord, sizeof(EnergyRecord));
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
        BT_Push(EEPROM_Data_Save_Task);
        BT_Push(SHELL_HCB_GetBattery);
        BT_Push(OLED_Refresh);
        if (GLOBAL_TIME_INDICATOR.min >= 60) {
            GLOBAL_TIME_INDICATOR.min = 0;
            GLOBAL_TIME_INDICATOR.hour++;
            /// Each hour
            BT_Push(SHELL_getTime);
            if (GLOBAL_TIME_INDICATOR.hour >= 24) {
                GLOBAL_TIME_INDICATOR.hour = 0;
                ///Each day
                BT_Push(SHELL_UpdateEEPROM);
            }
        }
    }
}