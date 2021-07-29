//
// Created by Lenor on 2021/7/25.
//

#ifndef HALOCUBE_DEV_FakeMPU6050_H
#define HALOCUBE_DEV_FakeMPU6050_H

#include "GLOBAL_INCLUDE.h"
#include "math.h"

#define FakeMPU6050_ADDRESS 0x68

void MPU6050_init();
void FGetDirection(uint8_t *direction);

#endif //HALOCUBE_DEV_FakeMPU6050_H
