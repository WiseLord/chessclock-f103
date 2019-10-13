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
    chess.active = CHESS_INACTIVE;
    chess.firstMove = CHESS_INACTIVE;
    chess.gameEnd = false;
}

ChessClock *chessGet(void)
{
    return &chess;
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

    chess.active = side;
    if (side >= 0) {
        chess.moveTime = chess.moveTimeInitValue;
    }

    // First move color
    if (chess.firstMove == CHESS_INACTIVE) {
        chess.firstMove = side;
    }
}

bool chessIsRunning(void)
{
    return (chess.active >= 0);
}

void swTimeChessUpdate(void)
{
    // If inactive, do nothing
    if (chess.active < 0) {
        return;
    }

    if (chess.moveTime > 0) {
        chess.moveTime--;
    }

    if (chess.moveTime == 0) {
        chess.tim[chess.active]--;
        if (chess.tim[chess.active] == 0) {
            chessSetMove(CHESS_INACTIVE);
            chess.gameEnd = true;
        }
    }

    if (!chess.gameEnd) {
        chess.gameTime++;
    }
}
