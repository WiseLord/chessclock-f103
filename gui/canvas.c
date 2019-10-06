#include "canvas.h"
#include <string.h>

#include "../chess.h"
#include "../menu.h"
#include "../settings.h"
#include "../swtimers.h"
#include "../tr/labels.h"
#include "../utils.h"

typedef union {
    struct {
        char str[12];
    } tim[CHESS_TIM_END];
} DrawData;

static DrawData drawData;

static Canvas canvas;

const Layout *layoutGet(void);

void canvasInit(void)
{
    glcdInit(&canvas.glcd);
    dispdrvSetBrightness(LCD_BR_MAX);

    canvas.layout = layoutGet();

    PalIdx palIdx = (PalIdx)settingsRead(PARAM_DISPLAY_PALETTE);
    paletteSetIndex(palIdx);
    canvas.pal = paletteGet(palIdx);

    bool rotate = settingsRead(PARAM_DISPLAY_ROTATE);
    glcdRotate(rotate);

    canvas.glcd->rect = canvas.layout->rect;

    menuGet()->dispSize = canvas.layout->menu.itemCnt;
}

Canvas *canvasGet(void)
{
    return &canvas;
}

void canvasClear(void)
{
    GlcdRect rect = canvas.glcd->rect;

    glcdDrawRect(0, 0, rect.w, rect.h, canvas.pal->bg);
    glcdShift(0);

    const int16_t ft = canvas.glcd->drv->height / 100;

    if (rect.x >= 2 * ft && rect.y >= 2 * ft) {
        glcdDrawFrame(-ft, -ft, rect.w + 2 * ft, rect.h + 2 * ft, ft, canvas.pal->bg);
        glcdDrawFrame(-2 * ft, -2 * ft, rect.w + 4 * ft, rect.h + 4 * ft, ft, canvas.pal->fg);
    }

    glcdSetFontColor(canvas.pal->fg);
    glcdSetFontBgColor(canvas.pal->bg);

    memset(&drawData, 0, sizeof(drawData));
}

static void drawMenuItem(uint8_t idx, const tFont *fontItem)
{
    const Layout *lt = canvas.layout;

    uint8_t fIh = (uint8_t)fontItem->chars[0].image->height;

    Menu *menu = menuGet();
    uint8_t items = menu->dispSize;

    int16_t width = lt->rect.w;
    MenuIdx menuIdx = menu->list[idx];
    uint8_t active = (menu->active == menu->list[idx]);

    const uint8_t ih = fIh + 4; // Menu item height
    int16_t y_pos = lt->rect.h - ih * (items - idx + menu->dispOft);

    // Draw selection frame
    glcdDrawFrame(0, y_pos, width, ih, 1, active ? canvas.pal->fg : canvas.pal->bg);

    // Draw menu name
    glcdSetFont(fontItem);
    glcdSetFontColor(canvas.pal->fg);

    glcdSetXY(4, y_pos + 2);
    if (menu->list[idx] != MENU_NULL) {
        glcdWriteString("  ");
    } else {
        glcdWriteString("< ");
    }

    glcdWriteString(menuGetName(menuIdx));

    // Draw menu value
    int16_t x = canvas.glcd->x;
    glcdSetXY(width - 2, y_pos + 2);
    glcdSetFontAlign(GLCD_ALIGN_RIGHT);

    // Inverse value color if selected
    uint16_t color = canvas.glcd->fontFg;
    uint16_t bgColor = canvas.glcd->fontBg;
    if (active && menu->selected) {
        glcdSetFontColor(bgColor);
        glcdSetFontBgColor(color);
    }
    glcdSetStringFramed(true);
    uint16_t strLen = glcdWriteString(menuGetValueStr(menuIdx));
    glcdSetStringFramed(false);

    glcdSetFontColor(color);
    glcdSetFontBgColor(bgColor);

    // Fill space between name and value
    glcdDrawRect(x, y_pos + 2, width - 2 - x - strLen, fIh, canvas.pal->bg);
}

void canvasShowMenu(bool clear)
{
    (void)clear;

    const Layout *lt = canvas.layout;

    Menu *menu = menuGet();

    const int16_t fHh = (int16_t)lt->menu.headFont->chars[0].image->height;
    const int16_t fIh = (int16_t)lt->menu.menuFont->chars[0].image->height;
    const uint8_t items = menu->dispSize;

    const int16_t dividerPos = (lt->rect.h - (fIh + 4) * items + fHh) / 2;

    // Show header
    const char *parentName = menuGetName(menu->parent);
    glcdSetFont(lt->menu.headFont);
    glcdSetFontColor(canvas.pal->fg);

    glcdSetXY(2, 0);
    glcdWriteString(parentName);
    // Fill free space after header
    glcdDrawRect(canvas.glcd->x, canvas.glcd->y, lt->rect.w - canvas.glcd->x, fHh, canvas.pal->bg);

    glcdDrawRect(0, dividerPos, lt->rect.w, 1, canvas.glcd->fontFg);

    for (uint8_t idx = 0; idx < menu->listSize; idx++) {
        if (idx >= menu->dispOft && idx < items + menu->dispOft) {
            drawMenuItem(idx, lt->menu.menuFont);
        }
    }
}

static void timToStr(int32_t time, char *str)
{
    char *buf;

    int16_t msec = time % 1000;
    time /= 1000;
    int16_t sec = time % 60;
    time /= 60;
    int16_t min = time % 60;
    time /= 60;
    int16_t hour = time % 24;

    if (hour > 0) {
        buf = utilMkStr("%02d:%02d:%02d", hour, min, sec);
    } else {
        buf = utilMkStr("%02d:%02d:%02d", min, sec, msec / 10);
    }

    strcpy(str, buf);
}

static void drawGameTime(bool clear, ChessTimer tim)
{
    (void)clear;

    const Layout *lt = canvas.layout;

    int32_t gameTim = chessTimGet(tim);
    timToStr(gameTim, drawData.tim[tim].str);
    glcdSetFont(lt->chess.gameTimFont);
    glcdWriteString(drawData.tim[tim].str);
}

void canvasShowChess(bool clear)
{
    const Layout *lt = canvas.layout;

    glcdSetXY(0, lt->rect.h / 2);
    drawGameTime(clear, CHESS_TIM_GAME_WHITE);

    glcdSetXY(lt->rect.w / 2, lt->rect.h / 2);
    drawGameTime(clear, CHESS_TIM_GAME_BLACK);
}
