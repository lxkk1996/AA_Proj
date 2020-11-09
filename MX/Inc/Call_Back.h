#ifndef _CALL_BACK_H
#define _CALL_BACK_H

#include "stdio.h"
#include "string.h"

#include "main.h"
#include "usart.h"
#include "adc.h"
#include "tim.h"
#include "sdio.h"
#include "ff.h"
#include "fatfs.h"

#include "MPU6050.h"
#include "DC_Control.h"
#include "DC_PID.h"
#include "LCD_TFT.h"
#include "LCD_RTS.h"
#include "Key.h"
#include "Menu_Func.h"
#include "RGB_LED.h"
#include "KeyBoard.h"


uint8_t UART1_BLE_ATMode(void);
uint8_t UART2_BLE_ATMode(void);
uint8_t UART1_BLE_Remote(void);
uint8_t* BLE_GetRemoteState(void);

#endif