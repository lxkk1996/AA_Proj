#ifndef __OLED_H
#define __OLED_H

#include "i2c.h"

#define OLED_ADDR 0x78//it can be 0x7A
#define OLED_PRECMD 0x00
#define OLED_PREDATA 0x40
#define OLED_H_DATA 0xFF
#define OLED_L_DATA 0x00
#define OLED_Time_Out 100
#define OLED_Picture_Size 1024
typedef enum
{
  Horizontal_ADDR_Mode,
  Vertical_ADDR_Mode,
  Page_ADDR_Mode
}ADDR_Mode_TypeDef;

typedef enum
{
  Reset,
  Set
}Set_TypeDef;
typedef Set_TypeDef Switch_TypeDef;
typedef Set_TypeDef Remap_TypeDef;
typedef Set_TypeDef Direction_TypeDef;

void OLED_Transmit_DATA(I2C_HandleTypeDef *hi2c, uint8_t *OLED_DATA, uint16_t Data_Size);

void OLED_Test();
void OLED_DisableScroll(I2C_HandleTypeDef *hi2c);
void OLED_EnableScroll(I2C_HandleTypeDef *hi2c);
void OLED_SetArea(I2C_HandleTypeDef *hi2c);
    
void OLED_Init(I2C_HandleTypeDef *hi2c);
void OLED_Set_Clock(I2C_HandleTypeDef *hi2c, uint8_t Fosc, uint8_t DCLK);
void OLED_Set_PCP(I2C_HandleTypeDef *hi2c, uint8_t Phase_1, uint8_t Phase_2);
void OLED_Set_VCOMH(I2C_HandleTypeDef *hi2c, uint8_t VCOMH);
void OLED_Set_Start_Line(I2C_HandleTypeDef *hi2c,uint8_t DSL);
void OLED_Set_SEG_Remap(I2C_HandleTypeDef *hi2c, Remap_TypeDef Remap);
void OLED_Set_Multiplex_Ratio(I2C_HandleTypeDef *hi2c, uint8_t Ratio);
void OLED_Set_COM_OD(I2C_HandleTypeDef *hi2c, Direction_TypeDef Direction);
void OLED_Set_Display_Offset(I2C_HandleTypeDef *hi2c, uint8_t Offset);
void OLED_Set_COM_PHC(I2C_HandleTypeDef *hi2c, uint8_t PHC);
void OLED_Set_Charge_Pump(I2C_HandleTypeDef *hi2c, Switch_TypeDef Switch);
void OLED_Set_ADDR_Mode(I2C_HandleTypeDef *hi2c, ADDR_Mode_TypeDef Mode);
void OLED_Set_StartPage(I2C_HandleTypeDef *hi2c, uint8_t Page);
void OLED_Set_StartColumn(I2C_HandleTypeDef *hi2c, uint8_t Column);
void OLED_Set_Contrast(I2C_HandleTypeDef *hi2c, uint8_t Contrast);
void OLED_Display_On(I2C_HandleTypeDef *hi2c, Switch_TypeDef Switch);
void OLED_Set_Entire_On(I2C_HandleTypeDef *hi2c, Set_TypeDef Set);
void OLED_Set_Inverse_On(I2C_HandleTypeDef *hi2c, Set_TypeDef Set);
void OLED_Set_StartEnd_Page(I2C_HandleTypeDef *hi2c, uint8_t Page_S, uint8_t Page_E);
void OLED_Set_StartEnd_Column(I2C_HandleTypeDef *hi2c, uint8_t Column_S, uint8_t Column_E);
void OLED_Clear(I2C_HandleTypeDef *hi2c);
void OLED_Clear_Page(I2C_HandleTypeDef *hi2c, uint8_t Page);
void OLED_Clear_Rec(I2C_HandleTypeDef *hi2c, uint8_t Page_S ,uint8_t Page_E, uint8_t Col_S, uint8_t Col_E);
void OLED_Picture(I2C_HandleTypeDef *hi2c, uint8_t *Picture);
void OLED_String(I2C_HandleTypeDef *hi2c, uint8_t Page, uint8_t Column, char *String);
void OLED_String16(I2C_HandleTypeDef *hi2c, uint8_t Row, uint8_t Col, char *String);
void OLED_String32(I2C_HandleTypeDef *hi2c, uint8_t Row, uint8_t Col, char *String);
void OLED_Num(I2C_HandleTypeDef *hi2c, uint8_t Row, uint8_t Col, int32_t Num);
void OLED_Progress_Bar(I2C_HandleTypeDef *hi2c, uint8_t Page, uint8_t Num);
void OLED_FNum(I2C_HandleTypeDef *hi2c, uint8_t Row, uint8_t Col, double Num, uint8_t Precision);

#endif