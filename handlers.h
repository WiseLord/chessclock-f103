#ifndef HANDLERS_H
#define HANDLERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

int32_t getSysTimer(void);

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);

void SysTick_Handler(void);

void TIM2_IRQHandler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif // HANDLERS_H
