#ifndef SWTIMERS_H
#define SWTIMERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define SW_TIM_OFF       -1

typedef uint8_t SwTimer;
enum {
    SW_TIM_DISPLAY = 0,
    SW_TIM_INPUT_POLL,

    SW_TIM_TIME_WHITE,
    SW_TIM_TIME_BLACK,
    SW_TIM_MOVE_WHITE,
    SW_TIM_MOVE_BLACK,

    SW_TIM_END,
};

typedef struct {
    int32_t value;
    bool enabled;
} SwTimer_type;

void swTimInit(void);
void swTimUpdate(void);

void swTimSet(SwTimer timer, int32_t value, bool enable);
int32_t swTimGet(SwTimer timer);
void swTimEnable(SwTimer timer, bool enable);
bool swTimeIsEnabled(SwTimer timer);

#ifdef __cplusplus
}
#endif

#endif // SWTIMERS_H
