#include "main.h"
#include "tim.h"
#include "DC_Control.h"

int16_t L_CNT = 0;
int16_t R_CNT = 0;

void DM_DIR_CTL_L(uint8_t Dir)
{
    switch(Dir)
    {
        case Dir_N:
        {
           INPUT_A1_GPIO_Port->BSRR = (uint32_t)INPUT_A1_Pin << 16U;
           INPUT_A2_GPIO_Port->BSRR = (uint32_t)INPUT_A2_Pin << 16U;
        }break;
        case Dir_S:
        {
           INPUT_A1_GPIO_Port->BSRR = INPUT_A1_Pin;
           INPUT_A2_GPIO_Port->BSRR = INPUT_A2_Pin;
        }break;
        case Dir_F:
        {
           INPUT_A1_GPIO_Port->BSRR = INPUT_A1_Pin;
           INPUT_A2_GPIO_Port->BSRR = (uint32_t)INPUT_A2_Pin << 16U;
        }break;
        case Dir_B:
        {
           INPUT_A2_GPIO_Port->BSRR = INPUT_A2_Pin;
           INPUT_A1_GPIO_Port->BSRR = (uint32_t)INPUT_A1_Pin << 16U;
        }break;
    }
}

void DM_DIR_CTL_R(uint8_t Dir)
{
    switch(Dir)
    {
        case Dir_N:
        {
           INPUT_B1_GPIO_Port->BSRR = (uint32_t)INPUT_B1_Pin << 16U;
           INPUT_B2_GPIO_Port->BSRR = (uint32_t)INPUT_B2_Pin << 16U;
        }break;
        case Dir_S:
        {
           INPUT_B1_GPIO_Port->BSRR = INPUT_B1_Pin;
           INPUT_B2_GPIO_Port->BSRR = INPUT_B2_Pin;
        }break;
        case Dir_F:
        {
           INPUT_B2_GPIO_Port->BSRR = INPUT_B2_Pin;
           INPUT_B1_GPIO_Port->BSRR = (uint32_t)INPUT_B1_Pin << 16U;
           
        }break;
        case Dir_B:
        {
           INPUT_B1_GPIO_Port->BSRR = INPUT_B1_Pin;
           INPUT_B2_GPIO_Port->BSRR = (uint32_t)INPUT_B2_Pin << 16U;
        }break;
    }
}

void DM_SetPWM_L(uint16_t PWM)
{
    TIM8->CCR1 = PWM;
}

void DM_SetPWM_R(uint16_t PWM)
{
    TIM8->CCR2 = PWM;
}

void DM_WheelControl_L(int16_t PWM)
{
    if(PWM > 0)
    {
        DM_DIR_CTL_L(Dir_F);
        DM_SetPWM_L(PWM);
    }
    else if(PWM < 0)
    {
        DM_DIR_CTL_L(Dir_B);
        DM_SetPWM_L(-PWM);
    }
    else
    {
        DM_DIR_CTL_L(Dir_N);
        DM_SetPWM_L(PWM);
    }
}

void DM_WheelControl_R(int16_t PWM)
{
    if(PWM > 0)
    {
        DM_DIR_CTL_R(Dir_F);
        DM_SetPWM_R(PWM);
    }
    else if(PWM < 0)
    {
        DM_DIR_CTL_R(Dir_B);
        DM_SetPWM_R(-PWM);
    }
    else
    {
        DM_DIR_CTL_R(Dir_N);
        DM_SetPWM_R(PWM);
    }
}

void DM_WheelControl_All(int16_t PWM)
{
    if(PWM > 0)
    {
        DM_DIR_CTL_L(Dir_F);
        DM_SetPWM_L(PWM);
        DM_DIR_CTL_R(Dir_F);
        DM_SetPWM_R(PWM);
    }
    else if(PWM < 0)
    {
        DM_DIR_CTL_L(Dir_B);
        DM_SetPWM_L(-PWM);
        DM_DIR_CTL_R(Dir_B);
        DM_SetPWM_R(-PWM);
    }
    else
    {
        DM_DIR_CTL_L(Dir_N);
        DM_SetPWM_L(PWM);
        DM_DIR_CTL_R(Dir_N);
        DM_SetPWM_R(PWM);
    }
}

void DM_PWM_Engage(uint8_t EngageCMD)
{
    switch(EngageCMD)
    {
        case DisEngage:
        {
            DM_SetPWM_L(0);
            DM_SetPWM_R(0);
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
            HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
        }break;
        case Engage:
        {
            DM_SetPWM_L(0);
            DM_SetPWM_R(0);
            HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
            HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
        }break;
    }
}

void DM_EncodeStart(void)
{
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
}

void DM_EncodeStop(void)
{
    HAL_TIM_Encoder_Stop(&htim3, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Stop(&htim4, TIM_CHANNEL_ALL);
}

int16_t DM_ReadCNT_L(void)
{
    int16_t CNT = 0;
    CNT = -(int16_t)(TIM4->CNT);
    TIM4->CNT = 0;
    return -CNT;
}

int16_t DM_ReadCNT_R(void)
{
    int16_t CNT = 0;
    CNT = (int16_t)(TIM3->CNT);
    TIM3->CNT = 0;
    return CNT;
}

