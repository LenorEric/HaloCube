//
// Created by Lenor on 2021/7/24.
//

#include "PageController.h"


RenderList RList;

extern uint16_t GLOBAL_FRAME_INDICATOR;

void RenderListInit() {
    RList.left = 0;
    RList.right = 0;
    GLOBAL_FRAME_INDICATOR = 0;
}

uint8_t PLNotEmpty() {
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


Page PageList[32];
uint8_t pageNum = 0, currentPage = 0;

void newPage() {
    ///Main Page 0
    for (int i = 0; i < 4; i++) {
        PageList[pageNum].actionFunc[i] = PF_GotoPage0;
        PageList[pageNum].selectIcon[i] = ICON16_Back;
    }
    PageList[pageNum].actionFunc[0] = PF_GotoPage1;
    PageList[pageNum].selectIcon[0] = ICON16_ShortCut;
    PageList[pageNum].actionFunc[1] = PF_GotoPage2;
    PageList[pageNum].selectIcon[1] = ICON16_Statistics;
    PageList[pageNum].actionFunc[2] = PF_GotoPage0;
    PageList[pageNum].selectIcon[2] = ICON16_Setting;
    PageList[pageNum].actionFunc[3] = PF_GotoPage0;
    PageList[pageNum].selectIcon[3] = ICON16_Back;
    PageList[pageNum].selfRender = RENDER_MainPage;
    pageNum++;

    ///ShortCut 1
    PageList[pageNum].actionFunc[0] = PF_AirConditioner;
    PageList[pageNum].selectIcon[0] = ICON16_Back;
    PageList[pageNum].actionFunc[1] = PF_PC;
    PageList[pageNum].selectIcon[1] = ICON16_PC_ON;
    PageList[pageNum].actionFunc[2] = PF_LightBulb;
    PageList[pageNum].selectIcon[2] = ICON16_Bulb_ON;
    PageList[pageNum].actionFunc[3] = PF_GotoPage0;
    PageList[pageNum].selectIcon[3] = ICON16_Return;
    PageList[pageNum].selfRender = RENDER_ShortCutPage;
    pageNum++;

    ///Statistics 2
    PageList[pageNum].actionFunc[0] = PF_GotoPage4;
    PageList[pageNum].selectIcon[0] = ICON16_30;
    PageList[pageNum].actionFunc[1] = PF_GotoPage3;
    PageList[pageNum].selectIcon[1] = ICON16_3d;
    PageList[pageNum].actionFunc[2] = PF_GotoPage2;
    PageList[pageNum].selectIcon[2] = ICON16_Back;
    PageList[pageNum].actionFunc[3] = PF_GotoPage0;
    PageList[pageNum].selectIcon[3] = ICON16_Return;
    PageList[pageNum].selfRender = RENDER_StatisticPage;
    pageNum++;

    ///Statistics - 3days 3
    PageList[pageNum].actionFunc[0] = PF_GotoPage2;
    PageList[pageNum].selectIcon[0] = ICON16_Back;
    PageList[pageNum].actionFunc[1] = PF_GotoPage2;
    PageList[pageNum].selectIcon[1] = ICON16_Back;
    PageList[pageNum].actionFunc[2] = PF_GotoPage2;
    PageList[pageNum].selectIcon[2] = ICON16_Back;
    PageList[pageNum].actionFunc[3] = PF_GotoPage0;
    PageList[pageNum].selectIcon[3] = ICON16_Return;
    PageList[pageNum].selfRender = RENDER_3daysPage;
    pageNum++;

    ///Statistics - 30days 4
    PageList[pageNum].actionFunc[0] = PF_GotoPage2;
    PageList[pageNum].selectIcon[0] = ICON16_Back;
    PageList[pageNum].actionFunc[1] = PF_GotoPage2;
    PageList[pageNum].selectIcon[1] = ICON16_Back;
    PageList[pageNum].actionFunc[2] = PF_GotoPage2;
    PageList[pageNum].selectIcon[2] = ICON16_Back;
    PageList[pageNum].actionFunc[3] = PF_GotoPage0;
    PageList[pageNum].selectIcon[3] = ICON16_Return;
    PageList[pageNum].selfRender = RENDER_30daysPage;
    pageNum++;
    // todo: Add more pages
}

void PageInit() {
    newPage();
}

void PageAction(uint8_t action) {
    /// todo: More action types
    PageList[currentPage].actionFunc[action]();
}

void GotoPage(uint8_t pageNumber) {
    currentPage = pageNumber;
    while (PLNotEmpty())
        RenderListPop();
    RenderListPush(PageList[pageNumber].selfRender);
}

Page getPage() {
    return PageList[currentPage];
}
