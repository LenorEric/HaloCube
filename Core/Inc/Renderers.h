//
// Created by Lenor on 2021/7/25.
//

#ifndef HALOCUBE_DEV_RENDERERS_H
#define HALOCUBE_DEV_RENDERERS_H

#include <stdlib.h>
#include "PageController.h"
#include "HaloCube_OLED.h"
#include "string.h"
#include "GLOBAL_INCLUDE.h"
#include "PageFunction.h"
#include "def_time.h"
#include "OLED_Data.h"
#include "Battery.h"
#include "PowerController.h"

uint8_t openScreenAnimation();
uint8_t RENDER_MainPage();
//uint8_t RENDER_BulbPage();
uint8_t RENDER_SelectingUI();
uint8_t RENDER_TestPage();
uint8_t RENDER_ShortCutPage();
uint8_t RENDER_StatisticPage();
uint8_t RENDER_3daysPage();
uint8_t RENDER_30daysPage();

#endif //HALOCUBE_DEV_RENDERERS_H
