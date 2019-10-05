#include "layout.h"

static const Layout lt220x176 = {
    .rect.x = 0,
    .rect.y = 0,
    .rect.w = 220,
    .rect.h = 176,

    .time.hmsFont = &fontterminusdig58,
    .time.dmyFont = &fontterminusdig40,
    .time.wdFont = &fontterminus32,
    .time.timeW = 10,
    .time.timeB = 98,

    .menu.headFont = &fontterminus20b,
    .menu.menuFont = &fontterminus18,
    .menu.itemCnt = 7,
};

const Layout *layoutGet(void)
{
    return &lt220x176;
}
