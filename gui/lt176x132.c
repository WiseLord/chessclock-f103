#include "layout.h"

static const Layout lt176x132 = {
    .rect.x = 0,
    .rect.y = 0,
    .rect.w = 176,
    .rect.h = 132,

    .time.hmsFont = &fontterminusdig40,
    .time.dmyFont = &fontterminusdig30,
    .time.wdFont = &fontterminus32,
    .time.timeW = 6,
    .time.timeB = 138,

    .menu.headFont = &fontterminus16b,
    .menu.menuFont = &fontterminus12,
    .menu.itemCnt = 7,
};

const Layout *layoutGet(void)
{
    return &lt176x132;
}
