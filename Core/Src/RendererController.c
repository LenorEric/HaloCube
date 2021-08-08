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

uint8_t PLNotEmpty(){
    return RList.right - RList.left;
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
    ///Main Page 0
    PageList[pageNum].actionType = 0;
    for (int i = 0; i < 4; i++) {
        PageList[pageNum].actionPage[i] = pageNum;
        PageList[pageNum].selectIcon[i] = ICON16_Exit;
    }
    PageList[pageNum].actionPage[0] = 1;
    PageList[pageNum].selectIcon[0] = ICON16_ShortCut;
    PageList[pageNum].selectIcon[1] = ICON16_Statistics;
    PageList[pageNum].selectIcon[2] = ICON16_Setting;
    PageList[pageNum].selfRender = RENDER_MainPage;
    pageNum++;

    ///ShortCut 1
    PageList[pageNum].actionType = 0x07;
    PageList[pageNum].actionFunc[0] = PF_AirConditioner;
    PageList[pageNum].selectIcon[0] = ICON16_Exit;
    PageList[pageNum].actionFunc[1] = PF_PC;
    PageList[pageNum].selectIcon[1] = ICON16_PC_ON;
    PageList[pageNum].actionFunc[2] = PF_LightBulb;
    PageList[pageNum].selectIcon[2] = ICON16_Bulb_ON;
    PageList[pageNum].actionPage[3] = 0;
    PageList[pageNum].selectIcon[3] = ICON16_Back;
    PageList[pageNum].selfRender = RENDER_ShortCutPage;
    pageNum++;

    ///

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

void GotoPage(uint8_t pageNumber){
    currentPage = pageNumber;
    while (PLNotEmpty())
        RenderListPop();
    RenderListPush(PageList[pageNumber].selfRender);
}

Page getPage() {
    return PageList[currentPage];
}
