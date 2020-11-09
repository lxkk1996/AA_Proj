#include "Load.h"

int FRes;
uint8_t SD_DataBuff[10240];
uint32_t DataCnt;
uint32_t tmpoff;
extern uint8_t Show_Flg;
void Load_Video()
{
    FRes = f_open(&SDFile,"0:/BadApple_f60.bin",FA_READ | FA_OPEN_EXISTING);
    if(!FRes)
    {
        LCD_SetColor(&LCD_Prop, BLACK, WHITE);
        LCD_Reset_Cursor();
        LCD_Set_Cursor(0, 0);
        LCD_Cmd(0x2C);
        f_read(&SDFile, SD_DataBuff, 9600, &DataCnt);
        TIM7->ARR = 1666;
        HAL_TIM_Base_Start_IT(&htim7);
        Show_Flg = 1;
    }
}

uint8_t Load_BMP(const char* Str)
{
    uint32_t dc;
    BMP_HEARD bh;
    if(f_open(&SDFile, Str, FA_READ | FA_OPEN_EXISTING))
        return 1;
    f_read(&SDFile,&bh, 54, &dc);
    if(BMP_Chack(&bh))
        return 2;
    LCD_FilpRotate(&LCD_Prop, LCD_FHOR);
    LCD_SetArea(&LCD_Prop, 0, 0, bh.ih.iWidth, bh.ih.iHeight);
    LCD_WritePrepare();
    if(BMP_GetImageDepth(&bh) == 1)
    {
        f_lseek(&SDFile, bh.fh.fOffSet);
        //BMP4×Ö½Ú¶ÔÆëËã·¨
        do
        {
            f_read(&SDFile, SD_DataBuff, (bh.ih.iWidth + 31)/32*4, &dc);
            LCD_ShowBit(SD_DataBuff, bh.ih.iWidth);
//            LCD_ShowBit(SD_DataBuff, )
        }while(--bh.ih.iHeight);
    }
    else
    {
        
    }
    f_close(&SDFile);
    LCD_FilpRotate(&LCD_Prop, LCD_FHOR);
    return 0;
}