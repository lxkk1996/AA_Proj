#include "main.h"
#include "stdint.h"
#include "stdio.h"
#include "LCD_TFT.h"
#include "LCD_Font.h"

LCD_Property_Typedef    LCD_Prop = {0};

void LCD_HWReset(void)
{
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
    LCD_Delay(5);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    LCD_Delay(120);
}

void LCD_BackLight(uint8_t cmd)
{
    if(cmd) GPIOB->BSRR = LCD_BL_Pin;
    else GPIOB->BSRR = (uint32_t)LCD_BL_Pin << 16U;
}

void LCD_SWReset(void)
{
    LCD_Cmd(0x01);
    LCD_Delay(120);
}

void LCD_Cmd(uint16_t Cmd)
{
    LCD->CMD = Cmd;
}

void LCD_WriteData(uint16_t Data)
{
    LCD->DATA = Data;
}

void LCD_WriteBulk_DATA(uint16_t* DataBuff, uint32_t BuffSize)
{
    LCD_WriteData(*(DataBuff++));
    while(--BuffSize)
    {
        LCD_WriteData(*(DataBuff++));
    }
}

void LCD_WriteBulk_Color(uint16_t Color, uint32_t ColorNum)
{
    LCD_WriteData(Color);
    while(--ColorNum)
    {
        LCD_WriteData(Color);
    }
}

uint16_t LCD_ReadData(void)
{
    return LCD->DATA;
}

void LCD_ReadData_Bulk(uint16_t* DataBuff, uint32_t BuffSize)
{
    *DataBuff++ = LCD_ReadData();
    while(--BuffSize)
    {
        *DataBuff++ = LCD_ReadData();
    }
}

void LCD_ReadID(LCD_Property_Typedef* pLcd_Prop)
{
    uint16_t ID_Buff[4] = {0};
    LCD_Cmd(0XD3);
    LCD_ReadData_Bulk(ID_Buff, 4);
    pLcd_Prop->ID = (ID_Buff[2] << 8) | ID_Buff[3];
}

void LCD_ReadDisplay_Status(LCD_Property_Typedef* pLcd_Prop)
{
    uint16_t Status_Buff[5] = {0};
    LCD_Cmd(0X09);
    LCD_ReadData_Bulk(Status_Buff, 5);
    pLcd_Prop->Display_Status.Status = ((uint32_t)Status_Buff[1] << 24) | ((uint32_t)Status_Buff[2] << 16) | ((uint32_t)Status_Buff[3] << 8) | ((uint32_t)Status_Buff[4]);
    if(pLcd_Prop->Display_Status.Row_Col_Exchage)
    {
        pLcd_Prop->Max_H = 239;
        pLcd_Prop->Max_W = 319;
    }
    else
    {
        pLcd_Prop->Max_H = 319;
        pLcd_Prop->Max_W = 239;
    }
}

void LCD_MemoryAccess(uint16_t Data)
{
    LCD_Cmd(0x36);
    LCD_WriteData(Data);
}

void LCD_FilpRotate(LCD_Property_Typedef* pLcd_Prop, LCD_FR_ENUM hv)
{
    uint16_t tmp = 0x00;
    tmp |= (pLcd_Prop->Display_Status.Row_Col_Exchage ^ ((hv & LCD_RRA) >> 2))      ?   0x20 : 0x00;
    if(tmp & 0x20)
    {
        tmp |= (pLcd_Prop->Display_Status.Row_Addr_Order ^ ((hv & LCD_FVER) >> 1))  ?   0x80 : 0x00;
        tmp |= (pLcd_Prop->Display_Status.Col_Addr_Order ^ (hv & LCD_FHOR))         ?   0x40 : 0x00;
    }
    else
    {
        tmp |= (pLcd_Prop->Display_Status.Row_Addr_Order ^ (hv & LCD_FHOR))         ?   0x80 : 0x00;
        tmp |= (pLcd_Prop->Display_Status.Col_Addr_Order ^ ((hv & LCD_FVER) >> 1))  ?   0x40 : 0x00;
    }
    tmp |= pLcd_Prop->Display_Status.Vertical_Refresh       ?   0x10 : 0x00;
    tmp |= pLcd_Prop->Display_Status.RGB_Order              ?   0x08 : 0x00;
    tmp |= pLcd_Prop->Display_Status.Horizontal_Refresh     ?   0x04 : 0x00;
    LCD_MemoryAccess(tmp);
    LCD_ReadDisplay_Status(pLcd_Prop);
}

void LCD_Set_Cursor(uint16_t SCol, uint16_t SRow)
{
    LCD_Cmd(0x2A);
    LCD_WriteData(SCol >> 8);
    LCD_WriteData(SCol & 0x00FF);
    LCD_Cmd(0x2B);
    LCD_WriteData(SRow >> 8);
    LCD_WriteData(SRow & 0x00FF);
}

void LCD_Reset_Cursor()
{
    if(LCD_Prop.Display_Status.Row_Col_Exchage)
    {
        LCD_Cmd(0x2A);
        LCD_WriteData(0x00);
        LCD_WriteData(0x00);
        LCD_WriteData(0x13F >> 8);
        LCD_WriteData(0x13F & 0x00FF);
        LCD_Cmd(0x2B);
        LCD_WriteData(0x00);
        LCD_WriteData(0x00);
        LCD_WriteData(0xEF >> 8);
        LCD_WriteData(0xEF & 0x00FF);
    }
    else
    {
        LCD_Cmd(0x2A);
        LCD_WriteData(0x00);
        LCD_WriteData(0x00);
        LCD_WriteData(0xEF >> 8);
        LCD_WriteData(0xEF & 0x00FF);
        LCD_Cmd(0x2B);
        LCD_WriteData(0x00);
        LCD_WriteData(0x00);
        LCD_WriteData(0x13F >> 8);
        LCD_WriteData(0x13F & 0x00FF);
    }
}

uint8_t LCD_SetArea(LCD_Property_Typedef* lcd, uint16_t SCol, uint16_t SRow, uint16_t ECol, uint16_t ERow)
{
    uint8_t Res = 0;
    if(SCol > ECol) {SCol = 0; Res |= 0x01;}
    if(SRow > ERow) {SRow = 0; Res |= 0x02;}
    
    if(lcd->Display_Status.Row_Col_Exchage)
    {
        if(ECol >= 320) {ECol = 319; Res |= 0x04;};
        if(ERow >= 240) {ERow = 239; Res |= 0x08;};
    }
    else
    {
        if(ECol >= 240) {ECol = 239; Res |= 0x04;};
        if(ERow >= 320) {ERow = 319; Res |= 0x08;};
    }
    LCD_Cmd(0x2A);
    LCD_WriteData(SCol >> 8);
    LCD_WriteData(SCol & 0x00FF);
    LCD_WriteData(ECol >> 8);
    LCD_WriteData(ECol & 0x00FF);
    LCD_Cmd(0x2B);
    LCD_WriteData(SRow >> 8);
    LCD_WriteData(SRow & 0x00FF);
    LCD_WriteData(ERow >> 8);
    LCD_WriteData(ERow & 0x00FF);
    return Res;
}

void LCD_WritePrepare()
{
    LCD_Cmd(0x2C);
}

void LCD_ShowPic(void* pdata)
{
    uint16_t * Pic = pdata;
    LCD_Reset_Cursor();
    LCD_Cmd(0x2C);
    for(int i=0; i < 76800; i++)
	{
		LCD->DATA = *Pic++;
	}
}

void LCD_ShowBit(uint8_t* BitBuff, uint32_t Bitsize)
{
    uint8_t* Tmp = BitBuff;
    uint32_t j = 0;
    for(;;)
    {
        do
        {
            if(*Tmp & 0x80)
                LCD_WriteData(LCD_Prop.Pixel_Color);
            else
                LCD_WriteData(LCD_Prop.Back_Color);
            *Tmp <<= 1;
            j++;
            if(j == Bitsize)break;
        }while(j % 8);
        Tmp++;
        if(j == Bitsize)break;
    }
}

void LCD_ClearAll(uint16_t Color)
{
    LCD_Reset_Cursor();
    LCD_Cmd(0x2C);
    LCD_WriteBulk_Color(Color, 76800);
}


void LCD_ShowString(LCD_Property_Typedef* lcd, uint16_t SCol, uint16_t SRow, const char *String, uint8_t FontWidth, uint8_t WriteMode)
{
    uint8_t Index=0;
    const uint8_t *Font = NULL;
    uint8_t Font_Data;
    uint8_t Font_Sec;
    uint8_t FontSec_Index = 0;;
    uint16_t CSCol,CECol,CSRow, CERow;
    uint16_t ColTmp, Row_Tmp;

    switch(FontWidth)
    {
        case FONT_WIDTH_8:
        {
            Font = LCD_ASC8;
            Font_Sec = 16;
        }break;
        case FONT_WIDTH_16:
        {
            Font = LCD_ASC16;
            Font_Sec = 32;
        }break;
    }
    while((String[Index]!=0) && (String[Index]>=0x20) && (String[Index]<=0x7F))
    {
        CSCol = SCol + (FontWidth -1)*Index;
        ColTmp = CSCol;
        CSRow = SRow;
        Row_Tmp = CSRow;
        CECol = SCol + (FontWidth -1)*(Index + 1);
        CERow = SRow + 15;
        LCD_SetArea(lcd, CSCol, CSRow, CECol, CERow);
        LCD_Cmd(0x2C);
        for(uint8_t Sec = 0; Sec < Font_Sec; Sec++)
        {
            Font_Data = Font[Font_Sec*(String[Index]-0x20) + Sec];
            for(uint8_t Bit = 0; Bit < 8; Bit++)
            {
                if(WriteMode)
                {
                    if(Font_Data & 0x80)
                    {
                        LCD_WriteData(lcd->Pixel_Color);
                    }
                    else
                    {
                        LCD_WriteData(lcd->Back_Color);
                    }
                }
                else if(Font_Data & 0x80)
                {
                    LCD_Set_Cursor(ColTmp, Row_Tmp);
                    LCD_Cmd(0x2C);
                    LCD_WriteData(lcd->Pixel_Color);
                }
                Font_Data <<= 1;
                ColTmp++;
            }
            if(FontWidth/8 == ++FontSec_Index)
            {
                FontSec_Index = 0;
                ColTmp = CSCol;
                Row_Tmp++;
            }
        }
        Index++;
    }
    LCD_Reset_Cursor();
}

void LCD_ShowFNum(LCD_Property_Typedef* lcd, uint16_t SCol, uint16_t SRow, double FNum, uint8_t Precision ,uint8_t FontWidth, uint8_t WriteMode)
{
    char String[10]={0x20};
    uint8_t ShiftFlg = 0;
    if(FNum < 0)
    {
        Precision += 3;
    }
    else
    {
        Precision += 2;
        ShiftFlg = 1;
    }
    snprintf(String + ShiftFlg, Precision, "%lf", FNum);
    LCD_ShowString(lcd, SCol, SRow, String, FontWidth, WriteMode);
}

void LCD_ShowNum(LCD_Property_Typedef* lcd, uint16_t SCol, uint16_t SRow, int Num, uint8_t FontWidth, uint8_t WriteMode)
{
    char String[10]={0x20};
    uint8_t ShiftFlg = 0;
    if(Num >= 0)
    {
        ShiftFlg = 1;
    }
    sprintf(String + ShiftFlg,"%d",Num);
    LCD_ShowString(lcd, SCol, SRow, String, FontWidth, WriteMode);
}

void LCD_ShowPoint(LCD_Property_Typedef* lcd, uint16_t CCol, uint16_t CRow)
{
    LCD_Set_Cursor(CCol, CRow);
    LCD_Cmd(0x2C);
    LCD_WriteData(lcd->Pixel_Color);
}

void LCD_ShowCorss(LCD_Property_Typedef* lcd, uint16_t CCol, uint16_t CRow, uint16_t Radius)
{
    LCD_Set_Cursor(CCol - Radius, CRow);
    LCD_Cmd(0x2C);
    LCD_WriteBulk_Color(lcd->Pixel_Color, 2 * Radius + 1);
    for(uint16_t i = 1; i <= Radius; i++)
    {
        LCD_ShowPoint(lcd, CCol, CRow + i);
    }
    for(uint16_t i = 1; i <= Radius; i++)
    {
        LCD_ShowPoint(lcd, CCol, CRow - i);
    }
}

//void LCD_ShowLine(LCD_Property_Typedef* lcd, uint16_t SCol, uint16_t SRow, uint16_t Lin)

void LCD_PropSet_PixelColor(LCD_Property_Typedef* lcd, uint16_t Color)
{
    lcd->Pixel_Color = Color;
}

void LCD_PropSet_BackColor(LCD_Property_Typedef* lcd, uint16_t Color)
{
    lcd->Back_Color = Color;
}

void LCD_SetColor(LCD_Property_Typedef* lcd, uint16_t pixColor, uint16_t BackColor)
{
    lcd->Pixel_Color = pixColor;
    lcd->Back_Color = BackColor;
}

uint8_t LCD_Init(LCD_Property_Typedef* lcd)
{
    uint8_t TryInit_Cnt = 5;
    uint8_t Res = 0;
    
    LCD_Cmd(CMD_NOP);
    LCD_ReadID(&LCD_Prop);
    while(LCD_Prop.ID != 0x9341 && TryInit_Cnt)
    {
        TryInit_Cnt--;
        LCD_HWReset();
        LCD_Cmd(CMD_NOP);
        LCD_ReadID(&LCD_Prop);
    }
    if(!TryInit_Cnt)
    {
        Res = 1;
        return Res;
    }
    LCD_Cmd(0xCF);
    LCD_WriteData(0x00); 
    LCD_WriteData(0xC1); 
    LCD_WriteData(0X30); 
    LCD_Cmd(0xED);  
    LCD_WriteData(0x64); 
    LCD_WriteData(0x03); 
    LCD_WriteData(0X12); 
    LCD_WriteData(0X81); 
    LCD_Cmd(0xE8);  
    LCD_WriteData(0x85); 
    LCD_WriteData(0x10); 
    LCD_WriteData(0x7A); 
    LCD_Cmd(0xCB);  
    LCD_WriteData(0x39); 
    LCD_WriteData(0x2C); 
    LCD_WriteData(0x00); 
    LCD_WriteData(0x34); 
    LCD_WriteData(0x02); 
    LCD_Cmd(0xF7);  
    LCD_WriteData(0x20); 
    LCD_Cmd(0xEA);  
    LCD_WriteData(0x00); 
    LCD_WriteData(0x00); 
    LCD_Cmd(0xC0);    //Power control 
    LCD_WriteData(0x1B);   //VRH[5:0] 
    LCD_Cmd(0xC1);    //Power control 
    LCD_WriteData(0x01);   //SAP[2:0];BT[3:0] 
    LCD_Cmd(0xC5);    //VCM control 
    LCD_WriteData(0x30); 	 //3F
    LCD_WriteData(0x30); 	 //3C
    LCD_Cmd(0xC7);    //VCM control2 
    LCD_WriteData(0XB7); 
    LCD_Cmd(0x36);    // Memory Access Control 
    LCD_WriteData(0x68);
    LCD_Cmd(0x3A);   //pixel Format
    LCD_WriteData(0x55); 
    LCD_Cmd(0xB1);   
    LCD_WriteData(0x00);   
    LCD_WriteData(0x1A); 
    LCD_Cmd(0xB6);    // Display Function Control 
    LCD_WriteData(0x0A); 
    LCD_WriteData(0xA2); 
    LCD_Cmd(0xF2);    // 3Gamma Function Disable 
    LCD_WriteData(0x00); 
    LCD_Cmd(0x26);    //Gamma curve selected 
    LCD_WriteData(0x01); 
    LCD_Cmd(0xE0);    //Set Gamma 
    LCD_WriteData(0x0F); 
    LCD_WriteData(0x2A); 
    LCD_WriteData(0x28); 
    LCD_WriteData(0x08); 
    LCD_WriteData(0x0E); 
    LCD_WriteData(0x08); 
    LCD_WriteData(0x54); 
    LCD_WriteData(0XA9); 
    LCD_WriteData(0x43); 
    LCD_WriteData(0x0A); 
    LCD_WriteData(0x0F); 
    LCD_WriteData(0x00); 
    LCD_WriteData(0x00); 
    LCD_WriteData(0x00); 
    LCD_WriteData(0x00); 		 
    LCD_Cmd(0XE1);    //Set Gamma 
    LCD_WriteData(0x00); 
    LCD_WriteData(0x15); 
    LCD_WriteData(0x17); 
    LCD_WriteData(0x07); 
    LCD_WriteData(0x11); 
    LCD_WriteData(0x06); 
    LCD_WriteData(0x2B); 
    LCD_WriteData(0x56); 
    LCD_WriteData(0x3C); 
    LCD_WriteData(0x05); 
    LCD_WriteData(0x10); 
    LCD_WriteData(0x0F); 
    LCD_WriteData(0x3F); 
    LCD_WriteData(0x3F); 
    LCD_WriteData(0x0F); 
    LCD_Cmd(0x2B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0xef);
    LCD_Cmd(0x2A); 
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x01);
    LCD_WriteData(0x3f);
    LCD_Cmd(0x11); //Exit Sleep
    HAL_Delay(120);
    LCD_Cmd(0x29); //display on
    
    LCD_ReadDisplay_Status(&LCD_Prop);
    LCD_BackLight(LCD_ON);
    LCD_ShowPic(&gImage_BadApple);
    HAL_Delay(1500);
    LCD_ClearAll(BLACK);
    
    return Res;
}

void LCD_FuncTest(void)
{
    LCD_ShowCorss(&LCD_Prop, 9, 9, 9);
    LCD_ShowCorss(&LCD_Prop, 309, 9, 9);
    LCD_ShowCorss(&LCD_Prop, 9, 229, 9);
    LCD_ShowCorss(&LCD_Prop, 309, 229, 9);
}