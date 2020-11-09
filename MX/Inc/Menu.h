#ifndef My_Menu_H
#define My_Menu_H

#include "main.h"
#include "LCD_TFT.h"

typedef enum
{
  NO_ROLL=0,
  ROLL_READY,
  ROLL_UP,
  ROLL_DOWN,
  NOROLL_UP,
  NOROLL_DOWN,
}Roll_Enum;

typedef enum
{
    MENU_OPERATOR = 0,
    ITEM_OPERATOR,
}OPERATOR_Enum;

///*****************************************************************************
//X Y: ����Ļ����ʾ������
//Jump_Menu_Num: �˵�����ת�˵� ��Ϊ0�����˵����
//Item_Name[]: �˵������� ��С�ɸ��� ������Ļ����
//(*Operator)()�� �˵���� ������ѡ�� (*NULL_Func)
//*****************************************************************************/
typedef struct
{
  uint8_t X;
  uint8_t Y;
  uint8_t Jump_Menu_Num;
  char Item_Name[15];
  void (*Operator)(void* Data);
}Item_TypeDef;

///*****************************************************************************
//Menu_Current_Top: ��ǰ��Ļ�˵�����
//Menu_Current_Item: ��ǰ�˵���
//Menu_Back_Index: �ϼ��˵�������
//Max_Item_Index: �˵��б������
//*****************************************************************************/
typedef struct
{
  uint8_t MCT;//Menu_Current_Top
  uint8_t MCI;//Menu_Current_Item
  uint8_t MBI;//Menu_Back_Index
  uint8_t MII;//Max_Item_Index
  Item_TypeDef *Item;
}Menu_TypeDef;

typedef struct{
    uint8_t En;//Enable
    uint8_t DMI;//Display Max_Item
    uint8_t CIn;//Col_Increment
    uint8_t RIn;//Row_Increment
    uint8_t DCM;//Display_Current_Menu
    uint8_t DCI;//Display_Current_ITEM
    OPERATOR_Enum OPC;//Operatain Control
    Roll_Enum ROF;//Roll_Flg
    Menu_TypeDef* MENU;
}MENU_PROPERTY_Typedef;

extern MENU_PROPERTY_Typedef Menu_Prop;

void NULL_Func(uint16_t Key);
void Menu_RetControl(MENU_PROPERTY_Typedef* Mp);
void Menu_Init(MENU_PROPERTY_Typedef* Mp, uint8_t MenuIndex);
void Menu_Event(MENU_PROPERTY_Typedef* Menu_Property, void * Data);
void Menu_MOperator(MENU_PROPERTY_Typedef* Menu_Property, void* pData);
void Menu_Execute(uint8_t* pData);
#endif