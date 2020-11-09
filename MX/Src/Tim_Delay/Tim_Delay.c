#include "main.h"
#include "Tim_Delay.h"

void Tim_Delay(uint8_t us)
{
    USED_TIM->CR1 &= ~(TIM_CR1_CEN);
    USED_TIM->SR &= ~(TIM_FLAG_UPDATE);
    USED_TIM->CNT = 0;
    USED_TIM->ARR = us*BASIC_FRE - 1;
    USED_TIM->CR1 |= TIM_CR1_CEN;
    while(!(USED_TIM->SR & TIM_FLAG_UPDATE));
}