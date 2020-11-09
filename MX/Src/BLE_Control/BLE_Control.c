#include "main.h"
#include "BLE_Control.h"

void BLE_Power(uint8_t Power_CMD)
{
    switch(Power_CMD)
    {
        case BLE_ON:
        {
            BLE_POW_GPIO_Port->BSRR = (uint32_t)BLE_POW_Pin << 16U;
        }break;
        case BLE_OFF:
        {
            BLE_POW_GPIO_Port->BSRR = BLE_POW_Pin;
        }break;
    }
}

void BLE_Mode_Select(uint8_t Mode_CMD)
{
    switch(Mode_CMD)
    {
        case BLE_ATMode:
        {
            BLE_Power(BLE_OFF);
            HAL_Delay(10);
            BLE_AT_GPIO_Port->BSRR = BLE_AT_Pin;
            BLE_Power(BLE_ON);
        }break;
        case BLE_NormalMode:
        {
            BLE_Power(BLE_OFF);
            HAL_Delay(10);
            BLE_AT_GPIO_Port->BSRR = (uint32_t)BLE_AT_Pin << 16U;
            BLE_Power(BLE_ON);
        }break;
    }
}
