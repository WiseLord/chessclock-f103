#include "chess.h"

#include "settings.h"
#include "swtimers.h"

static ChessClock chess;

void chessInit(void)
{
    int16_t h = settingsGet(PARAM_CHESS_HOURS);
    int16_t m = settingsGet(PARAM_CHESS_MINUTES);
    int16_t s = settingsGet(PARAM_CHESS_SECONDS);

    chess.gameTime = ((h * 60 + m) * 60 + s) * 1000;

    swTimSet(SW_TIM_TIME_WHITE, chess.gameTime, false);
    swTimSet(SW_TIM_TIME_BLACK, chess.gameTime, false);
}

void chessActivate(ChessColor color)
{
    for (ChessColor c = CHESS_WHITE; c < CHESS_END; c++) {
        swTimEnable(SW_TIM_TIME_WHITE + c, c == color);
    }
}

bool chessIsRunning(void)
{
    for (ChessColor c = CHESS_WHITE; c < CHESS_END; c++) {
        if (swTimeIsEnabled(SW_TIM_TIME_WHITE + c)) {
            return true;
        }
    }
    return false;
}

ChessClock *chessGet(void)
{
    return &chess;
}
