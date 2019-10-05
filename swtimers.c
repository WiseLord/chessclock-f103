#include "swtimers.h"

static SwTimer_type swTimers[SW_TIM_END];

void swTimInit(void)
{
    for (uint8_t i = 0; i < SW_TIM_END; i++) {
        swTimers[i].value = SW_TIM_OFF;
        swTimers[i].enabled = false;
    }
}

void swTimUpdate(void)
{
    for (uint8_t i = 0; i < SW_TIM_END; i++) {
        if (swTimers[i].value > 0 && swTimers[i].enabled) {
            swTimers[i].value--;
        }
    }
}

void swTimSet(SwTimer timer, int32_t value, bool enable)
{
    swTimers[timer].value = value;
    swTimers[timer].enabled = enable;
}

int32_t swTimGet(SwTimer timer)
{
    return swTimers[timer].value;
}

void swTimEnable(SwTimer timer, bool enable)
{
    swTimers[timer].enabled = enable;
}

bool swTimeIsEnabled(SwTimer timer)
{
    return swTimers[timer].enabled;
}
