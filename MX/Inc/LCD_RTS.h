#ifndef _LCD_RTS_H
#define _LCD_RTS_H

#include "main.h"
#include "tim.h"
#include "string.h"
#include "gpio.h"
#include "spi.h"
#include "LCD_TFT.h"
#include "Tim_Delay.h"

extern uint8_t RTS_RDX_CMD;
extern uint8_t RTS_RDY_CMD;
extern uint8_t RTS_XRecData[];
extern uint8_t RTS_YRecData[];
extern uint16_t RTS_XData, RTS_YData;
extern volatile uint8_t RTS_CalibFlg;
extern uint8_t RTS_CalibIndex;

extern double RTS_DefaultCalibXAB[2];
extern double RTS_DefaultCalibYAB[2];

extern double RTS_CalibXAB[2];
extern double RTS_CalibYAB[2];

uint16_t RTS_ReadData(uint8_t CMD,uint8_t* pData);
void RTS_ReadXY_AvgData();
void RTS_CalibPrepare(void);
void RTS_Calibrate();
#endif