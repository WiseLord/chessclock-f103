#include "labels.h"

const char *const labels_by[LABEL_END] = {
    [LABEL_BOOL_OFF]        = "ВЫКЛ",
    [LABEL_BOOL_ON]         = "УКЛ",

    [LABEL_PAL_MODE + PAL_SNOW]         = "Снег",
    [LABEL_PAL_MODE + PAL_AQUA]         = "Вада",
    [LABEL_PAL_MODE + PAL_FIRE]         = "Агонь",

    // NOTE: Keep in sync with MenuIdx in menu.h
    [LABEL_MENU + MENU_NULL]            = "Назад",

    [LABEL_MENU + MENU_SETUP]           = "Налады",

    [LABEL_MENU + MENU_SETUP_CHESS]     = "Шахматы",
    [LABEL_MENU + MENU_SETUP_SYSTEM]    = "Сістэма",
    [LABEL_MENU + MENU_SETUP_DISPLAY]   = "Дысплэй",

    [LABEL_MENU + MENU_CHESS_GAME_H]    = "Гадзін на гульню",
    [LABEL_MENU + MENU_CHESS_GAME_M]    = "Хвілін на гульню",
    [LABEL_MENU + MENU_CHESS_MOVE_S]    = "Секунд на ход",

    [LABEL_MENU + MENU_SYSTEM_LANG]     = "Мова",
    [LABEL_MENU + MENU_SYSTEM_ENC_RES]  = "Вырашэнне энкодера",

    [LABEL_MENU + MENU_DISPLAY_ROTATE]  = "Круціць",
    [LABEL_MENU + MENU_DISPLAY_PALETTE] = "Палітра",
};
