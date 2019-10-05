#ifndef SCREEN_H
#define SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "gui/icons.h"

typedef uint8_t ScreenMode;
enum {
    SCREEN_CHESS = 0,
    SCREEN_MENU,

    SCREEN_END
};

typedef struct {
    ScreenMode mode;
    ScreenMode def;
    Icon iconHint;
} Screen;

void screenInit(void);

Screen *screenGet(void);

void screenSetMode(ScreenMode value);
ScreenMode screenGetMode(void);

void screenToClear(void);
void screenShow(bool clear);

#ifdef __cplusplus
}
#endif

#endif // SCREEN_H
