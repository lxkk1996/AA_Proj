#ifndef _LOAD_BMP_H
#define _LOAD_BMP_H

#include "stdio.h"


#include "main.h"
#include "tim.h"
#include "fatfs.h"
#include "ff.h"
#include "sdio.h"

#include "LCD_TFT.h"
#include "BMP_Decode.h"

void Load_Video();
uint8_t Load_BMP(const char* Str);
#endif