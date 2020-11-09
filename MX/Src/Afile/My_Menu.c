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
    .En     = 1,//�˵�ʹ��
    .DMI    = 8,//��Ļ��ҳ����ʾ���˵���(�������)
    .CIn    = 1,//������
    .RIn    = 16,//������
    .DCM    = 0,//��ǰMenu����
    .DCI    = 0,//��ǰItem����
    .OPC    = MENU_OPERATOR,//Menuָ�����Ȩ
    .ROF    = NO_ROLL,//����Flg
    .MENU   = MyMenu,//Menuָ��
};
