#include "Call_Back.h"

uint8_t Touch_Down_Flg = 0;
uint8_t Show_Flg = 0;
uint8_t Encode_GetCNT = 0;
uint8_t Encode_Read_Flg = 0;

uint8_t CC[3] = {0};
extern uint8_t MPU_DMP_Flg;
extern uint8_t MPU_DMP_CNT;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == LCD_PEN_INT_Pin)
    {
        HAL_NVIC_DisableIRQ(EXTI4_IRQn);
        if(!RTS_CalibFlg)
        {
            RTS_Calibrate();
        }
        else
        {
            RTS_ReadXY_AvgData();
        }
        Keyboard_Detect(RTS_XData, RTS_YData - 159, &My_Keyboard);
        Touch_Down_Flg = 1;
        switch(Show_Flg)
        {
            case 1:
            {
                Show_Flg = 2;
            }break;
            case 2:
            {
                Show_Flg = 1;
            }break;
        }
//        printf("%c\n", Keyboard(RTS_XData, RTS_YData - 159));
//        CC[0] = Keyboard(RTS_XData, RTS_YData - 159);
//        LCD_ShowString(&LCD_Prop, 0, 0, CC, 16, 1);
        
    }
    else if(GPIO_Pin == MPU_INT_Pin)
    {
        if(!mpu_dmp_get_data(&Pitch,&Roll,&Yaw))
        {
            MPU_Read_GyroData(Gyro);
            if(MPU_DMP_CNT++ > 5)
            {
                MPU_DMP_Flg = 1;
                MPU_DMP_CNT = 0;
            }
            if(Encode_Read_Flg)
            {
                Encode_Read_Flg = 0;
                L_CNT = DM_ReadCNT_L();
                R_CNT = DM_ReadCNT_R();
            }
            if(Pitch > -45 && Pitch < 45)
            {
                PWM_L =  Blance_PWM_Cal(Pitch, Gyro[1], L_CNT, R_CNT);
                PWM_R = PWM_L;
                PWM_Limiting(&PWM_L, &PWM_R);
                DM_WheelControl_All(PWM_L);
            }
            else
            {
                PWM_L = 0;
                PWM_R = 0;
                Integral_Speed = 0;
                DM_WheelControl_All(0);
            }
        }
    }
}

/*
TIM5 1ms

*/
uint32_t XXCnt = 13148;
extern uint32_t DataCnt;
extern uint8_t SD_DataBuff[];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint16_t Touch_Cnt = 0;
    
    if(htim == &htim5)
    {
        Key_Detect();
        if(Touch_Down_Flg)
        {
//            if(!(GPIOC->IDR & LCD_PEN_INT_Pin))
//            {
//                RTS_ReadXY_AvgData();
//                if(RTS_CalibFlg)
//                {
//                    if(RTS_XData < 50 && RTS_YData < 50)
//                    {
//                        LCD_ClearAll(BLUE);
//                    }
//                    LCD_ShowPoint(&LCD_Prop, RTS_XData, RTS_YData);
//                }
//            }
            if(Touch_Cnt < 100)
            {
                Touch_Cnt++;
            }
            else
            {
                Touch_Cnt = 0;
                if(GPIOC->IDR & LCD_PEN_INT_Pin)
                {
                    Touch_Down_Flg = 0;
                    __HAL_GPIO_EXTI_CLEAR_IT(LCD_PEN_INT_Pin);
                    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
                }
            }
        }
        if(Encode_GetCNT < 100)
        {
            Encode_GetCNT++;
        }
        else
        {
            Encode_Read_Flg = 1;
            Encode_GetCNT = 0;
        }
        
        if(TMP_StartFlg)
        {
            if(TMP_ControlCnt < 100)
            {
                TMP_ControlCnt++;
            }
            else
            {
                TMP_ControlCnt = 0;
                TMP_ShowFlg = 1;
                HAL_ADC_Start_IT(&hadc1);
            }
        }
    }
    else if(htim == &htim7)
    {
        if(Show_Flg == 1)
        {
            LCD_ShowBit(SD_DataBuff, 76800);
            f_read(&SDFile, SD_DataBuff, 9600, &DataCnt);
            if(DataCnt != 9600)
            {
                Show_Flg = 0;
                f_close(&SDFile);
                HAL_TIM_Base_Stop_IT(&htim7);
            }
        }
        
        if(RGB_TwinkTestFlg)
        {
            if(RGB_TwinkTestIndex % 8 != 0)
            {
                RGB_Mix(RGB_TwinkTestIndex);
                RGB_TwinkTestIndex++;
            }
            else
            {
                RGB_Mix(0);
                RGB_TwinkTestIndex = 0;
                RGB_TwinkTestFlg = 0;
                HAL_TIM_Base_Stop_IT(&htim7);
            }
        }
        if(RGB_TWinkFlg)
        {
            if(RGB_TWinkTims)
            {
                if(!(RGB_TWinkTims % 2))
                {
                  RGB_Mix(RGB_TWinkColor);
                }
                else
                {
                  RGB_Mix(0);
                }
                RGB_TWinkTims--;
            }
            else
            {
                RGB_TWinkFlg = 0;
                HAL_TIM_Base_Stop_IT(&htim7);
            }
        }
        
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc == &hadc1)
    {
        ADCTmpData = HAL_ADC_GetValue(hadc);
        TMP_Now = 3.3/4096.0*ADCTmpData*1000 - 273.2;
        TMP_ControlOutput();
    }
}

uint8_t UART1_BLE_ATMode(void)
{//Òì³£Ö¸Õë»Ö¸´ ´ýÌí¼Ó...
    static uint8_t BLERec_Data[50] = {0};
    static uint8_t BLERec_Pt = 0;
    static uint8_t BLE_ATData_Flg = 0;
    if(BLE_AT_Debug)
    {
        if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE))
        {
            BLERec_Data[BLERec_Pt++] = (uint8_t)(USART1->DR & (uint8_t)0x00FF);
            if(BLERec_Pt > 49) BLERec_Pt = 49;
        }
        if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart1);
            if(BLE_ATData_Flg || BLERec_Data[0] != '+')
            {
                BLE_ATData_Flg = 0;
                HAL_UART_Transmit(&huart2, BLERec_Data, BLERec_Pt, 0xFFFF);
                BLERec_Pt = 0;
            }
            else
            {
                 BLE_ATData_Flg = 1;
            }
        }
        return 0;
    }
    else
    {
        return 1;
    }
}

uint8_t UART2_BLE_ATMode(void)
{//Òì³£Ö¸Õë»Ö¸´ ´ýÌí¼Ó...
    static uint8_t DebugRec_Data[50] = {0};
    static uint8_t DebugRec_Pt = 0;
    
    if(BLE_AT_Debug)
    {
        if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE))
        {
            DebugRec_Data[DebugRec_Pt++] = (uint8_t)(USART2->DR & (uint8_t)0x00FF);
            if(DebugRec_Pt > 49) DebugRec_Pt = 49;
        }
        if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE))
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart2);
            HAL_UART_Transmit(&huart1, DebugRec_Data, DebugRec_Pt, 0xFFFF);
            DebugRec_Pt = 0;
        }
        return 0;
    }
    else
    {
        return 1;
    }
}

uint8_t UART1_BLE_Remote(void)
{
    if(BLE_RemoteFlg)
    {
        if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE))
        {
            BLE_RemotRec[BLE_RemotePt++] = (uint8_t)(USART1->DR & (uint8_t)0x00FF);
            if(BLE_RemotePt > 49) BLE_RemotePt = 49;
        }
        if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart1);
            BLE_RemoteDataFlg = BLE_RemotePt;
            BLE_RemotePt = 0;
        }
        return 0;
    }
    else
    {
        return 1;
    }
}

uint8_t* BLE_GetRemoteState(void)
{
    static uint8_t Data[50] = {0};
    if(!BLE_RemoteDataFlg) return NULL;
    memcpy(Data, BLE_RemotRec, BLE_RemoteDataFlg);
    BLE_RemoteDataFlg = 0;
    return Data;
}