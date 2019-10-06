#include "canvas.h"
#include <string.h>

#include "../chess.h"
#include "../menu.h"
#include "../settings.h"
#include "../swtimers.h"
#include "../tr/labels.h"
#include "../utils.h"

typedef union {
    RTC_type rtc;
    struct {
        uint16_t freq;
        int16_t value;
        Icon icon;
        bool stereo;
        bool forcedMono;
        bool rdsFlag;
    } par;
} DrawData;

static DrawData prev;

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

    memset(&prev, 0, sizeof(prev));
}

static void drawMenuItem(uint8_t idx, const tFont *fontItem);

static void drawTmSpacer(char spacer, bool clear)
{
    if (clear) {
        glcdWriteUChar(LETTER_SPACE_CHAR);
        glcdWriteUChar(spacer);
        glcdWriteUChar(LETTER_SPACE_CHAR);
    }
}

static void drawTm(RTC_type *rtc, RtcMode tm, bool clear)
{
    int8_t time = *((int8_t *)rtc + tm);

    if (clear) {
        glcdSetFontColor(canvas.pal->fg);
        glcdSetFontBgColor(canvas.pal->bg);
        glcdWriteUChar(LETTER_SPACE_CHAR);

        if (time >= 0) {
            glcdWriteString(utilMkStr("%02d", time));
        } else {
            glcdWriteString("--");
        }
        glcdWriteUChar(LETTER_SPACE_CHAR);
        glcdSetFontColor(canvas.pal->fg);
        glcdSetFontBgColor(canvas.pal->bg);
    }
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

static void canvasShowTimer(bool clear, ChessColor color)
{
    const Layout *lt = canvas.layout;

    RTC_type rtc;
    int32_t value = chessTimGet(CHESS_TIM_GAME_WHITE + color);

    if (value < 0) {
        rtc.hour = -1;
        rtc.min = -1;
        rtc.sec = -1;
    } else {
        value /= 1000;
        rtc.hour = (int8_t)(value / 3600);
        rtc.min = (int8_t)(value / 60 % 60);
        rtc.sec = (int8_t)(value % 60);
    }

    // HH:MM:SS
    glcdSetFont(lt->time.hmsFont);

    int16_t digW = lt->time.hmsFont->chars[glcdFontSymbolPos('0')].image->width;
    int16_t ltspW = lt->time.hmsFont->chars[glcdFontSymbolPos(LETTER_SPACE_CHAR)].image->width;

    int16_t timeLen = 6 * digW + 15 * ltspW;    // 6 digits HHMMSS + 13 letter spaces + 2 ':'
    int16_t timeX = (lt->rect.w - timeLen) / 2;

    if (color == CHESS_WHITE) {
        glcdSetXY(timeX, lt->time.timeW);
    } else {
        glcdSetXY(timeX, lt->time.timeB);
    }
    drawTm(&rtc, RTC_HOUR, clear);

    drawTmSpacer(':', clear);

    timeX += digW * 2 + ltspW * 6;
    glcdSetX(timeX);
    drawTm(&rtc, RTC_MIN, clear);

    drawTmSpacer(':', clear);

    timeX += digW * 2 + ltspW * 6;
    glcdSetX(timeX);
    drawTm(&rtc, RTC_SEC, clear);

    prev.rtc = rtc;
}

void canvasShowChess(bool clear)
{
    clear = true;
    canvasShowTimer(clear, CHESS_WHITE);
    canvasShowTimer(clear, CHESS_BLACK);
}
