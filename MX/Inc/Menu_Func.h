#ifndef _Menu_Func_H
#define _Menu_Func_H

#include "main.h"

#include "usart.h"
#include "Menu.h"
#include "BLE_Control.h"
#include "tim.h"
#include "RGB_LED.h"
#include "MPU6050.h"
#include "Load.h"
#include "DC_Control.h"
#include "DC_PID.h"
#include "KeyBoard.h"
#include "Tmp_Control.h"

void Menu_BLE_Normal(void* Data);
void Menu_BLE_OFF(void* Data);
void Menu_BLE_AT(void* Data);
void Menu_BLE_Remote(void* Data);

void Menu_RGB_TwinkTest(void* Data);
void Menu_MPU_Enable(void* Data);
void Menu_MPU_Disable(void* Data);
void Menu_LoadVideo(void* Data);

void Menu_Set_Tmp(void* Data);
void Menu_Enable_Tmp(void* Data);
void Menu_Disable_Tmp(void* Data);

extern uint8_t BLE_AT_Debug;
extern uint8_t BLE_RemoteFlg;
extern uint8_t BLE_RemotRec[50];
extern uint8_t BLE_RemotePt;
extern uint8_t BLE_RemoteDataFlg;
extern uint8_t RGB_TwinkTestFlg;
extern uint8_t RGB_TwinkTestIndex;

#endif