//
// Created by Lenor on 2021/8/9.
//

#include "PowerController.h"

uint32_t EnergyRecord[30][32];
#define EnergyBulb EnergyRecord[DT_TS_TODAY][0]
#define EnergyPC EnergyRecord[DT_TS_TODAY][1]
#define EnergyAC EnergyRecord[DT_TS_TODAY][2]
#define EnergyFridge EnergyRecord[DT_TS_TODAY][3]
#define ApplianceNumber 4
uint16_t PowerBulb = 18, PowerPC = 50, PowerAC=700, PowerFridge = 25;
char appName[][8] = {"bulb", "pc", "ac","fridge"};
uint32_t PowerStatusFlag = 0; ///Each bit is a flag. From low to high(Bulb PC AC)

void goPowerSec() {
    if (PowerStatusFlag & 1) {
        EnergyBulb += PowerBulb;
    }
    if (PowerStatusFlag & 1 << 1) {
        EnergyPC += PowerPC;
    }
    if (PowerStatusFlag & 1 << 2) {
        EnergyAC += PowerAC;
    }
    EnergyFridge += PowerFridge;
}

void PowerInit(){
    memset(EnergyRecord,0,sizeof(EnergyRecord));
    GetBulbStatus();
    GetPCStatus();
}

void Power_BulbSetStatus(uint8_t status) {
    if (status)
        PowerStatusFlag |= 1;
    else
        PowerStatusFlag &= ~(1);
}

void Power_PCSetStatus(uint8_t status) {
    if (status)
        PowerStatusFlag |= 1 << 1;
    else
        PowerStatusFlag &= ~(1 << 1);
}

void Power_ACSetStatus(uint8_t status) {
    if (status)
        PowerStatusFlag |= 1 << 2;
    else
        PowerStatusFlag &= ~(1 << 2);
}

void Power_GiveTopThreeConsumption(char name[3][8], uint32_t consumption[3]) {
    uint32_t energy[] = {EnergyBulb, EnergyPC, EnergyAC,EnergyFridge};
    uint32_t tempe = 0;
    char tempn[8];
    for (uint8_t i = 0; i < sizeof(energy)/sizeof(uint32_t); i++) {
        for (uint8_t j = 2; j != 255; j--) {
            if (energy[i] >= consumption[j]) {
                if (j == 2) {
                    strcpy(name[j], appName[i]);
                    consumption[j] = energy[i];
                } else {
                    tempe = consumption[j + 1];
                    strcpy(tempn, name[j+1]);
                    consumption[j + 1] = consumption[j];
                    strcpy(name[j + 1],name[j]);
                    consumption[j] = tempe;
                    strcpy(name[j], tempn);
                }
            }
        }
    }
}

void Power_GiveTopThreeConsumptionOf7days(char name[3][8], uint32_t consumption[3]) {
    uint32_t energy[ApplianceNumber];
    memset(energy, 0, sizeof(energy));
    for (uint8_t app=0; app < ApplianceNumber; app++){
        for (uint8_t day=DT_TS_TODAY;day>(DT_TS_TODAY+23)%30;day=(day+29)%30){
            energy[app] += EnergyRecord[day][app];
        }
    }
    uint32_t tempe = 0;
    char tempn[8];
    for (uint8_t i = 0; i < sizeof(energy)/sizeof(uint32_t); i++) {
        for (uint8_t j = 2; j != 255; j--) {
            if (energy[i] >= consumption[j]) {
                if (j == 2) {
                    strcpy(name[j], appName[i]);
                    consumption[j] = energy[i];
                } else {
                    tempe = consumption[j + 1];
                    strcpy(tempn, name[j+1]);
                    consumption[j + 1] = consumption[j];
                    strcpy(name[j + 1],name[j]);
                    consumption[j] = tempe;
                    strcpy(name[j], tempn);
                }
            }
        }
    }
}


