//
// Created by Lenor on 2021/8/5.
//

#ifndef HALOCUBE_PAGEFUNCTION_H
#define HALOCUBE_PAGEFUNCTION_H

#include "GLOBAL_INCLUDE.h"
#include "RendererController.h"
#include "Bulb.h"

void PF_Restart();
void PF_AirConditioner();
void PF_PC();
void PF_LightBulb();

extern uint8_t GLOBAL_PC_ON_FLAG;

#endif //HALOCUBE_PAGEFUNCTION_H
