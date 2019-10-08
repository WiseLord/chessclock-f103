#ifndef CHESS_H
#define CHESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef int8_t ChessSide;
enum {
    CHESS_INACTIVE = -1,

    CHESS_LEFT = 0,
    CHESS_RIGHT,

    CHESS_END,
};

typedef struct {
    int32_t gameTimeInitValue;
    int32_t moveTimeInitValue;

    int32_t tim[CHESS_END];
    int32_t gameTime;
    int32_t moveTime;
    ChessSide active;
    ChessSide firstMove;
    bool gameEnd;
} ChessClock;

void chessInit(void);
ChessClock *chessGet(void);

void chessSetMove(ChessSide color);
bool chessIsRunning(void);
void swTimeChessUpdate(void);

#ifdef __cplusplus
}
#endif

#endif // CHESS_H
