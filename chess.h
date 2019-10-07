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

    CHESS_TIM_COLOR_END,

    CHESS_TIME_TOTAL = CHESS_TIM_COLOR_END,
    CHESS_TIM_MOVE,

    CHESS_TIM_END,
};

typedef struct {
    int32_t gameTimerInitValue;
    int32_t moveTimerInitValue;
    struct {
        int32_t value;
        bool enabled;
    } tim[CHESS_TIM_END];
    uint8_t gameMask;
    bool gameFinished;
} ChessClock;

void chessInit(void);
void chessActivate(ChessColor color);
void chessPause(void);
bool chessIsRunning(void);
void swTimeChessUpdate(void);

ChessClock *chessGet(void);
int32_t chessTimGet(ChessTimer timer);

#ifdef __cplusplus
}
#endif

#endif // CHESS_H
