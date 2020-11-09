#include "Menu.h"

void Menu_Event(MENU_PROPERTY_Typedef* Mp, void * Data);
void Menu_Update(MENU_PROPERTY_Typedef* Mp);
void Main_Menu(void* Data);
void Test_Menu(void* Data);

uint16_t  LCD_Select = BLUE;
uint16_t  LCD_Font   = WHITE;
uint16_t  LCD_Backgrand = BLACK;

void Menu_Clear()
{//Clear Menu
    LCD_SetArea(&LCD_Prop, 200, 0, 319, 143);
    LCD_Cmd(0x2C);
    LCD_WriteBulk_Color(LCD_Backgrand, 17280);
}

void Menu_Updatelist(MENU_PROPERTY_Typedef* Mp, uint16_t Row, uint16_t Col, const char *String)
{//屏幕显示一行菜单项函数，可更改
    LCD_SetColor(&LCD_Prop, LCD_Font, LCD_Backgrand);
    Row = Row * Mp->RIn;
    Col = Col * Mp->CIn + 200;
    LCD_ShowString(&LCD_Prop, Col, Row, String, 8, 0);
}

void Menu_Selectlist(MENU_PROPERTY_Typedef* Mp, uint16_t Row, uint16_t Col, const char *String)
{
    Row = Row * Mp->RIn;
    Col = Col * Mp->CIn + 200;
    LCD_SetArea(&LCD_Prop, Col, Row, 319, Row + Mp->RIn);
    LCD_Cmd(0x2C);
    LCD_WriteBulk_Color(LCD_Select, 1920);
    LCD_SetColor(&LCD_Prop, LCD_Font, LCD_Backgrand);
    LCD_ShowString(&LCD_Prop, Col, Row, String, 8, 0);
}

void Menu_DeSelectlist(MENU_PROPERTY_Typedef* Mp, uint16_t Row, uint16_t Col, const char *String)
{
    Row = Row * Mp->RIn;
    Col = Col * Mp->CIn + 200;
    LCD_SetArea(&LCD_Prop, Col, Row, 319, Row + Mp->RIn);
    LCD_Cmd(0x2C);
    LCD_WriteBulk_Color(LCD_Backgrand, 1920);
    LCD_SetColor(&LCD_Prop, LCD_Font, LCD_Backgrand);
    LCD_ShowString(&LCD_Prop, Col, Row, String, 8, 0);
}

void Menu_RetControl(MENU_PROPERTY_Typedef* Mp)
{
    Mp->OPC = MENU_OPERATOR;
}

void Menu_Event(MENU_PROPERTY_Typedef* Mp, void* DataFrame)
{
    if(DataFrame == NULL) return;
    uint8_t *Data = DataFrame;
    if(Mp->OPC == MENU_OPERATOR)
    {
        if(*Data != 0x00) return;
        Menu_MOperator(Mp, Data);
    }
    else if(Mp->OPC == ITEM_OPERATOR)
    {
        if((*Data == 0x00) && (*(Data + 1) == 0x02))
        {
            Mp->MENU[Mp->DCM].Item[Mp->DCI].Operator(Data);
            Mp->OPC = MENU_OPERATOR;
            Menu_Update(Mp);
        }
        else
        {
            Mp->MENU[Mp->DCM].Item[Mp->DCI].Operator(Data);
        }
    }
}

void Menu_Update(MENU_PROPERTY_Typedef* Mp)
{
    if((Mp->ROF == ROLL_READY) || (Mp->ROF == ROLL_UP) || (Mp->ROF == ROLL_DOWN) || (Mp->ROF == NO_ROLL))
    {
        Menu_Clear();
        for(uint8_t i = Mp->MENU[Mp->DCM].MCT; i < ((Mp->DMI > Mp->MENU[Mp->DCM].MII) ?\
            (Mp->MENU[Mp->DCM].MII + 1) : (Mp->MENU[Mp->DCM].MCT+Mp->DMI)); i++)
        {
            if(i == Mp->DCI)
            {
                Menu_Selectlist(Mp, (i-Mp->MENU[Mp->DCM].MCT), 0, Mp->MENU[Mp->DCM].Item[i].Item_Name);
            }
            else
            {
                Menu_Updatelist(Mp, (i-Mp->MENU[Mp->DCM].MCT), 0, Mp->MENU[Mp->DCM].Item[i].Item_Name);
            }
        }
    }
    else if(Mp->ROF == NOROLL_UP)
    {
        if(Mp->DCI != Mp->MENU[Mp->DCM].MII)
        {
//            Menu_Updatelist(Mp, Mp->DCI - Mp->MENU[Mp->DCM].MCT + 1, 152, " ");
            Menu_DeSelectlist(Mp, Mp->DCI - Mp->MENU[Mp->DCM].MCT + 1, 0, Mp->MENU[Mp->DCM].Item[Mp->DCI + 1].Item_Name);
        }
        else
        {
//            Menu_Updatelist(Mp, 0, 152, " ");
            Menu_DeSelectlist(Mp, 0, 0, Mp->MENU[Mp->DCM].Item[0].Item_Name);
        }
        Menu_Selectlist(Mp, Mp->DCI-Mp->MENU[Mp->DCM].MCT, 0, Mp->MENU[Mp->DCM].Item[Mp->DCI].Item_Name);
    }
    else
    {
        if(Mp->DCI)
        {
//            Menu_Updatelist(Mp, Mp->DCI-Mp->MENU[Mp->DCM].MCT-1, 152, " ");
            Menu_DeSelectlist(Mp, Mp->DCI - Mp->MENU[Mp->DCM].MCT - 1, 0, Mp->MENU[Mp->DCM].Item[Mp->DCI - 1].Item_Name);
        }
        else
        {
//            Menu_Updatelist(Mp, Mp->MENU[Mp->DCM].MII, 152, " ");
            Menu_DeSelectlist(Mp, Mp->MENU[Mp->DCM].MII, 0, Mp->MENU[Mp->DCM].Item[Mp->MENU[Mp->DCM].MII].Item_Name);
        }
        Menu_Selectlist(Mp, Mp->DCI-Mp->MENU[Mp->DCM].MCT, 0, Mp->MENU[Mp->DCM].Item[Mp->DCI].Item_Name);
    }
}

void Menu_MOperator(MENU_PROPERTY_Typedef* Mp, void* pData)
{
    uint8_t* Data = pData;
    
    Mp->ROF = (Mp->DMI > (Mp->MENU[Mp->DCM].MII + 1)) ? NO_ROLL : ROLL_READY;
    switch(*(Data + 1))
    {
        case 0x00://Reflash
        {
            Menu_Update(Mp);
        }break;
        case 0x01://Enter
        {
            if(Mp->MENU[Mp->DCM].Item[Mp->DCI].Operator != NULL)
            {
                Mp->OPC = ITEM_OPERATOR;
                Mp->MENU[Mp->DCM].Item[Mp->DCI].Operator(Data);
            }
            else
            {
                Mp->DCM = Mp->MENU[Mp->DCM].Item[Mp->DCI].Jump_Menu_Num;
                Mp->DCI = 0;
                Menu_Update(Mp);
            }
        }break;
        case 0x02://Back
        {
            Mp->OPC = MENU_OPERATOR;
            Mp->MENU[Mp->DCM].MCI = Mp->MENU[Mp->DCM].MCT = 0;
            Mp->DCM = Mp->MENU[Mp->DCM].MBI;
            Mp->DCI = Mp->MENU[Mp->DCM].MCI;
            Menu_Update(Mp);
        }break;
        case 0x03://Up
        {
            if(Mp->ROF)
            {
                if(Mp->DCI == Mp->MENU[Mp->DCM].MCT)
                {
                    if(Mp->MENU[Mp->DCM].MCT)
                    {
                        Mp->MENU[Mp->DCM].MCI = --Mp->DCI;
                        Mp->MENU[Mp->DCM].MCT--;
                    }
                    else
                    {
                        Mp->MENU[Mp->DCM].MCI = Mp->DCI = Mp->MENU[Mp->DCM].MII;
                        Mp->MENU[Mp->DCM].MCT = Mp->MENU[Mp->DCM].MII-Mp->DMI+1;
                    }
                    Mp->ROF = ROLL_UP;
                }
                else
                {
                    Mp->DCI--;
                    Mp->MENU[Mp->DCM].MCI--;
                    Mp->ROF = NOROLL_UP;
                }
            }
            else
            {
                Mp->DCI-- ? (Mp->MENU[Mp->DCM].MCI = Mp->DCI) :\
                    (Mp->MENU[Mp->DCM].MCI = Mp->DCI = Mp->MENU[Mp->DCM].MII);
                    Mp->ROF = NOROLL_UP;
            }
            Menu_Update(Mp);
            break;
        }
        case 0x04://Down
        {
            if(Mp->ROF)
            {
                if(Mp->DCI == (Mp->MENU[Mp->DCM].MCT+Mp->DMI-1))
                {
                    if(Mp->DCI!=Mp->MENU[Mp->DCM].MII)
                    {
                        Mp->DCI++;
                        Mp->MENU[Mp->DCM].MCI++;
                        Mp->MENU[Mp->DCM].MCT++;
                    }
                    else
                    {
                        Mp->DCI=0;
                        Mp->MENU[Mp->DCM].MCI=0;
                        Mp->MENU[Mp->DCM].MCT=0;
                    }
                    Mp->ROF = ROLL_DOWN;
                }
                else
                {
                    Mp->DCI++;
                    Mp->MENU[Mp->DCM].MCI++;
                    Mp->ROF = NOROLL_DOWN;
                }
            }
            else
            {
                (Mp->DCI++ == Mp->MENU[Mp->DCM].MII) ? (Mp->MENU[Mp->DCM].MCI = Mp->DCI = 0) :\
                    (Mp->MENU[Mp->DCM].MCI = Mp->DCI);
                    Mp->ROF = NOROLL_DOWN;
            }
            Menu_Update(Mp);
        }break;
        default:
        {
            Menu_Update(Mp);
        }break;
    }
}


void Menu_Init(MENU_PROPERTY_Typedef* Mp, uint8_t MenuIndex)
{
    Mp->DCM = MenuIndex;
    Mp->DCI = 0;
    Menu_Update(Mp);
}

void Menu_Execute(uint8_t* pData)
{
    Menu_Event(&Menu_Prop, pData);
}