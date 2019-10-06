#include "chess.h"

#include "settings.h"
#include "swtimers.h"

static ChessClock chess;
static ChessTimer_type chessTimer[CHESS_TIM_END];

void chessTimSet(ChessTimer timer, int32_t value, bool enable)
{
    chessTimer[timer].value = value;
    chessTimer[timer].enabled = enable;
}

void chessInit(void)
{
    int16_t h = settingsGet(PARAM_CHESS_HOURS);
    int16_t m = settingsGet(PARAM_CHESS_MINUTES);
    int16_t s = settingsGet(PARAM_CHESS_SECONDS);

    chess.gameTime = ((h * 60 + m) * 60 + s) * 1000;

    chessTimSet(CHESS_TIM_GAME_WHITE, chess.gameTime, false);
    chessTimSet(CHESS_TIM_GAME_BLACK, chess.gameTime, false);
}

void chessActivate(ChessColor color)
{
    for (ChessColor c = CHESS_WHITE; c < CHESS_END; c++) {
        chessTimer[CHESS_TIM_GAME_WHITE + c].enabled = (c == color);
    }
}

bool chessIsRunning(void)
{
    for (ChessColor c = CHESS_WHITE; c < CHESS_END; c++) {
        if (chessTimer[CHESS_TIM_GAME_WHITE + c].enabled) {
            return true;
        }
    }
    return false;
}

ChessClock *chessGet(void)
{
    return &chess;
}

void swTimeChessUpdate(void)
{
    for (uint8_t i = 0; i < CHESS_TIM_END; i++) {
        if (chessTimer[i].enabled && chessTimer[i].value >= 0) {
            chessTimer[i].value--;
        }
    }
}

int32_t chessTimGet(ChessTimer timer)
{
    return chessTimer[timer].value;
}
