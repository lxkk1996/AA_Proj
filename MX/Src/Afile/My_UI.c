#include "My_UI.h"

char buff[256];

uint8_t UI_init(void)
{
    uint8_t SDflg = 0;
    int64_t SaveCalibData[4];
    uint32_t br;
    
    
    
    LCD_SetColor(&LCD_Prop, WHITE, BLACK);
    if(LCD_Init(&LCD_Prop))
    {
        printf("No LCD Detect!\n");
        OLED_Init(&hi2c2);
        return 1;
    }
    if(f_mount(&SDFatFS, "0:", 1))
    {
        SDflg = 1;
        printf("No SD Card Detect!\n");
        LCD_ShowString(&LCD_Prop, 0, 0, "No SD Card!", 8 ,0);
    }
    else
    {
        if(!f_open(&SDFile, "0:/CalibData.bin", FA_READ | FA_WRITE | FA_OPEN_ALWAYS))
        {
            if(f_size(&SDFile) == 32)
            {
                f_read(&SDFile, SaveCalibData, 32, &br);
                RTS_CalibFlg = 1;
                RTS_CalibXAB[0] = (double)SaveCalibData[0]/1000000.0;
                RTS_CalibXAB[1] = (double)SaveCalibData[1]/1000000.0;
                RTS_CalibYAB[0] = (double)SaveCalibData[2]/1000000.0;
                RTS_CalibYAB[1] = (double)SaveCalibData[3]/1000000.0;
                f_close(&SDFile);
            }
            else
            {
                LCD_SetColor(&LCD_Prop, WHITE, BLUE);
                RTS_CalibPrepare();
                while(!RTS_CalibFlg);
                f_lseek(&SDFile, 0);
                SaveCalibData[0] = (int64_t)(RTS_CalibXAB[0] * 1000000);
                SaveCalibData[1] = (int64_t)(RTS_CalibXAB[1] * 1000000);
                SaveCalibData[2] = (int64_t)(RTS_CalibYAB[0] * 1000000);
                SaveCalibData[3] = (int64_t)(RTS_CalibYAB[1] * 1000000);
                SDIO_WriteDiv();
                f_write(&SDFile, SaveCalibData, 32, &br);
                f_close(&SDFile);
                SDIO_ReadDiv();
            }
        }
        else
        {
            SDflg = 1;
        }
    }
    if(SDflg)
    {
        RTS_CalibFlg = 1;
        RTS_CalibXAB[0] = RTS_DefaultCalibXAB[0];
        RTS_CalibXAB[1] = RTS_DefaultCalibXAB[1];
        RTS_CalibYAB[0] = RTS_DefaultCalibYAB[0];
        RTS_CalibYAB[1] = RTS_DefaultCalibYAB[1];
    }
    LCD_SetColor(&LCD_Prop, WHITE, BLACK);
    Menu_Init(&Menu_Prop, 0);
    Key_init();
    if(!SDflg)
    {
        LCD_SetColor(&LCD_Prop, WHITE, BLUE);
        if(!Load_BMP("0:/Key.bmp"))
        {
            Keyboard_Enable(&My_Keyboard);
        }
        else
        {
            Keyboard_Disable(&My_Keyboard);
        }
    }
    LCD_SetColor(&LCD_Prop, WHITE, BLACK);
    
//    strcpy(buff, "/");
//    scan_files(buff);
    
    return 0;
}