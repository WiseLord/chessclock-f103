#include "settings.h"

#include "chess.h"
#include "display/glcd.h"
#include "eemul.h"
#include "screen.h"
#include "tr/labels.h"

static int16_t chessGameH;
static int16_t chessGameM;
static int16_t chessGameS;

#define GENERATE_EE_RC_MAP(CMD)  [PARAM_RC_ ## CMD] = {0x80 + RC_CMD_ ## CMD, (int16_t)EE_NOT_FOUND},

static const EE_Map eeMap[] = {
    [PARAM_NULL]            =   {0x00,  0},

    [PARAM_DISPLAY_ROTATE]  =   {0x42,  false},
    [PARAM_DISPLAY_PALETTE] =   {0x44,  PAL_DEFAULT},

    [PARAM_CHESS_GAME_H]    =   {0x60,  0},
    [PARAM_CHESS_GAME_M]    =   {0x61,  5},
    [PARAM_CHESS_MOVE_S]    =   {0x62,  0},
    [PARAM_CHESS_MOVE_TYPE] =   {0x63,  MOVE_TYPE_NONE},

    [PARAM_SYSTEM_LANG]     =   {0x70,  LANG_DEFAULT},
    [PARAM_SYSTEM_ENC_RES]  =   {0x74,  4},
};

void settingsInit(void)
{
    eeInit();

    settingsSet(PARAM_CHESS_GAME_H, settingsRead(PARAM_CHESS_GAME_H));
    settingsSet(PARAM_CHESS_GAME_M, settingsRead(PARAM_CHESS_GAME_M));
    settingsSet(PARAM_CHESS_MOVE_S, settingsRead(PARAM_CHESS_MOVE_S));
}

int16_t settingsGet(Param param)
{
    int16_t ret = 0;

    switch (param) {
    case PARAM_CHESS_GAME_H:
        ret = chessGameH;
        break;
    case PARAM_CHESS_GAME_M:
        ret = chessGameM;
        break;
    case PARAM_CHESS_MOVE_S:
        ret = chessGameS;
        break;
    default:
        break;
    }

    return ret;
}

void settingsSet(Param param, int16_t value)
{
    switch (param) {
    case PARAM_CHESS_GAME_H:
        chessGameH = value;
        break;
    case PARAM_CHESS_GAME_M:
        chessGameM = value;
        break;
    case PARAM_CHESS_MOVE_S:
        chessGameS = value;
        break;
    default:
        break;
    }
}

int16_t settingsRead(Param param)
{
    if (param == PARAM_NULL || param >= PARAM_END) {
        return (int16_t)EE_NOT_FOUND;
    }

    uint16_t value = eeReadRaw(eeMap[param].cell);

    if (value != EE_NOT_FOUND) {
        return (int16_t)value;
    }

    // Return default value if not found
    return eeMap[param].def;
}

void settingsStore(Param param, int16_t value)
{
    if (param == PARAM_NULL || param >= PARAM_END) {
        return;
    }

    eeUpdateRaw(eeMap[param].cell, (uint16_t)value);
}

const EE_Map *eeMapGet(void)
{
    return eeMap;
}
