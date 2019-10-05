#include "layout.h"

static const Layout lt320x240 = {
    .rect.x = 0,
    .rect.y = 0,
    .rect.w = 320,
    .rect.h = 240,

    .time.hmsFont = &fontterminusdig80,
    .time.dmyFont = &fontterminusdig64,
    .time.wdFont = &fontterminusmod18x3,
    .time.timeW = 10,
    .time.timeB = 130,

    .menu.headFont = &fontterminus28b,
    .menu.menuFont = &fontterminus22b,
    .menu.itemCnt = 8,
};

const Layout *layoutGet(void)
{
    return &lt320x240;
}
