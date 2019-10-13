#include "layout.h"

static const Layout lt480x320 = {
    .rect.x = 0,
    .rect.y = 0,
    .rect.w = 480,
    .rect.h = 320,


    .menu.headFont = &fontterminus32b,
    .menu.menuFont = &fontterminus24b,
    .menu.itemCnt = 10,
};

const Layout *layoutGet(void)
{
    return &lt480x320;
}
