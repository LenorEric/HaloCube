  //
// Created by Lenor on 2021/7/24.
//

#include "RendererController.h"


RenderList RList;

extern uint16_t GLOBAL_FRAME_INDICATOR;

void RenderListInit() {
    RList.left = 0;
    RList.right = 0;
    GLOBAL_FRAME_INDICATOR = 0;
}

void RenderListPush(RenderFunc func) {
    RList.RenderSequence[RList.right++] = func;
    RList.right %= 31;
}

void doNothing() {}

void RenderListPop() {
    if (RList.right == RList.left)
        return;
    RList.left++;
    RList.left %= 31;
    GLOBAL_FRAME_INDICATOR = 0;
}

RenderFunc RenderListGet() {
    if (RList.right == RList.left)
        return doNothing;
    return RList.RenderSequence[RList.left];
}


Page PageList[31];
uint8_t pageNum = 0, currentPage = 0;

void newPage() {
    PageList[pageNum].actionType = 0;
    for (int i = 0; i < 4; i++){
        PageList[pageNum].actionPage[i] = 0;
        PageList[pageNum].selectIcon[i] = ICON12_Exit;
    }
    PageList[pageNum].selfRender = RENDER_mainPage;
    pageNum++;
    // todo
}

void PageInit() {
    newPage();
}

void PageAction(uint8_t action) {
    /*todo*/
    if (PageList[currentPage].actionType >> action & 1) {
        PageList[currentPage].actionFunc[action]();
    } else {
        currentPage = PageList[currentPage].actionPage[action];
    }
    RenderListPush(PageList[currentPage].selfRender);
}

Page getPage() {
    return PageList[currentPage];
}