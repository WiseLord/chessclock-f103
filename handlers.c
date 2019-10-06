#include "handlers.h"

#include "hwlibs.h"

#include "chess.h"
#include "display/dispdrv.h"
#include "input.h"
#include "pins.h"
#include "swtimers.h"

static int32_t sysTimer = 0;

int32_t getSysTimer(void)
{
    return sysTimer;
}

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    while (1) {
    }
}

void MemManage_Handler(void)
{
    while (1) {
    }
}

void BusFault_Handler(void)
{
    while (1) {
    }
}

void UsageFault_Handler(void)
{
    while (1) {
    }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
    dispdrvBusIRQ();
    inputPoll();
    swTimUpdate();
    swTimeChessUpdate();

    sysTimer++;
}

void TIM2_IRQHandler(void)
{
    if (LL_TIM_IsActiveFlag_UPDATE(TIM2)) {
        // Clear the update interrupt flag
        LL_TIM_ClearFlag_UPDATE(TIM2);

        // Callbacks
        dispdrvPwm();
    }
}

void USART1_IRQHandler(void)
{
    // Check RXNE flag value in SR register
    if (LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1)) {
        LL_USART_ReceiveData8(USART1);
    } else {
        // Call Error function
    }
}

void USART2_IRQHandler(void)
{
    // Check RXNE flag value in SR register
    if (LL_USART_IsActiveFlag_RXNE(USART2) && LL_USART_IsEnabledIT_RXNE(USART2)) {
        LL_USART_ReceiveData8(USART2);
    } else {
        // Call Error function
    }
}

void TIM3_IRQHandler(void)
{
    if (LL_TIM_IsActiveFlag_UPDATE(TIM3)) {
        // Clear the update interrupt flag
        LL_TIM_ClearFlag_UPDATE(TIM3);

    }
}
