//
// Created by Lenor on 2021/7/25.
//

#ifndef HALOCUBE_DEV_RENDERERS_H
#define HALOCUBE_DEV_RENDERERS_H

#include "RendererController.h"
#include "HaloCube_OLED.h"
#include "string.h"
#include "GLOBAL_INCLUDE.h"
#include "def_time.h"

uint8_t openScreenAnimation();
uint8_t RENDER_mainPage();
uint8_t RENDER_bulbPage();
uint8_t RENDER_selectingUI();

#endif //HALOCUBE_DEV_RENDERERS_H
