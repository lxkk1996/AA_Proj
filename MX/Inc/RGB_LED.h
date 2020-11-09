#ifndef _RGB_LED_H
#define _RGB_LED_H

#include "main.h"
#include "gpio.h"
#include "tim.h"

#define RGB_G   ((uint16_t)0x0001)
#define RGB_R   ((uint16_t)0x0002)
#define RGB_B   ((uint16_t)0x0004)

extern uint16_t RGB_TWinkColor;
extern uint8_t RGB_TWinkTims;
extern uint8_t RGB_TWinkFlg;

void RGB_OFF(void);
void RGB_Red(void);
void RGB_Green(void);
void RGB_Blue(void);
void RGB_Mix(uint16_t RGBMix);
void RGB_Twinkle(uint16_t RGBMix, uint8_t Tims);
void RGB_Test(void);
#endif