//
// Created by Lenor on 2021/7/24.
//

#ifndef HALOCUBE_DEV_RENDERERCONTROLLER_H
#define HALOCUBE_DEV_RENDERERCONTROLLER_H


#include "OLED_Data.h"
#include "Renderers.h"
#include "PageFunction.h"
#include "GLOBAL_INCLUDE.h"

typedef uint8_t (*RenderFunc)(void);

typedef struct {
    RenderFunc RenderSequence[31];
    int left, right;
} RenderList;

void RenderListInit();

void RenderListPush(RenderFunc);

void RenderListPop();

RenderFunc RenderListGet();


typedef void (*PageFunc)(void);

typedef struct {
    uint8_t actionType; // 1 for function, 0 for page
    PageFunc actionFunc[4];
    uint8_t actionPage[4];
    uint8_t *selectIcon[4];
    RenderFunc selfRender;
} Page;

void PageInit();
void PageAction(uint8_t action);
Page getPage();


#endif //HALOCUBE_DEV_RENDERERCONTROLLER_H
