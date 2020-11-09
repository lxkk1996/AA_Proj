#ifndef _KEY_BOARD_H
#define _KEY_BOARD_H

#include "stdio.h"
#include "stdint.h"

typedef struct{
    uint8_t En;
    uint8_t D_flg;//Data_flg
    uint8_t T_Flg;
    uint8_t S_Flg;
    uint8_t Cmd;
    uint8_t Data;
}KEYBOARD_Typedef;
extern KEYBOARD_Typedef My_Keyboard;
void Keyboard_Enable(KEYBOARD_Typedef* pkey);
void Keyboard_Disable(KEYBOARD_Typedef* pkey);
uint8_t Keyboard_Detect(int x, int y, KEYBOARD_Typedef* pkey);
uint8_t Keyboard_StateData(KEYBOARD_Typedef* pkey, uint8_t* pdata);
#endif