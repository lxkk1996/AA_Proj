#ifndef _MY_UI_H
#define _MY_UI_H

#include "stdint.h"
#include "stdio.h"

#include "LCD_TFT.h"
#include "Menu.h"
#include "My_Menu.h"
#include "ff.h"
#include "fatfs.h"
#include "Load.h"
#include "RGB_LED.h"
#include "LCD_RTS.h"
#include "key.h"
#include "KeyBoard.h"
#include "My_SD.h"
#include "OLED.h"

uint8_t UI_init(void);

#endif