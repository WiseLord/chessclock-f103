#include "chess.h"

#include "settings.h"
#include "swtimers.h"

static ChessClock chess;

void chessTimSet(ChessTimer timer, int32_t value, bool enable)
{
    chess.tim[timer].enabled = enable;
    chess.tim[timer].value = value;
}

void chessInit(void)
{
    int16_t h = settingsGet(PARAM_CHESS_HOURS);
    int16_t m = settingsGet(PARAM_CHESS_MINUTES);
    int16_t s = settingsGet(PARAM_CHESS_SECONDS);

    chess.gameTimerInitValue = ((h * 60 + m) * 60 + s) * 1000;
    chess.moveTimerInitValue = 5000;

    chessTimSet(CHESS_TIM_GAME_WHITE, chess.gameTimerInitValue, false);
    chessTimSet(CHESS_TIM_GAME_BLACK, chess.gameTimerInitValue, false);

    chessTimSet(CHESS_TIM_MOVE, chess.moveTimerInitValue, false);

    chessTimSet(CHESS_TIME_TOTAL, 0, false);
    chess.gameMask = 0;
    chess.gameFinished = false;
}

void chessActivate(ChessColor color)
{
    if (chess.gameFinished) {
        return;
    }

    if (chess.tim[CHESS_TIM_GAME_WHITE + color].enabled) {
        return;
    }

    for (ChessColor c = CHESS_WHITE; c < CHESS_END; c++) {
        chess.tim[CHESS_TIM_GAME_WHITE + c].enabled = (c == color);
    }
    chessTimSet(CHESS_TIM_MOVE, chess.moveTimerInitValue, false);
    if (color < CHESS_END) {
        chess.tim[CHESS_TIME_TOTAL].enabled = true;
        if (!chess.gameMask) {
            chess.gameMask = (uint8_t )(1 << color);
        }
    }
}

void chessPause(void)
{
    for (ChessColor c = CHESS_WHITE; c < CHESS_END; c++) {
        chess.tim[CHESS_TIM_GAME_WHITE + c].enabled = false;
    }
    chess.tim[CHESS_TIM_MOVE].enabled = false;
    chess.tim[CHESS_TIME_TOTAL].enabled = false;
}

bool chessIsRunning(void)
{
    return chess.tim[CHESS_TIME_TOTAL].enabled;
}

ChessClock *chessGet(void)
{
    return &chess;
}

void swTimeChessUpdate(void)
{
    for (uint8_t i = 0; i < CHESS_TIM_COLOR_END; i++) {
        if (chess.tim[i].enabled && chess.tim[i].value > 0) {
            if (chess.tim[CHESS_TIM_MOVE].value > 0) {
                chess.tim[CHESS_TIM_MOVE].value--;
            }
            if (chess.tim[CHESS_TIM_MOVE].value == 0) {
                chess.tim[i].value--;
                if (chess.tim[i].value == 0) {
                    chessPause();
                    chess.gameFinished = true;
                }
            }
        }
    }
    if (chess.tim[CHESS_TIME_TOTAL].enabled) {
        chess.tim[CHESS_TIME_TOTAL].value++;
    }
}

int32_t chessTimGet(ChessTimer timer)
{
    return chess.tim[timer].value;
}
