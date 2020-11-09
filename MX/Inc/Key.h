#ifndef _Key_H
#define _Key_H

#include "main.h"
#include "gpio.h"
#include "tim.h"

typedef enum{
    KEY_NONE = 0,
    KEY_OK,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT
}KEY_STATE_ENUM;

KEY_STATE_ENUM Key_Detect(void);
KEY_STATE_ENUM Key_GetState(void);
uint8_t* Key_StateData(void);
void Key_init();
#endif