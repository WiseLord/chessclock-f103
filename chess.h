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

typedef uint8_t ChessTimer;
enum {
    CHESS_TIM_GAME_WHITE,
    CHESS_TIM_GAME_BLACK,
    CHESS_TIM_MOVE_WHITE,
    CHESS_TIM_MOVE_BLACK,

    CHESS_TIM_END,
};

typedef struct {
    int32_t value;
    bool enabled;
    bool clear;
} ChessTim;

typedef struct {
    int32_t gameTime;
    int32_t moveTime;
    ChessTim tim[CHESS_TIM_END];
} ChessClock;

void chessInit(void);
void chessActivate(ChessColor color);
bool chessIsRunning(void);
void swTimeChessUpdate(void);

void chessTimSet(ChessTimer timer, int32_t value, bool enable);

ChessClock *chessGet(void);
int32_t chessTimGet(ChessTimer timer);

#ifdef __cplusplus
}
#endif

#endif // CHESS_H
