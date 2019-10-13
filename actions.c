#include "actions.h"

#include <stddef.h>

#include "chess.h"
#include "gui/canvas.h"
#include "input.h"
#include "menu.h"
#include "pins.h"
#include "settings.h"
#include "swtimers.h"
#include "tr/labels.h"

static void actionGetButtons(void);
static void actionGetEncoder(void);
static void actionGetTimers(void);
static void actionRemapBtnShort(void);
static void actionRemapBtnLong(void);
static void actionRemapBtnInstant(void);
static void actionRemapCommon(void);
static void actionRemapEncoder(void);

static Action action = {ACTION_NONE, false, FLAG_ENTER, SCREEN_CHESS, 0, ACTION_NONE};

static void actionSet(ActionType type, int16_t value)
{
    action.type = type;
    action.value = value;
}

static void actionSetScreen(ScreenMode screen, int16_t timeout)
{
    action.screen = screen;
    action.timeout = timeout;
}

static void actionDispExpired(ScreenMode scrMode)
{
    Screen *screen = screenGet();

    screen->iconHint = ICON_EMPTY;

    actionSetScreen(SCREEN_CHESS, 1000); // TODO: Return to parent screen caused menu
}

static void actionGetButtons(void)
{
    CmdBtn cmdBtn = getBtnCmd();

    if (cmdBtn & 0x00FF0000) {
        actionSet(ACTION_BTN_INSTANT, (cmdBtn >> 16) & 0xFF);
    } else if (cmdBtn & 0x0000FF00) {
        actionSet(ACTION_BTN_LONG, (cmdBtn >> 8) & 0xFF);
    } else if (cmdBtn & 0x000000FF) {
        actionSet(ACTION_BTN_SHORT, cmdBtn & 0xFF);
    }
}

static void actionGetEncoder(void)
{
    int8_t encVal = getEncoder();

    if (encVal) {
        actionSet(ACTION_ENCODER, encVal);
    }
}

static void actionGetTimers(void)
{
    if (swTimGet(SW_TIM_DISPLAY) == 0) {
        actionSet(ACTION_DISP_EXPIRED, 0);
    }
}

static void actionRemapBtnShort(void)
{
    Screen *screen = screenGet();
    ScreenMode scrMode = screen->mode;

    switch (action.value) {
    case BTN_D0:
        if (scrMode == SCREEN_MENU) {
            actionSet(ACTION_NAVIGATE, CMD_NAV_UP);
        }
        break;
    case BTN_D1:
        if (scrMode == SCREEN_MENU) {
            actionSet(ACTION_NAVIGATE, CMD_NAV_DOWN);
        }
        break;
    case BTN_D2:
        break;
    case BTN_D3:
        break;
    case BTN_D4:
        break;
    case BTN_D5:
        if (scrMode == SCREEN_MENU) {
            actionSet(ACTION_NAVIGATE, CMD_NAV_OK);
        } else {
            if (chessIsRunning()) {
                chessSetMove(CHESS_INACTIVE);
            } else {
                chessInit();
            }
        }
        break;
    case ENC_A:
        actionSet(ACTION_ENCODER, -1);
        break;
    case ENC_B:
        actionSet(ACTION_ENCODER, +1);
        break;
    default:
        break;
    }
}

static void actionRemapBtnLong(void)
{
    Screen *screen = screenGet();
    ScreenMode scrMode = screen->mode;

    switch (action.value) {
    case BTN_D0:
        break;
    case BTN_D1:
        break;
    case BTN_D2:
        break;
    case BTN_D3:
        break;
    case BTN_D4:
        break;
    case BTN_D5:
        if (scrMode == SCREEN_MENU) {
            actionDispExpired(scrMode);
            chessInit();
        } else {
            actionSet(ACTION_MENU_SELECT, MENU_SETUP_CHESS);
        }
        break;
    case ENC_A:
        actionSet(ACTION_ENCODER, -1);
        break;
    case ENC_B:
        actionSet(ACTION_ENCODER, +1);
        break;
    default:
        break;
    }
}

static void actionRemapBtnInstant(void)
{
    Screen *screen = screenGet();
    ScreenMode scrMode = screen->mode;

    switch (action.value) {
    case BTN_D0:
        if (scrMode == SCREEN_CHESS) {
            chessSetMove(CHESS_LEFT);
        }
        break;
    case BTN_D1:
        if (scrMode == SCREEN_CHESS) {
            chessSetMove(CHESS_RIGHT);
        }
        break;
    case BTN_D2:
        break;
    case BTN_D3:
        break;
    case BTN_D4:
        break;
    case BTN_D5:
        break;
    case ENC_A:
        break;
    case ENC_B:
        break;
    default:
        break;
    }
}

static void actionNavigateMenu(RcCmd cmd)
{
    Menu *menu = menuGet();

    switch (cmd) {
    case CMD_NAV_OK:
    case CMD_NAV_RIGHT:
        actionSet(ACTION_MENU_SELECT, (int16_t)(menuGetFirstChild()));
        break;
    case CMD_NAV_BACK:
    case CMD_NAV_LEFT:
        if (menu->selected) {
            menu->selected = false;
        } else if (menuIsTop()) {
            // TODO: Return to original screen called menu
//            actionSet(ACTION_STANDBY, FLAG_ENTER);
        } else {
            actionSet(ACTION_MENU_SELECT, (int16_t)menu->parent);
        }
        break;
    case CMD_NAV_UP:
        actionSet(ACTION_MENU_CHANGE, -1);
        break;
    case CMD_NAV_DOWN:
        actionSet(ACTION_MENU_CHANGE, +1);
        break;
    }
}

static void actionNavigateCommon(RcCmd cmd)
{
    switch (cmd) {
    case CMD_NAV_OK:
        break;
    case CMD_NAV_BACK:
        break;
    case CMD_NAV_RIGHT:
        break;
    case CMD_NAV_LEFT:
        break;
    case CMD_NAV_UP:
        break;
    case CMD_NAV_DOWN:
        break;
    }
}

static void actionRemapNavigate(void)
{
    ScreenMode screen = screenGetMode();

    switch (screen) {
    case SCREEN_MENU:
        actionNavigateMenu((RcCmd)action.value);
        break;
    default:
        actionNavigateCommon((RcCmd)action.value);
        break;
    }
}

static void actionRemapEncoder(void)
{
    ScreenMode screen = screenGetMode();

    int16_t encCnt = action.value;

    switch (screen) {
    case SCREEN_MENU:
        actionSet(ACTION_MENU_CHANGE, encCnt);
        break;
    default:

        break;
    }
}

static void actionRemapCommon(void)
{
    ScreenMode screen = screenGetMode();

    switch (action.type) {
    case ACTION_OPEN_MENU:
        switch (screen) {
        case SCREEN_MENU:
//            actionSet(ACTION_NAVIGATE, CMD_NAV_OK);
            break;
        }
        break;
        break;
    default:
        break;
    }
}

void actionUserGet(void)
{
    actionSet(ACTION_NONE, 0);

    if (ACTION_NONE == action.type) {
        actionGetButtons();
    }

    if (ACTION_NONE == action.type) {
        actionGetEncoder();
    }

    if (ACTION_NONE == action.type) {
        actionGetTimers();
    }

    switch (action.type) {
    case ACTION_BTN_SHORT:
        actionRemapBtnShort();
        break;
    case ACTION_BTN_LONG:
        actionRemapBtnLong();
        break;
    case ACTION_BTN_INSTANT:
        actionRemapBtnInstant();
        break;
    default:
        break;
    }

    actionRemapCommon();

    if (ACTION_NAVIGATE == action.type) {
        actionRemapNavigate();
    }
    if (ACTION_ENCODER == action.type) {
        actionRemapEncoder();
    }
}


void actionHandle(bool visible)
{
    Screen *screen = screenGet();
    ScreenMode scrMode = screen->mode;

    action.visible = visible;
    action.timeout = 0;

    switch (action.type) {
    case ACTION_DISP_EXPIRED:
        actionDispExpired(scrMode);
        break;

    case ACTION_OPEN_MENU:
        break;

    case ACTION_MENU_SELECT: {
        MenuIdx parent = menuGet()->parent;
        menuSetActive((MenuIdx)action.value);
        if (parent != menuGet()->parent) {
            screenToClear();
        }
        actionSetScreen(SCREEN_MENU, 10000);
        break;
    }
    case ACTION_MENU_CHANGE:
        menuChange((int8_t)action.value);
        actionSetScreen(SCREEN_MENU, 10000);
        break;

    default:
        break;
    }

    if (action.visible) {
        screenSetMode(action.screen);
        if (action.timeout > 0) {
            swTimSet(SW_TIM_DISPLAY, action.timeout);
        }
    }
}
