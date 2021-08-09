//
// Created by Lenor on 2021/8/10.
//

#ifndef HALOCUBE_PC_H
#define HALOCUBE_PC_H

#include "GLOBAL_INCLUDE.h"
#include "nRF24L01.h"
#include "PowerController.h"

uint8_t GetPCStatus();
uint8_t ReturnPCStatus();
void TurnOnPC();
void TurnOffPC();

#endif //HALOCUBE_PC_H
