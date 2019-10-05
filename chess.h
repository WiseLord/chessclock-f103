#ifndef CHESS_H
#define CHESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t ChessColor;
enum {
    CHESS_WHITE = 0,
    CHESS_BLACK,

    CHESS_END,
};

typedef struct {
    int32_t gameTime;
    int32_t moveTime;
} ChessClock;

void chessInit(void);
void chessActivate(ChessColor color);
bool chessIsRunning(void);

ChessClock *chessGet(void);

#ifdef __cplusplus
}
#endif

#endif // CHESS_H
