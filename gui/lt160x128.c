#include "layout.h"

static const Layout lt160x128 = {
    .rect.x = 0,
    .rect.y = 0,
    .rect.w = 160,
    .rect.h = 128,


    .menu.headFont = &fontterminus14b,
    .menu.menuFont = &fontterminus12,
    .menu.itemCnt = 7,
};

const Layout *layoutGet(void)
{
    return &lt160x128;
}
