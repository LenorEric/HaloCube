//
// Created by Lenor on 2021/8/9.
//

#include "PowerController.h"

uint32_t EnergyBulb, EnergyPC, EnergyAC;
uint16_t PowerBulb = 14, PowerPC = 100, PowerAC=700;
uint8_t PowerStatusFlag = 0; ///Each bit is a flag. From low to high(Bulb PC AC)

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
}

void PowerInit(){
    GetBulbStatus();
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
    uint32_t energy[] = {EnergyBulb, EnergyPC, EnergyAC};
    char appName[][8] = {"bulb", "pc", "ac"};
    uint32_t tempe = 0;
    char tempn[8];
    for (uint8_t i = 0; i < 3; i++) {
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
                    consumption[j + 1] = tempe;
                    strcpy(name[j], tempn);
                }
            }
        }
    }
}

