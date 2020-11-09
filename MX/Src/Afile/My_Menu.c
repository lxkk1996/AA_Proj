#include "My_menu.h"
#include "Menu_Func.h"

/* Item define Start */
Item_TypeDef MainItem[]={
    {0,0,1,"System Test", NULL},
};

Item_TypeDef SystemSet[]={
    {0,0,2,"BlueTooth Test", NULL},
    {0,0,0,"RGB LED Test", &Menu_RGB_TwinkTest},
    {0,0,3,"BlanceCar Test", NULL},
    {0,0,0,"Video Test", &Menu_LoadVideo},
    {0,0,4,"Temper Test", NULL}
};

Item_TypeDef BlueSet[]={
    {0,0,0,"Normal Mode", &Menu_BLE_Normal},
    {1,0,0,"AT Mode", &Menu_BLE_AT},
    {2,0,0,"Remote Mode", &Menu_BLE_Remote},
    {3,0,0,"BlueTooth OFF",&Menu_BLE_OFF}
};

Item_TypeDef Mputest[]={
    {0,0,0,"Car Enable", &Menu_MPU_Enable},
    {1,0,0,"Car Disable", &Menu_MPU_Disable},
};

Item_TypeDef TmpControlSet[]={
    {0,0,0,"Set Tmp", &Menu_Set_Tmp},
    {1,0,0,"Enable", &Menu_Enable_Tmp},
    {2,0,0,"Disable", &Menu_Disable_Tmp}
};
/* Item define End */

/* Menu Define Start */
Menu_TypeDef MyMenu[]={
  {0,0,0,0,MainItem},//0
  {0,0,0,4,SystemSet},//1
  {0,0,1,3,BlueSet},//2
  {0,0,1,1,Mputest},//3
  {0,0,1,2,TmpControlSet},//4
};
/* Menu Define End */

/* Menu Property  define */
MENU_PROPERTY_Typedef Menu_Prop = 
{
    .En     = 1,//菜单使能
    .DMI    = 8,//屏幕单页可显示最大菜单项(计算滚动)
    .CIn    = 1,//列增量
    .RIn    = 16,//行增量
    .DCM    = 0,//当前Menu索引
    .DCI    = 0,//当前Item索引
    .OPC    = MENU_OPERATOR,//Menu指令控制权
    .ROF    = NO_ROLL,//翻屏Flg
    .MENU   = MyMenu,//Menu指针
};
