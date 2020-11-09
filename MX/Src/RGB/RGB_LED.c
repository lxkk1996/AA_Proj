#include "RGB_LED.h"

void RGB_OFF(void)
{
    RGB_R_GPIO_Port->BSRR = RGB_R_Pin | RGB_G_Pin | RGB_B_Pin;
}

void RGB_Red(void)
{
    RGB_R_GPIO_Port->BSRR = ((uint32_t)RGB_R_Pin << 16U) | RGB_G_Pin | RGB_B_Pin;
}

void RGB_Green(void)
{
    RGB_R_GPIO_Port->BSRR = (uint32_t)RGB_G_Pin << 16U | RGB_R_Pin | RGB_B_Pin;
}

void RGB_Blue(void)
{
    RGB_R_GPIO_Port->BSRR = ((uint32_t)RGB_B_Pin << 16U) | RGB_R_Pin | RGB_G_Pin;
}

void RGB_Mix(uint16_t RGBMix)
{
    RGB_R_GPIO_Port->BSRR = ((uint32_t)RGBMix << 16) | (~RGBMix & 0x0007);
}

void RGB_Test(void)
{
    RGB_Mix(RGB_R);
    HAL_Delay(200);
    RGB_Mix(RGB_G);
    HAL_Delay(200);
    RGB_Mix(RGB_B);
    HAL_Delay(200);
    RGB_Mix(RGB_R);
    HAL_Delay(200);
    RGB_Mix(RGB_G);
    HAL_Delay(200);
    RGB_Mix(RGB_B);
    HAL_Delay(200);
    RGB_OFF();
}

uint16_t RGB_TWinkColor;
uint8_t RGB_TWinkTims;
uint8_t RGB_TWinkFlg;
void RGB_Twinkle(uint16_t RGBMix, uint8_t Tims)
{
    RGB_TWinkColor = RGBMix;
    RGB_TWinkTims = 2 * Tims;
    RGB_TWinkFlg = 1;
    TIM7->ARR = 9999;
    TIM7->CNT = 0;
    HAL_TIM_Base_Start_IT(&htim7);
}