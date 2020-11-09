#include "Key.h"

#define Key_UpRead      ((KEY_UP_GPIO_Port->IDR & KEY_UP_Pin) ? 0 : 1)
#define Key_DownRead    ((KEY_DOWN_GPIO_Port->IDR & KEY_DOWN_Pin) ? 0 : 1)
#define Key_LeftRead    ((KEY_LEFT_GPIO_Port->IDR & KEY_LEFT_Pin) ? 0 : 1)
#define Key_RightRead   ((KEY_RIGHT_GPIO_Port->IDR & KEY_RIGHT_Pin) ? 0 : 1)
#define Key_OkRead      ((KEY_OK_GPIO_Port->IDR & KEY_OK_Pin) ? 0 : 1)

typedef struct{
    uint8_t key_En;
    uint16_t Delay;
    uint16_t Count;
    uint8_t DownFlg;
    KEY_STATE_ENUM State;
}KEY_Typedef;

KEY_Typedef My_Key = {0, 100, 0, 0, KEY_NONE};


KEY_STATE_ENUM Key_GetState(void)
{
    KEY_STATE_ENUM Tmp_state;
    Tmp_state = My_Key.State;
    My_Key.State = KEY_NONE;
    return Tmp_state;
}

uint8_t* Key_StateData(void)
{
    static uint8_t Data[2] = {0x00, 0x00};
    Data[1] = Key_GetState();
    if(Data[1] == KEY_NONE)
    {
        return NULL;
    }
    else
    {
        return Data;
    }
}

KEY_STATE_ENUM Key_Detect(void)
{
    if(My_Key.key_En)
    {
        if(Key_UpRead)
        {
            My_Key.DownFlg = 1;
            if(++My_Key.Count < My_Key.Delay) return KEY_NONE;
            My_Key.State = KEY_UP;
            goto Lable_KeyDown;
        }
        else if(Key_DownRead)
        {
            My_Key.DownFlg = 1;
            if(++My_Key.Count < My_Key.Delay) return KEY_NONE;
            My_Key.State = KEY_DOWN;
            goto Lable_KeyDown;
        }
        else if(Key_LeftRead)
        {
            My_Key.DownFlg = 1;
            if(++My_Key.Count < My_Key.Delay) return KEY_NONE;
            My_Key.State = KEY_LEFT;
            goto Lable_KeyDown;
        }
        else if(Key_RightRead)
        {
            My_Key.DownFlg = 1;
            if(++My_Key.Count < My_Key.Delay) return KEY_NONE;
            My_Key.State = KEY_RIGHT;
            goto Lable_KeyDown;
        }
        else if(Key_OkRead)
        {
            My_Key.DownFlg = 1;
            if(++My_Key.Count < My_Key.Delay) return KEY_NONE;
            My_Key.State = KEY_OK;
            goto Lable_KeyDown;
        }
    }
    My_Key.DownFlg = 0;
    My_Key.Count = 0;
    return KEY_NONE;
    
Lable_KeyDown:
    My_Key.Count = 0;
    return My_Key.State;
}

void Key_init()
{
    My_Key.key_En = 1;
    HAL_TIM_Base_Start_IT(&htim5);
}