#include "chess.h"

#include "settings.h"
#include "swtimers.h"

static ChessClock chess;

void chessTimSet(ChessTimer timer, int32_t value, bool enable)
{
    chess.tim[timer].value = value;
    chess.tim[timer].enabled = enable;
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
        chess.tim[CHESS_TIM_GAME_WHITE + c].enabled = (c == color);
    }
}

bool chessIsRunning(void)
{
    for (ChessColor c = CHESS_WHITE; c < CHESS_END; c++) {
        if (chess.tim[CHESS_TIM_GAME_WHITE + c].enabled) {
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
        if (chess.tim[i].enabled && chess.tim[i].value >= 0) {
            chess.tim[i].value--;
        }
    }
}

int32_t chessTimGet(ChessTimer timer)
{
    return chess.tim[timer].value;
}
