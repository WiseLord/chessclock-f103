#include "layout.h"

static const Layout lt400x240 = {
    .rect.x = 0,
    .rect.y = 0,
    .rect.w = 400,
    .rect.h = 240,

    .chess.gameTotalFont = &fontterminusdig64,
    .chess.gameTimFont = &fontterminusdig80,
    .chess.moveTimFont = &fontterminusdig40,

    .menu.headFont = &fontterminus28b,
    .menu.menuFont = &fontterminus22b,
    .menu.itemCnt = 8,
};

const Layout *layoutGet(void)
{
    return &lt400x240;
}
