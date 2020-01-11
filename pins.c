#include "pins.h"

#include "hwlibs.h"

#include "settings.h"

static void pinsInitButtons(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStructf;

    GPIO_InitStructf.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStructf.Pull = LL_GPIO_PULL_UP;

    GPIO_InitStructf.Pin = DISP_DATA_Pin;
    LL_GPIO_Init(DISP_DATA_Port, &GPIO_InitStructf);
}

static void pinsInitDisplay(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
#ifdef STM32F3
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
#endif

    GPIO_InitStruct.Pin = DISP_CS_Pin;
    LL_GPIO_Init(DISP_CS_Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = DISP_BCKL_Pin;
    LL_GPIO_Init(DISP_BCKL_Port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = DISP_RS_Pin;
    LL_GPIO_Init(DISP_RS_Port, &GPIO_InitStruct);
#ifndef _DISP_SPI
    GPIO_InitStruct.Pin = DISP_WR_Pin;
    LL_GPIO_Init(DISP_WR_Port, &GPIO_InitStruct);
#endif
}

void pinsInit(void)
{
    // Enable clock for all GPIO peripherials
#ifdef STM32F1
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
#endif
#ifdef STM32F3
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
#endif

    pinsInitButtons();
    pinsInitDisplay();

#ifdef STM32F1
    LL_GPIO_AF_Remap_SWJ_NOJTAG();
#endif
}

void pinsSetBckl(bool value)
{
    if (value) {
        SET(DISP_BCKL);
    } else {
        CLR(DISP_BCKL);
    }
}
