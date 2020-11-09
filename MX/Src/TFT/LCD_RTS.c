
#include "LCD_RTS.h"


uint8_t RTS_RDX_CMD = 0x90;
uint8_t RTS_RDY_CMD = 0xD0;
uint8_t RTS_XRecData[2];
uint8_t RTS_YRecData[2];
uint16_t RTS_XData, RTS_YData;

uint16_t RTS_CalibData[4][2];
uint16_t RTS_CalibXData[12], RTS_CalibYData[12];

double RTS_DefaultCalibXAB[2] = {0.0880540065, -21.81890226005};
double RTS_DefaultCalibYAB[2] = {-0.06739163731, 255.4849134630};

double RTS_CalibXAB[2] = {0, 0};
double RTS_CalibYAB[2] = {0, 0};

volatile uint8_t RTS_CalibFlg = 1;
uint8_t RTS_CalibIndex = 0;

uint16_t RTS_ReadData(uint8_t CMD, uint8_t* pData)
{
    uint16_t Temp = 0;
    
    GPIOA->BSRR = (uint32_t)CS_Pin << 16U;
    HAL_SPI_Transmit(&hspi1, &CMD, 1, 100);
//    Tim_Delay(6); //转换数据延时 是否需要看情况
    memset(pData, 0x00, 2);//Stm32 SPI 接收驱动有Bug 在全双工接收数据同时会发送接收数据指针的所指向的数据
    HAL_SPI_Receive(&hspi1, pData, 2, 100);
    GPIOA->BSRR = CS_Pin;
    Temp = ((uint16_t)(pData[0]) << 8) | pData[1];
    Temp >>= 3;
    return Temp;
}

void RTS_ReadXY_AvgData()
{
    uint8_t SwapX, SwapY;
    uint16_t SwapTmp;
    for(uint8_t i = 0; i < 12; i++)
    {
        RTS_CalibXData[i] = RTS_ReadData(RTS_RDX_CMD,  (uint8_t *)(&RTS_CalibXData[i]));
        RTS_CalibYData[i] = RTS_ReadData(RTS_RDY_CMD,  (uint8_t *)(&RTS_CalibYData[i]));
        //加上一些阈值处理
    }
    for(uint8_t i = 0; i < 11; i++)
    {
        SwapX = i;
        SwapY = i;
        for(uint8_t j = i + 1; j < 12; j++)
        {
            if(RTS_CalibXData[SwapX] > RTS_CalibXData[j])
                SwapX = j;
            if(RTS_CalibYData[SwapY] > RTS_CalibYData[j])
                SwapY = j;
        }
        SwapTmp = RTS_CalibXData[SwapX];
        RTS_CalibXData[SwapX] = RTS_CalibXData[i];
        RTS_CalibXData[i] = SwapTmp;
        SwapTmp = RTS_CalibYData[SwapY];
        RTS_CalibYData[SwapY] = RTS_CalibYData[i];
        RTS_CalibYData[i] = SwapTmp;
    }
    RTS_CalibXData[0] = 0;
    RTS_CalibYData[0] = 0;
    for(uint8_t i = 3; i < 9; i++)
    {
        RTS_CalibXData[0] += RTS_CalibXData[i];
        RTS_CalibYData[0] += RTS_CalibYData[i];
    }
    RTS_CalibXData[0] /= 6;
    RTS_CalibYData[0] /= 6;
    RTS_XData = (uint16_t)(RTS_CalibXAB[0] * RTS_CalibXData[0] + RTS_CalibXAB[1]);
    RTS_YData = (uint16_t)(RTS_CalibYAB[0] * RTS_CalibYData[0] + RTS_CalibYAB[1]);
}

void RTS_CalibPrepare(void)
{
    RTS_CalibFlg = 0;
    RTS_CalibIndex = 0;
    LCD_ClearAll(BLUE);
    LCD_ShowString(&LCD_Prop, 100, 100, "LCD Calibrate...", 8, 0);
    LCD_ShowCorss(&LCD_Prop, 9, 9, 9);
    HAL_TIM_Base_Start_IT(&htim5);
}

void RTS_Calibrate()
{
//    uint16_t TmpX, TmpY;
    uint8_t SwapX, SwapY;
    uint16_t SwapTmp;
    for(uint8_t i = 0; i < 12; i++)
    {
        RTS_CalibXData[i] = RTS_ReadData(RTS_RDX_CMD,  (uint8_t *)(&RTS_CalibXData[i]));
        RTS_CalibYData[i] = RTS_ReadData(RTS_RDY_CMD,  (uint8_t *)(&RTS_CalibYData[i]));
        //加上一些阈值处理
    }
    for(uint8_t i = 0; i < 11; i++)
    {
        SwapX = i;
        SwapY = i;
        for(uint8_t j = i + 1; j < 12; j++)
        {
            if(RTS_CalibXData[SwapX] > RTS_CalibXData[j])
                SwapX = j;
            if(RTS_CalibYData[SwapY] > RTS_CalibYData[j])
                SwapY = j;
        }
        SwapTmp = RTS_CalibXData[SwapX];
        RTS_CalibXData[SwapX] = RTS_CalibXData[i];
        RTS_CalibXData[i] = SwapTmp;
        SwapTmp = RTS_CalibYData[SwapY];
        RTS_CalibYData[SwapY] = RTS_CalibYData[i];
        RTS_CalibYData[i] = SwapTmp;
    }
    RTS_CalibXData[0] = 0;
    RTS_CalibYData[0] = 0;
    for(uint8_t i = 1; i < 11; i++)
    {
        RTS_CalibXData[0] += RTS_CalibXData[i];
        RTS_CalibYData[0] += RTS_CalibYData[i];
    }
    RTS_CalibXData[0] /= 10;
    RTS_CalibYData[0] /= 10;
    switch(RTS_CalibIndex)
    {
        case 0:
        {
            RTS_CalibData[RTS_CalibIndex][0] = RTS_CalibXData[0];
            RTS_CalibData[RTS_CalibIndex][1] = RTS_CalibYData[0];
            LCD_Prop.Pixel_Color = BLUE;
            LCD_ShowCorss(&LCD_Prop, 9, 9, 9);
            LCD_Prop.Pixel_Color = WHITE;
            LCD_ShowCorss(&LCD_Prop, 309, 9, 9);
            RTS_CalibIndex++;
        }break;
        case 1:
        {
            RTS_CalibData[RTS_CalibIndex][0] = RTS_CalibXData[0];
            RTS_CalibData[RTS_CalibIndex][1] = RTS_CalibYData[0];
            LCD_Prop.Pixel_Color = BLUE;
            LCD_ShowCorss(&LCD_Prop, 309, 9, 9);
            LCD_Prop.Pixel_Color = WHITE;
            LCD_ShowCorss(&LCD_Prop, 9, 229, 9);
            RTS_CalibIndex++;
        }break;
        case 2:
        {
            RTS_CalibData[RTS_CalibIndex][0] = RTS_CalibXData[0];
            RTS_CalibData[RTS_CalibIndex][1] = RTS_CalibYData[0];
            LCD_Prop.Pixel_Color = BLUE;
            LCD_ShowCorss(&LCD_Prop, 9, 229, 9);
            LCD_Prop.Pixel_Color = WHITE;
            LCD_ShowCorss(&LCD_Prop, 309, 229, 9);
            RTS_CalibIndex++;
        }break;
        case 3:
        {
            RTS_CalibData[RTS_CalibIndex][0] = RTS_CalibXData[0];
            RTS_CalibData[RTS_CalibIndex][1] = RTS_CalibYData[0];
            
            RTS_CalibXAB[0] = 300.0 / (((RTS_CalibData[1][0] + RTS_CalibData[3][0])/2.0) - ((RTS_CalibData[0][0] + RTS_CalibData[2][0])/2.0));
            RTS_CalibYAB[0] = 220.0 / (((RTS_CalibData[2][1] + RTS_CalibData[3][1])/2.0) - ((RTS_CalibData[0][1] + RTS_CalibData[1][1])/2.0));
            RTS_CalibXAB[1] = 9 - RTS_CalibXAB[0] * ((RTS_CalibData[0][0] + RTS_CalibData[2][0])/2.0);
            RTS_CalibYAB[1] = 9 - RTS_CalibYAB[0] * ((RTS_CalibData[0][1] + RTS_CalibData[1][1])/2.0);
            LCD_Prop.Pixel_Color = BLUE;
            LCD_ShowCorss(&LCD_Prop, 309, 229, 9);
            LCD_Prop.Pixel_Color = WHITE;
            RTS_CalibIndex = 0;
            RTS_CalibFlg = 1;
        }break;
    }
}

void RTS_SetOrientation(uint8_t Orientation)
{
    if(Orientation)
    {
        RTS_RDX_CMD = 0x90;
        RTS_RDY_CMD = 0xD0;
    }
    else
    {
        RTS_RDX_CMD = 0xD0;
        RTS_RDY_CMD = 0x90;
    }
}

