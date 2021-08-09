//
// Created by Lenor on 2021/8/9.
//

#ifndef HALOCUBE_BULB_H
#define HALOCUBE_BULB_H

#include "GLOBAL_INCLUDE.h"
#include "nRF24L01.h"
#include "PowerController.h"

uint8_t GetBulbStatus();
uint8_t ReturnBulbStatus();
void TurnOnBulb();
void TurnOffBulb();

#endif //HALOCUBE_BULB_H
