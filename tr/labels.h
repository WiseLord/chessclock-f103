#ifndef TR_H
#define TR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../gui/palette.h"
#include "../menu.h"
#include "../pins.h"
#include "../screen.h"

typedef enum {
    LANG_DEFAULT = 0,

    LANG_BY,
    LANG_RU,

    LANG_END
} Lang;

typedef enum {
    LABEL_BOOL_OFF,
    LABEL_BOOL_ON,

    LABEL_PAL_MODE,
    LABEL_PAL_MODE_END = LABEL_PAL_MODE + (PAL_END - PAL_DEFAULT),

    // Menu
    LABEL_MENU = LABEL_PAL_MODE_END,
    LABEL_MENU_END = LABEL_MENU + (MENU_END - MENU_NULL),

    LABEL_END = LABEL_MENU_END,
} Label;

extern const char *const labels_by[LABEL_END];
extern const char *const labels_ru[LABEL_END];

void labelsSetLang(Lang value);
Lang labelsGetLang(void);
const char *labelsGetLangName(Lang value);

const char *labelsGet(Label value);

void labelsInit(void);

#ifdef __cplusplus
}
#endif

#endif // TR_H
