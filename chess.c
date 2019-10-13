#include "chess.h"

#include "settings.h"
#include "swtimers.h"

static ChessClock chess;

void chessInit(void)
{
    int16_t gh = settingsGet(PARAM_CHESS_GAME_H);
    int16_t gm = settingsGet(PARAM_CHESS_GAME_M);
    int16_t ms = settingsGet(PARAM_CHESS_MOVE_S);

    chess.gameTimeInitValue = ((gh * 60 + gm) * 60) * 1000;

    chess.moveTimeInitValue = ms * 1000;

    // Reset to default values
    for (ChessSide color = CHESS_LEFT; color < CHESS_END; color++) {
        chess.tim[color] = chess.gameTimeInitValue;
    }
    chess.gameTime = 0;
    chess.moveTime = 0;

    chess.moveType = (ChessMoveType)(settingsRead(PARAM_CHESS_MOVE_TYPE));

    chess.active = CHESS_INACTIVE;
    chess.firstMove = CHESS_INACTIVE;
    chess.gameEnd = false;
}

ChessClock *chessGet(void)
{
    return &chess;
}

static void chessChangeSide()
{
    int32_t inc = 0;

    switch (chess.moveType) {
    case MOVE_TYPE_DELAY:
        chess.moveTime = chess.moveTimeInitValue;
        break;
    case MOVE_TYPE_BRONSTEIN:
        if (chess.active >= 0) {
            inc = chess.moveTimeInitValue;
            if (chess.moveTime > 0) {
                inc -= chess.moveTime;
            }
        }
        chess.moveTime = chess.moveTimeInitValue;
        break;
    case MOVE_TYPE_FISCHER:
        if (chess.active >= 0) {
            inc = chess.moveTimeInitValue;
        }
        chess.moveTime = chess.moveTimeInitValue;
        break;
    default:
        break;
    }

    chess.tim[chess.active] += inc;
}

void chessSetMove(ChessSide side)
{
    // Game is finished, do nothing
    if (chess.gameEnd) {
        return;
    }

    // Color is already active, do nothing
    if (chess.active == side) {
        return;
    }

    if (side >= 0) {
        chessChangeSide();
    }
    chess.active = side;

    // First move color
    if (chess.firstMove == CHESS_INACTIVE) {
        chess.firstMove = side;
    }
}

bool chessIsRunning(void)
{
    return (chess.active >= 0);
}

void chessUpdateTimers(void)
{
    // If inactive, do nothing
    if (chess.active < 0) {
        return;
    }

    if (chess.moveTime > 0) {
        chess.moveTime--;
    }

    if (chess.moveType == MOVE_TYPE_BRONSTEIN ||
        chess.moveType == MOVE_TYPE_FISCHER ||
        chess.moveTime == 0) {
        if (--chess.tim[chess.active] == 0) {
            chessSetMove(CHESS_INACTIVE);
            chess.gameEnd = true;
        }
    }

    if (!chess.gameEnd) {
        chess.gameTime++;
    }
}
