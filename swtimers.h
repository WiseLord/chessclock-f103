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

    SW_TIM_END,
};

void swTimInit(void);
void swTimUpdate(void);

void swTimSet(SwTimer timer, int32_t value);
int32_t swTimGet(SwTimer timer);

#ifdef __cplusplus
}
#endif

#endif // SWTIMERS_H
