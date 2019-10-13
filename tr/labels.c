#include "labels.h"

#include "../settings.h"

#define GENERATE_MENU_RC_TEXT(CMD)  [LABEL_MENU + MENU_RC_ ## CMD] = # CMD,
#define GENERATE_AUDIO_IC_TEXT(IC)  [LABEL_AUDIO_IC + AUDIO_IC_ ## IC] = # IC,
#define GENERATE_TUNER_IC_TEXT(IC)  [LABEL_TUNER_IC + TUNER_IC_ ## IC] = # IC,

static Lang lang = LANG_END;

static const char *const lang_names[LANG_END] = {
    [LANG_DEFAULT]         = "English",
    [LANG_BY]              = "Беларуская",
    [LANG_RU]              = "Русский",
};

static const char *const labels_default[LABEL_END] = {
    [LABEL_BOOL_OFF]        = "OFF",
    [LABEL_BOOL_ON]         = "ON",

    [LABEL_PAL_MODE + PAL_SNOW]         = "Snow",
    [LABEL_PAL_MODE + PAL_AQUA]         = "Aqua",
    [LABEL_PAL_MODE + PAL_FIRE]         = "Fire",

    // NOTE: Keep in sync with MenuIdx in menu.h
    [LABEL_MENU + MENU_NULL]            = "Up menu",

    [LABEL_MENU + MENU_SETUP]           = "Settings",

    [LABEL_MENU + MENU_SETUP_CHESS]     = "Chess",
    [LABEL_MENU + MENU_SETUP_SYSTEM]    = "System",
    [LABEL_MENU + MENU_SETUP_DISPLAY]   = "Display",

    [LABEL_MENU + MENU_CHESS_GAME_H]    = "Hours for game",
    [LABEL_MENU + MENU_CHESS_GAME_M]    = "Minutes for game",
    [LABEL_MENU + MENU_CHESS_MOVE_S]    = "Seconds for move",

    [LABEL_MENU + MENU_SYSTEM_LANG]     = "Language",
    [LABEL_MENU + MENU_SYSTEM_ENC_RES]  = "Encoder resolution",

    [LABEL_MENU + MENU_DISPLAY_ROTATE]  = "Rotate",
    [LABEL_MENU + MENU_DISPLAY_PALETTE] = "Palette",
};

void labelsSetLang(Lang value)
{
    lang = value;
}

Lang labelsGetLang(void)
{
    return  lang;
}

const char *labelsGetLangName(Lang value)
{
    return lang_names[value];
}

const char *labelsGet(Label value)
{
    const char *ret = labels_default[value];

    switch (lang) {
    case LANG_BY:
        if (labels_by[value])
            ret = labels_by[value];
        break;
    case LANG_RU:
        if (labels_ru[value])
            ret = labels_ru[value];
        break;
    default:
        break;
    }

    return ret;
}

void labelsInit(void)
{
    lang = (Lang)(settingsRead(PARAM_SYSTEM_LANG));
}
