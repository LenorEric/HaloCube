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
uint8_t PLNotEmpty();

RenderFunc RenderListGet();


typedef void (*PageFunc)(void);

typedef struct {
    PageFunc actionFunc[4];
    uint8_t *selectIcon[4];
    RenderFunc selfRender;
} Page;

void PageInit();
void PageAction(uint8_t action);
void GotoPage(uint8_t pageNumber);
Page getPage();


#endif //HALOCUBE_DEV_RENDERERCONTROLLER_H
