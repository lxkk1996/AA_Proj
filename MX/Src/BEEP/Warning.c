#include "Warning.h"

void Beep_Init(void)
{
  MX_TIM2_Init();
}

void Beep_Warning(uint8_t CNT)
{
  for(int i=0;i<CNT;i++)
  {
    HAL_Delay(200);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
    HAL_Delay(200);
    HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);
  }
}