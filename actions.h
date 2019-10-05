#ifndef ACTIONS_H
#define ACTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "screen.h"

#define FLAG_OFF        0
#define FLAG_ON         1

#define FLAG_EXIT       0
#define FLAG_ENTER      1
#define FLAG_SWITCH     2

#define FLAG_NEXT       -1
#define FLAG_PREV       -2

#define ACTION_HIDDEN   false
#define ACTION_VISIBLE  true

#define DIRECTION_UP    1
#define DIRECTION_DOWN  -1

#define GENERATE_NAV(RC)    CMD_ ## RC,

#define FOREACH_CMD(CMD)    \
    CMD(NAV_OK)             \
    CMD(NAV_BACK)           \
    CMD(NAV_RIGHT)          \
    CMD(NAV_UP)             \
    CMD(NAV_LEFT)           \
    CMD(NAV_DOWN)           \

typedef uint16_t RcCmd;
enum {
    FOREACH_CMD(GENERATE_NAV)

    RC_CMD_END
};

typedef uint8_t ActionType;
enum {
    ACTION_NONE = 0,

    ACTION_BTN_SHORT,
    ACTION_BTN_LONG,
    ACTION_ENCODER,

    ACTION_DISP_EXPIRED,

    ACTION_OPEN_MENU,
    ACTION_NAVIGATE,

    ACTION_MENU_SELECT,
    ACTION_MENU_CHANGE,

    ACTION_TYPE_END
};

typedef struct {
    ActionType type;
    bool visible;
    int16_t value;

    ScreenMode screen;
    int16_t timeout;

    ScreenMode prevScreen;
} Action;

void actionUserGet(void);
void actionHandle(bool visible);

#ifdef __cplusplus
}
#endif

#endif // ACTIONS_H
