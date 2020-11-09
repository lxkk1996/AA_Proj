#include "main.h"
#include "Tmp_Control.h"

double TMP_Set = 50.0;
double TMP_Now = 0;
uint32_t ADCTmpData;
uint8_t TMP_StartFlg = 0;
uint8_t TMP_ShowFlg = 0;
uint8_t TMP_ControlCnt = 0;

void TMP_StartControl()
{
    TMP_StartFlg = 1;
}

void TMP_StopControl()
{
    TMP_StartFlg = 0;
    TMP_ShowFlg = 0;
    Test_GPIO_Port->BSRR = Test_Pin;
}

void TMP_ControlOutput()
{
    if(TMP_Now > TMP_Set)
    {
        Test_GPIO_Port->BSRR = Test_Pin;
    }
    else
    {
        Test_GPIO_Port->BSRR = (uint32_t)Test_Pin << 16U;
    }
}