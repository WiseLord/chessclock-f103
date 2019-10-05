#include "menu.h"

#include <string.h>

#include "gui/canvas.h"
#include "input.h"
#include "screen.h"
#include "settings.h"
#include "tr/labels.h"
#include "utils.h"

#define GENERATE_MENU_ITEM(CMD)    [MENU_RC_ ## CMD] = {MENU_SETUP_RC, MENU_TYPE_RC, PARAM_RC_ ## CMD},

static Menu menu;

typedef struct {
    MenuIdx parent;
    MenuType type;
    Param param;
} MenuItem;

static const MenuItem menuItems[MENU_END] = {
//   menu index                parent menu              menu type           parameter
    [MENU_NULL]             = {MENU_NULL,               MENU_TYPE_PARENT,   PARAM_NULL},

    [MENU_SETUP]            = {MENU_NULL,               MENU_TYPE_PARENT,   PARAM_NULL},

    [MENU_SETUP_SYSTEM]     = {MENU_SETUP,              MENU_TYPE_PARENT,   PARAM_NULL},
    [MENU_SETUP_DISPLAY]    = {MENU_SETUP,              MENU_TYPE_PARENT,   PARAM_NULL},
    [MENU_SETUP_CHESS]      = {MENU_SETUP,              MENU_TYPE_PARENT,   PARAM_NULL},

    [MENU_SYSTEM_LANG]      = {MENU_SETUP_SYSTEM,       MENU_TYPE_ENUM,     PARAM_SYSTEM_LANG},
    [MENU_SYSTEM_ENC_RES]   = {MENU_SETUP_SYSTEM,       MENU_TYPE_NUMBER,   PARAM_SYSTEM_ENC_RES},

    [MENU_CHESS_HOURS]       = {MENU_SETUP_CHESS,       MENU_TYPE_NUMBER,   PARAM_CHESS_HOURS},
    [MENU_CHESS_MINUTES]     = {MENU_SETUP_CHESS,       MENU_TYPE_NUMBER,   PARAM_CHESS_MINUTES},
    [MENU_CHESS_SECONDS]     = {MENU_SETUP_CHESS,       MENU_TYPE_NUMBER,   PARAM_CHESS_SECONDS},

    [MENU_DISPLAY_ROTATE]   = {MENU_SETUP_DISPLAY,      MENU_TYPE_BOOL,     PARAM_DISPLAY_ROTATE},
    [MENU_DISPLAY_PALETTE]  = {MENU_SETUP_DISPLAY,      MENU_TYPE_ENUM,     PARAM_DISPLAY_PALETTE},
};

static void menuMove(int8_t diff)
{
    int8_t newIdx = 0;

    for (int8_t idx = 0; idx < MENU_MAX_LEN; idx++) {
        if (menu.list[idx] == menu.active) {
            newIdx = idx;
            break;
        }
    }
    newIdx = (int8_t)((newIdx + menu.listSize + diff) % menu.listSize);

    menu.active = menu.list[newIdx];

    // Recalculate offset if needed
    if (menu.dispOft < newIdx - (menu.dispSize - 1)) {
        menu.dispOft = newIdx - (menu.dispSize - 1);
    } else if (menu.dispOft > newIdx) {
        menu.dispOft = newIdx;
    }
}

static int16_t menuGetValue(MenuIdx index)
{
    int16_t ret = 0;

    switch (index) {
    case MENU_SYSTEM_LANG:
        ret = (int16_t)(labelsGetLang());
        break;
    case MENU_SYSTEM_ENC_RES:
        ret = inputGetEncRes();
        break;

    case MENU_DISPLAY_ROTATE:
        ret = glcdGetRotate();
        break;
    case MENU_DISPLAY_PALETTE:
        ret = paletteGetIndex();
        break;

    default:
        ret = settingsGet(menuItems[index].param);
        break;
    }

    return ret;
}

static void menuStoreCurrentValue(void)
{
    switch (menu.active) {
    case MENU_SYSTEM_LANG:
        labelsSetLang((Lang)(menu.value));
        break;
    case MENU_SYSTEM_ENC_RES:
        inputSetEncRes((int8_t)menu.value);
        break;

    case MENU_DISPLAY_ROTATE:
        glcdRotate((bool)menu.value);
        canvasClear();
        break;
    case MENU_DISPLAY_PALETTE:
        paletteSetIndex((PalIdx)menu.value);
        canvasGet()->pal = paletteGet((PalIdx)menu.value);
        break;

    default:
        break;
    }

    if (menu.active < MENU_END) {
        Param param = menuItems[menu.active].param;
        settingsSet(param, menu.value);
        settingsStore(param, menu.value);
    }

}

static void menuValueChange(int8_t diff)
{
    if (menuItems[menu.active].type == MENU_TYPE_BOOL) {
        if (diff)
            menu.value = !menu.value;
        return;
    }

    menu.value += diff;

    switch (menu.active) {
    case MENU_SYSTEM_LANG:
        if (menu.value > LANG_END - 1)
            menu.value = LANG_END - 1;
        if (menu.value < LANG_DEFAULT)
            menu.value = LANG_DEFAULT;
        break;
    case MENU_SYSTEM_ENC_RES:
        if (menu.value > ENC_RES_MAX)
            menu.value = ENC_RES_MAX;
        if (menu.value < ENC_RES_MIN)
            menu.value = ENC_RES_MIN;
        break;

    case MENU_DISPLAY_PALETTE:
        if (menu.value > PAL_END - 1)
            menu.value = PAL_END - 1;
        if (menu.value < PAL_DEFAULT)
            menu.value = PAL_DEFAULT;
        break;

    case MENU_CHESS_HOURS:
        if (menu.value > 23)
            menu.value = 0;
        if (menu.value < 0)
            menu.value = 23;
        break;
    case MENU_CHESS_MINUTES:
    case MENU_CHESS_SECONDS:
        if (menu.value > 59)
            menu.value = 0;
        if (menu.value < 0)
            menu.value = 59;
        break;
    default:
        break;
    }
}

static bool menuIsValid(MenuIdx index)
{
    switch (index) {
    case MENU_NULL:
        // Don't allow null menu
        return false;
    }

    return true;
}

static void menuSelect(MenuIdx index)
{
    menu.selected = 0;

    menu.active = (index != MENU_NULL) ? index : menu.parent;
    menu.parent = menuItems[index].parent;
    menu.dispOft = 0;

    uint8_t idx;

    for (idx = 0; idx < MENU_MAX_LEN; idx++) {
        menu.list[idx] = 0;
    }

    idx = 0;
    if (!menuIsTop()) {
        menu.list[idx++] = MENU_NULL;
        menu.active = MENU_NULL;
    }
    for (MenuIdx item = 0; item < MENU_END; item++) {
        if ((menuItems[item].parent == menu.parent) && menuIsValid(item)) {
            menu.list[idx++] = (uint8_t)item;
            if (idx >= MENU_MAX_LEN) {
                break;
            }
        }
    }

    menu.listSize = idx;
}

static void menuUpdate(MenuIdx index)
{
    canvasClear();
    menuSelect(index);
    menu.active = index;
}

Menu *menuGet(void)
{
    return &menu;
}

void menuSetActive(MenuIdx index)
{
    if (menu.active == index) {
        menu.selected = !menu.selected;

        if (menu.selected) {
            menu.value = menuGetValue(menu.active);
        } else {
            menuStoreCurrentValue();
            menuUpdate(index);
        }

        return;
    }

    menuSelect(index);
}

void menuChange(int8_t diff)
{
    if (diff > 0)
        diff = 1;
    else if (diff < 0)
        diff = -1;

    if (menu.selected) {
        menuValueChange(diff);
    } else {
        menuMove(diff);
    }
}

bool menuIsTop(void)
{
    // TODO: Top menu on first selection instead of MENU_SETUP
    return (menu.parent == MENU_NULL || menu.parent == MENU_SETUP);
}

MenuIdx menuGetFirstChild(void)
{
    if (menu.active == MENU_NULL)
        return menu.parent;

    for (MenuIdx item = 0; item < MENU_END; item++) {
        if (menuItems[item].parent == menu.active) {
            return item;
        }
    }
    return menu.active;
}

const char *menuGetName(MenuIdx index)
{
    char *name = utilMkStr("%s", labelsGet((Label)(LABEL_MENU + (index - MENU_NULL))));

    return name;
}

const char *menuGetValueStr(MenuIdx index)
{
    const char *ret = ">";
    static const char *noVal = "--";

    // Parent menu type
    if (menuItems[index].type == MENU_TYPE_PARENT) {
        return (index == MENU_NULL) ? "" : ret;
    }

    int16_t value = menuGetValue(index);

    if (index == menu.active && menu.selected)
        value = menu.value;

    // Bool menu type
    if (menuItems[index].type == MENU_TYPE_BOOL) {
        ret = labelsGet((Label)(LABEL_BOOL_OFF + value));
        return ret;
    }

    if (menuItems[index].type == MENU_TYPE_NUMBER) {
        ret = utilMkStr("%5d", value);
        return ret;
    }

    if (menuItems[index].type == MENU_TYPE_RC) {
        if ((uint16_t)value == EE_NOT_FOUND) {
            ret = noVal;
        } else {
            ret = utilMkStr("%04d", (uint16_t)value);
        }
        return ret;
    }

    // Enum menu types
    switch (index) {
    case MENU_SYSTEM_LANG:
        ret = labelsGetLangName((Lang)value);
        break;

    case MENU_DISPLAY_PALETTE:
        ret = labelsGet((Label)(LABEL_PAL_MODE + value));
        break;
    default:
        ret = noVal;
        break;
    }

    return ret;
}
