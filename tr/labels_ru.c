#include "labels.h"

const char *const labels_ru[LABEL_END] = {
    [LABEL_BOOL_OFF]        = "ВЫКЛ",
    [LABEL_BOOL_ON]         = "ВКЛ",

    [LABEL_PAL_MODE + PAL_SNOW]         = "Снег",
    [LABEL_PAL_MODE + PAL_AQUA]         = "Вода",
    [LABEL_PAL_MODE + PAL_FIRE]         = "Огонь",

    // NOTE: Keep in sync with MenuIdx in menu.h
    [LABEL_MENU + MENU_NULL]            = "Назад",

    [LABEL_MENU + MENU_SETUP]           = "Настройки",

    [LABEL_MENU + MENU_SETUP_CHESS]     = "Шахматы",
    [LABEL_MENU + MENU_SETUP_SYSTEM]    = "Система",
    [LABEL_MENU + MENU_SETUP_DISPLAY]   = "Дисплей",

    [LABEL_MENU + MENU_CHESS_HOURS]     = "Часы",
    [LABEL_MENU + MENU_CHESS_MINUTES]   = "Минуты",
    [LABEL_MENU + MENU_CHESS_SECONDS]   = "Секунды",

    [LABEL_MENU + MENU_SYSTEM_LANG]     = "Язык",
    [LABEL_MENU + MENU_SYSTEM_ENC_RES]  = "Разрешение энкодера",

    [LABEL_MENU + MENU_DISPLAY_ROTATE]  = "Развернуть",
    [LABEL_MENU + MENU_DISPLAY_PALETTE] = "Палитра",
};
