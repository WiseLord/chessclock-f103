#include "screen.h"

#include "gui/canvas.h"
#include "menu.h"
#include "settings.h"
#include "swtimers.h"
#include "tr/labels.h"

static bool scrToClear = false;

static Screen screen = {
    .mode = SCREEN_MENU,
    .def = SCREEN_MENU,
};

static bool screenCheckClear(void)
{
    bool clear = false;

    static ScreenMode scrPrev = SCREEN_END;

    if (scrToClear) {
        clear = true;
        scrToClear = false;
    } else if (screen.mode != scrPrev) {
        clear = true;
        switch (screen.mode) {
        case SCREEN_MENU:
            menuGet()->selected = 0;
            break;
        default:
            break;
        }
    }

    // Save current screen and screen parameter
    scrPrev = screen.mode;

    return clear;
}

void screenInit(void)
{
    labelsInit();
    canvasInit();
}

Screen *screenGet(void)
{
    return &screen;
}

void screenSetMode(ScreenMode value)
{
    screen.mode = value;
}

ScreenMode screenGetMode()
{
    return screen.mode;
}

void screenSetBrightness(int8_t value)
{
    screen.brightness = value;
}

void screenToClear(void)
{
    scrToClear = true;
}

void screenShow(bool clear)
{
    GlcdRect rect = canvasGet()->layout->rect;

    glcdSetRect(&rect);

    if (!clear) {
        clear = screenCheckClear();
    }

    if (swTimGet(SW_TIM_INPUT_POLL) == 0) {
        swTimSet(SW_TIM_INPUT_POLL, 100);
    }

    if (clear) {
        canvasClear();
    }

    switch (screen.mode) {
    case SCREEN_MENU:
        canvasShowMenu(clear);
        break;
    case SCREEN_CHESS:
        canvasShowChess(clear);
        break;
    default:
        break;
    }
}

void screenPwm(void)
{
    static int8_t br;

    if (++br >= LCD_BR_MAX) {
        br = 0;
    }

    if (br == screen.brightness) {
        pinsSetBckl(false);
    } else if (br == 0) {
        pinsSetBckl(true);
    }
}
