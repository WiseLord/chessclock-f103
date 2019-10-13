#include "layout.h"

static const Layout lt320x240 = {
    .rect.x = 0,
    .rect.y = 0,
    .rect.w = 320,
    .rect.h = 240,

    .chess.gameTotalFont = &fontterminusdig64,
    .chess.gameTimFont = &fontterminusdig64,
    .chess.moveTimFont = &fontterminusdig40,

    .menu.headFont = &fontterminus28b,
    .menu.menuFont = &fontterminus22b,
    .menu.itemCnt = 8,
};

const Layout *layoutGet(void)
{
    return &lt320x240;
}
