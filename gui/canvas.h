#ifndef CANVAS_H
#define CANVAS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "layout.h"
#include "palette.h"

typedef struct {
    Glcd *glcd;
    const Palette *pal;
    const Layout *layout;
} Canvas;

typedef uint8_t RtcMode;
enum {
    RTC_HOUR,
    RTC_MIN,
    RTC_SEC,

    RTC_NOEDIT,
};

typedef struct {
    int8_t hour;
    int8_t min;
    int8_t sec;
} RTC_type;

void canvasInit(void);
Canvas *canvasGet(void);

void canvasClear(void);

void canvasShowMenu(bool clear);
void canvasShowChess(bool clear);

#ifdef __cplusplus
}
#endif

#endif // CANVAS_H
