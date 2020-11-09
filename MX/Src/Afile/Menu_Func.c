#include "Menu_Func.h"


uint8_t BLE_UART1_Init(uint32_t BaudRate)
{
    uint8_t Res = 0;
    huart1.Instance = USART1;
    huart1.Init.BaudRate = BaudRate;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    Res =  HAL_UART_Init(&huart1);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    return Res;
}

uint8_t BLE_UART2_Init(uint32_t BaudRate)
{
    uint8_t Res = 0;
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    Res = HAL_UART_Init(&huart2);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    return Res;
}

void Menu_BLE_Normal(void* Data)
{
    uint8_t* tmp = Data;
    if((tmp[0] == 0x00) && (tmp[1] == 0x01))
    {
        if(!BLE_UART1_Init(115200))
        {
            BLE_Mode_Select(BLE_NormalMode);
            BLE_AT_Debug = 0;
        }
    }
    Menu_RetControl(&Menu_Prop);
}

uint8_t BLE_AT_Debug = 0;
uint8_t BLE_RemoteFlg = 0;
uint8_t BLE_RemotRec[50] = {0};
uint8_t BLE_RemotePt = 0;
uint8_t BLE_RemoteDataFlg = 0;

void Menu_BLE_OFF(void* Data)
{
    uint8_t* tmp = Data;
    if((tmp[0] == 0x00) && (tmp[1] == 0x01))
    {
        BLE_Power(BLE_OFF);
        BLE_AT_Debug = 0;
        BLE_RemoteFlg = 0;
    }
    Menu_RetControl(&Menu_Prop);
}

void Menu_BLE_AT(void* Data)
{
    uint8_t* tmp = Data;
    if((tmp[0] == 0x00) && (tmp[1] == 0x01))
    {
        if(!BLE_UART1_Init(38400))
        {
            if(!BLE_UART2_Init(115200))
            {
                BLE_Mode_Select(BLE_ATMode);
                BLE_AT_Debug = 1;
                BLE_RemoteFlg = 0;
            }
        }
    }
    Menu_RetControl(&Menu_Prop);
}

void Menu_BLE_Remote(void* Data)
{
    uint8_t* tmp = Data;
    if((tmp[0] == 0x00) && (tmp[1] == 0x01))
    {
        if(!BLE_UART1_Init(115200))
        {
            BLE_Mode_Select(BLE_NormalMode);
            BLE_AT_Debug = 0;
            BLE_RemoteFlg = 1;
        }
    }
    Menu_RetControl(&Menu_Prop);
}

uint8_t RGB_TwinkTestFlg = 0;
uint8_t RGB_TwinkTestIndex = 0;

void Menu_RGB_TwinkTest(void* Data)
{
    uint8_t* tmp = Data;
    if((tmp[0] == 0x00) && (tmp[1] == 0x01))
    {
        RGB_TwinkTestFlg = 1;
        RGB_TwinkTestIndex = 1;
        TIM7->ARR = 9999;
        TIM7->CNT = 0;
        HAL_TIM_Base_Start_IT(&htim7);
    }
    Menu_RetControl(&Menu_Prop);
}

uint8_t MPU_InitCNT = 5;
uint8_t MPU_DMP_Flg = 0;
uint8_t MPU_DMP_CNT = 0;
void Menu_MPU_Enable(void* Data)
{
    uint8_t* tmp = Data;
    if((tmp[0] == 0x00) && (tmp[1] == 0x01))
    {
        MPU_Init();
        while(--MPU_InitCNT)
        {
            if(!mpu_dmp_init())
            {
                break;
            }
        }
        if(MPU_InitCNT)
        {
            LCD_ShowString(&LCD_Prop, 0, 0, "Pitch:", 8, 0);
            RGB_Twinkle(RGB_G, 3);
            PWM_L = 0;
            PWM_R = 0;
            Integral_Speed = 0;
            DM_WheelControl_All(0);
            DM_PWM_Engage(Engage);
            DM_EncodeStart();
//            Beep_Warning(1);
        }
        else
        {
            RGB_Twinkle(RGB_R, 3);
//            Beep_Warning(2);
        }
    }
    Menu_RetControl(&Menu_Prop);
}

void Menu_MPU_Disable(void* Data)
{
    uint8_t* tmp = Data;
    if((tmp[0] == 0x00) && (tmp[1] == 0x01))
    {
        MPU_Init();
        DM_PWM_Engage(DisEngage);
        DM_EncodeStop();
        PWM_L = 0;
        PWM_R = 0;
        Integral_Speed = 0;
        MPU_DMP_Flg = 0;
        LCD_ShowString(&LCD_Prop, 0, 0, "             ", 8, 1);
        RGB_Twinkle(RGB_G, 3);
    }
    Menu_RetControl(&Menu_Prop);
}

extern uint8_t Show_Flg;
void Menu_LoadVideo(void* Data)
{
    uint8_t* tmp = Data;
    if((tmp[0] == 0x00) && (tmp[1] == 0x01))
    {
        if(!Show_Flg)
        {
          Load_Video();
          Keyboard_Disable(&My_Keyboard);
        }
    }
    else if((tmp[0] == 0x00) && (tmp[1] == 0x02))
    {
        Show_Flg = 0;
        LCD_ClearAll(BLACK);
        LCD_SetColor(&LCD_Prop, WHITE, BLUE);
        Load_BMP("0:/Key.bmp");
        HAL_TIM_Base_Stop_IT(&htim7);
        Menu_RetControl(&Menu_Prop);
        Keyboard_Enable(&My_Keyboard);
    }
}

void Menu_Set_Tmp(void* Data)
{
    uint8_t* tmp = Data;
    char cTmp[2] = {0};
    static char Fnum[10] = {0};
    static uint8_t Fpt = 0;
    static uint8_t dotFlg = 0;
    
    switch(tmp[0])
    {
        case 0x00:
        {
            if(tmp[1] == 0x01)
            {
                LCD_ShowString(&LCD_Prop, 0, 0, "Set Tmp:", 8, 1);
                LCD_ShowFNum(&LCD_Prop, 64, 0, TMP_Set, 4, 8, 1);
            }
            else if(tmp[1] == 0x02)
            {
                Fpt = 0;
                dotFlg = 0;
                for(uint8_t i = 0;i < 10;i++)
                {
                    Fnum[i] = 0;
                }
                LCD_ShowString(&LCD_Prop, 0, 0, "                  ", 8, 1);
                LCD_ShowString(&LCD_Prop, 8, 144, "          ", 8, 1);
                Menu_RetControl(&Menu_Prop);
            }
        }break;
        case 0x01:
        {
            if(tmp[1] >= '0' && tmp[1] <= '9')
            {
                Fnum[Fpt++] = cTmp[0] = tmp[1];
                LCD_ShowString(&LCD_Prop, Fpt*8, 144, cTmp, 8, 1);
                if(Fpt > 8) Fpt = 8;
            }
            else if(tmp[1] == '.' && !dotFlg)
            {
                dotFlg = 1;
                Fnum[Fpt++] = cTmp[0] = tmp[1];
                LCD_ShowString(&LCD_Prop, Fpt*8, 144, ".", 8, 1);
                if(Fpt > 8) Fpt = 8;
            }
        }break;
        case 0x02:
        {
            if(tmp[1] == 0x01)
            {
                TMP_Set = atof(Fnum);
                LCD_ShowFNum(&LCD_Prop, 64, 0, TMP_Set, 4, 8, 1);
            }
            else if(tmp[1] == 0x00)
            {
                if(Fpt > 0)
                {
                    LCD_ShowString(&LCD_Prop, Fpt*8, 144, " ", 8, 1);
                    Fpt--;
                    if(Fnum[Fpt] == '.')
                    {
                        dotFlg = 0;
                    }
                    Fnum[Fpt] = 0;
                }
            }
        }break;
    }
}

void Menu_Enable_Tmp(void* Data)
{
    uint8_t* tmp = Data;
    if((tmp[0] == 0x00) && (tmp[1] == 0x01))
    {
        LCD_ShowString(&LCD_Prop, 0, 0, "Tmp Set:", 8, 1);
        LCD_ShowString(&LCD_Prop, 0, 16, "Tmp Now:", 8, 1);
        LCD_ShowFNum(&LCD_Prop, 64, 0, TMP_Set, 4, 8, 1);
        LCD_ShowFNum(&LCD_Prop, 64, 16, TMP_Now, 4, 8, 1);
        TMP_StartControl();
    }
    Menu_RetControl(&Menu_Prop);
}

void Menu_Disable_Tmp(void* Data)
{
    uint8_t* tmp = Data;
    if((tmp[0] == 0x00) && (tmp[1] == 0x01))
    {
        TMP_StopControl();
        LCD_ShowString(&LCD_Prop, 0, 0, "                   ", 8, 1);
        LCD_ShowString(&LCD_Prop, 0, 16, "                 ", 8, 1);
    }
    Menu_RetControl(&Menu_Prop);
}