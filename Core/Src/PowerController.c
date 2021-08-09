//
// Created by Lenor on 2021/8/9.
//

#include "PowerController.h"

uint32_t EnergyBulb, EnergyPC, EnergyAC;
uint16_t PowerBulb, PowerPC, PowerAC;
uint8_t PowerStatusFlag; ///Each bit is a flag. From low to high(Bulb PC AC)

void goPowerSec(){
    if (PowerStatusFlag & 1){
        EnergyBulb += PowerBulb;
    }
    if (PowerStatusFlag & 1<<1){
        EnergyPC += PowerPC;
    }
    if (PowerStatusFlag & 1<<2){
        EnergyAC += PowerAC;
    }
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

