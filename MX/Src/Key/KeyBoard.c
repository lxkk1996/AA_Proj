#include "KeyBoard.h"

KEYBOARD_Typedef My_Keyboard = {0, 0, 0, 0, 0, 0};
const uint8_t Key_LCase[4][14] = {'`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0x00,
                            0x01, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\',
                            0x02, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', 0x03, 0x03,
                            ' ', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', ' ', ' ', ' '};
const uint8_t Key_UCase[4][14] = {'~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0x00,
                                  0x01, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '|',
                                  0x02, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', 0x03, 0x03,
                                  ' ', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', ' ', ' ', ' '};

void Keyboard_Enable(KEYBOARD_Typedef* pkey)
{
    pkey->D_flg = 0;
    pkey->En = 1;
}

void Keyboard_Disable(KEYBOARD_Typedef* pkey)
{
    pkey->En = 0;
    pkey->D_flg = 0;
}

uint8_t Keyboard_StateData(KEYBOARD_Typedef* pkey, uint8_t* pdata)
{
    if(pkey->En)
    {
      if(pkey->D_flg)
      {
        pdata[0] = pkey->Cmd;
        pdata[1] = pkey->Data;
        pkey->D_flg = 0;
        return 0;
      }
    }
   return 1;
}

uint8_t Keyboard_Detect(int x, int y, KEYBOARD_Typedef* pkey)
{
    static uint16_t FKW[4] = {30, 40, 50, 60};
    static uint16_t NKW = 20;
    int Row = 0;
    int Col = 0;
    int Tmp = 0;
    if(pkey->En)
    {
        if(x < 0 || y < 0) return 1;
        Row = y/NKW;
        Tmp = x - FKW[Row];
        
        if(Row > 3)  return 2;
        else if(Tmp < 0) Col = 0;
        else Col = (((x - FKW[Row])/NKW + 1) > 13) ? 13 : ((x - FKW[Row])/NKW + 1);
        
        if(pkey->S_Flg)
        {
            if(Key_UCase[Row][Col] == 0x00)//backspace
            {
                pkey->Cmd = 0x02;
                pkey->Data = 0x00;
            }
            else if(Key_UCase[Row][Col] == 0x01)//TAB
            {
                pkey->S_Flg = 0;
                pkey->T_Flg = 1;
                pkey->Cmd = 0x03;
                pkey->Data = 0x00;
            }
            else if(Key_UCase[Row][Col] == 0x02)//shift
            {
                pkey->S_Flg = 0;
                pkey->T_Flg = 0;
                pkey->Cmd = 0x03;
                pkey->Data = 0x01;
            }
            else if(Key_UCase[Row][Col] == 0x03)//enter
            {
                pkey->Cmd = 0x02;
                pkey->Data = 0x01;
            }
            else//可视字符
            {
                pkey->Cmd = 0x01;
                pkey->Data = Key_UCase[Row][Col];
            }
            pkey->D_flg = 1;
        }
        else if(pkey->T_Flg)
        {
            switch (Key_UCase[Row][Col])
            {
                case 0x00://backspace
                {
                    pkey->D_flg = 1;
                    pkey->Cmd = 0x00;
                    pkey->Data = 0x02;
                }break;
                case 0x01://TAB
                {
                    pkey->D_flg = 1;
                    pkey->T_Flg = 0;
                    pkey->Cmd = 0x03;
                    pkey->Data = 0x00;
                }break;
                case 0x02://shift
                {
                    pkey->T_Flg = 0;
                    pkey->S_Flg = 1;
                    pkey->D_flg = 1;
                    pkey->Cmd = 0x03;
                    pkey->Data = 0x01;
                }break;
                case 0x03://enter
                {
                    pkey->D_flg = 1;
                    pkey->Cmd = 0x00;
                    pkey->Data = 0x01;
                }break;
                case 'W':
                {
                    pkey->D_flg = 1;
                    pkey->Cmd = 0x00;
                    pkey->Data = 0x03;
                }break;
//                case 'A':
//                {
//                    pkey->D_flg = 1;
//                    pkey->Cmd = 0x00;
//                    pkey->Data = 0x05;
//                }break;
                case 'S':
                {
                    pkey->D_flg = 1;
                    pkey->Cmd = 0x00;
                    pkey->Data = 0x04;
                }break;
//                case 'D':
//                {
//                    pkey->D_flg = 1;
//                    pkey->Cmd = 0x00;
//                    pkey->Data = 0x02;
//                }break;
            };
        }
        else
        {
            if(Key_LCase[Row][Col] == 0x00)//backspace
            {
                pkey->Cmd = 0x02;
                pkey->Data = 0x00;
            }
            else if(Key_LCase[Row][Col] == 0x01)//tab
            {
                pkey->S_Flg = 0;
                pkey->T_Flg = 1;
                pkey->Cmd = 0x03;
                pkey->Data = 0x00;
            }
            else if(Key_LCase[Row][Col] == 0x02)//shitf
            {
                pkey->S_Flg = 1;
                pkey->T_Flg = 0;
                pkey->Cmd = 0x03;
                pkey->Data = 0x01;
            }
            else if(Key_LCase[Row][Col] == 0x03)//enter
            {
                pkey->Cmd = 0x02;
                pkey->Data = 0x01;
            }
            else//可视字符
            {
                pkey->Cmd = 0x01;
                pkey->Data = Key_LCase[Row][Col];
            }
            pkey->D_flg = 1;
        }
    }
return 3;
}

//uint8_t Keyboard(int x, int y, KEYBOARD_Typedef* pkey)
//{
//    static uint16_t FKW[4] = {30, 40, 50, 60};
//    static uint16_t NKW = 20;
//    int Row = 0;
//    int Col = 0;
//    if(x < 0 || y < 0) return 1;
//    if(pkey->S_Flg)
//    {
//        Row = y/NKW;
//        switch(Row)
//        {
//            case 0:
//            {
//                if(x - FKW[Row] < 0)
//                {
//                    pkey->Data = '~';
//                    pkey->Cmd = 0x01;
//                }
//                else
//                {
//                    Col = (x - FKW[Row])/NKW;
//                    pkey->Cmd = 0x01;
//                    switch(Col)
//                    {
//                        case 0:     pkey->Data = '!';break;
//                        case 1:     pkey->Data = '@';break;
//                        case 2:     pkey->Data = '#';break;
//                        case 3:     pkey->Data = '$';break;
//                        case 4:     pkey->Data = '%';break;
//                        case 5:     pkey->Data = '^';break;
//                        case 6:     pkey->Data = '&';break;
//                        case 7:     pkey->Data = '*';break;
//                        case 8:     pkey->Data = '(';break;
//                        case 9:     pkey->Data = ')';break;
//                        case 10:    pkey->Data = '_';break;
//                        case 11:    pkey->Data = '+';break;
//                        default:    pkey->Cmd = 0x02;pkey->Data =0x06;//backspace
//                    }
//                }
//            }break;
//            case 1:
//            {
//                if(x - FKW[Row] < 0)
//                {
//                    pkey->T_Flg = 0;//TAB
//                }
//                else
//                {
//                    Col = (x - FKW[Row])/NKW;
//                    pkey->Cmd = 0x01;
//                    switch(Col)
//                    {
//                        case 0:     pkey->Data = 'Q';break;
//                        case 1:     pkey->Data = 'W';break;
//                        case 2:     pkey->Data = 'E';break;
//                        case 3:     pkey->Data = 'R';break;
//                        case 4:     pkey->Data = 'T';break;
//                        case 5:     pkey->Data = 'Y';break;
//                        case 6:     pkey->Data = 'U';break;
//                        case 7:     pkey->Data = 'I';break;
//                        case 8:     pkey->Data = 'O';break;
//                        case 9:     pkey->Data = 'P';break;
//                        case 10:    pkey->Data = '{';break;
//                        case 11:    pkey->Data = '}';break;
//                        default:    pkey->Data = '|';
//                    }
//                }
//            }break;
//            case 2:
//            {
//                if(x - FKW[Row] < 0)
//                {
//                    pkey->S_Flg = 0;
//                    pkey->Cmd = 0x03;
//                }
//                else
//                {
//                    Col = (x - FKW[Row])/NKW;
//                    pkey->Cmd = 0x01;
//                    switch(Col)
//                    {
//                        case 0:     pkey->Data = 'A';break;
//                        case 1:     pkey->Data = 'S';break;
//                        case 2:     pkey->Data = 'D';break;
//                        case 3:     pkey->Data = 'F';break;
//                        case 4:     pkey->Data = 'G';break;
//                        case 5:     pkey->Data = 'H';break;
//                        case 6:     pkey->Data = 'J';break;
//                        case 7:     pkey->Data = 'K';break;
//                        case 8:     pkey->Data = 'L';break;
//                        case 9:     pkey->Data = ':';break;
//                        case 10:    pkey->Data = '"';break;
//                        default:    pkey->Data = 0x06;pkey->Cmd = 0x02;//enter
//                    }
//                }
//            }break;
//            case 3:
//            {
//                if(x - FKW[Row] < 0)
//                {
//                    pkey->Data = ' ';//Space
//                    pkey->Cmd = 0x01;
//                }
//                else
//                {
//                    Col = (x - FKW[Row])/NKW;
//                    pkey->Cmd = 0x01;
//                    switch(Col)
//                    {
//                        case 0:     pkey->Data = 'Z';break;
//                        case 1:     pkey->Data = 'X';break;
//                        case 2:     pkey->Data = 'C';break;
//                        case 3:     pkey->Data = 'V';break;
//                        case 4:     pkey->Data = 'B';break;
//                        case 5:     pkey->Data = 'N';break;
//                        case 6:     pkey->Data = 'M';break;
//                        case 7:     pkey->Data = '<';break;
//                        case 8:     pkey->Data = '>';break;
//                        case 9:     pkey->Data = '?';break;
//                        default:    pkey->Data = ' ';//Space
//                    }
//                }
//            }break;
//            default: break;
//        }
//    }
//    else
//    {
//        Row = y/NKW;
//        switch(Row)
//        {
//            case 0:
//            {
//                if(x - FKW[Row] < 0)
//                {
//                    pkey->Data = '`';
//                    pkey->Cmd = 0x01;
//                }
//                else
//                {
//                    Col = (x - FKW[Row])/NKW;
//                    pkey->Cmd = 0x01;
//                    switch(Col)
//                    {
//                        case 0:     pkey->Data = '1';break;
//                        case 1:     pkey->Data = '2';break;
//                        case 2:     pkey->Data = '3';break;
//                        case 3:     pkey->Data = '4';break;
//                        case 4:     pkey->Data = '5';break;
//                        case 5:     pkey->Data = '6';break;
//                        case 6:     pkey->Data = '7';break;
//                        case 7:     pkey->Data = '8';break;
//                        case 8:     pkey->Data = '9';break;
//                        case 9:     pkey->Data = '0';break;
//                        case 10:    pkey->Data = '-';break;
//                        case 11:    pkey->Data = '=';break;
//                        default:    if(pkey->T_Flg)
//                                    {
//                                      pkey->Cmd = 0x00;pkey->Data =0x02;
//                                    }//backspace
//                                    else
//                                    {
//                                      pkey->Cmd = 0x02;pkey->Data =0x06;
//                                    }
//                    }
//                }
//            }break;
//            case 1:
//            {
//                if(x - FKW[Row] < 0)
//                {
//                   pkey->T_Flg = 1;//TAB
//                   pkey->Cmd = 0x03;
//                }
//                else
//                {
//                    Col = (x - FKW[Row])/NKW;
//                    pkey->Cmd = 0x01;
//                    switch(Col)
//                    {
//                        case 0:     pkey->Data = 'q';break;
//                        case 1:     if(pkey->T_Flg)
//                                    {
//                                      pkey->Cmd = 0x00;pkey->Data =0x03;
//                                    }//w
//                                    else
//                                    {
//                                      pkey->Data = 'w';
//                                    }break;
//                        case 2:     pkey->Data = 'e';break;
//                        case 3:     pkey->Data = 'r';break;
//                        case 4:     pkey->Data = 't';break;
//                        case 5:     pkey->Data = 'y';break;
//                        case 6:     pkey->Data = 'u';break;
//                        case 7:     pkey->Data = 'i';break;
//                        case 8:     pkey->Data = 'o';break;
//                        case 9:     pkey->Data = 'p';break;
//                        case 10:    pkey->Data = '[';break;
//                        case 11:    pkey->Data = ']';break;
//                        default:    pkey->Data = '\\';break;
//                    }
//                }
//            }break;
//            case 2:
//            {
//                if(x - FKW[Row] < 0)
//                {
//                  if(pkey->T_Flg)
//                    pkey->S_Flg = 0;
//                  else
//                  {
//                    pkey->S_Flg = 1;
//                    pkey->Cmd = 0x03;
//                  }
//                }
//                else
//                {
//                    Col = (x - FKW[Row])/NKW;
//                    pkey->Cmd = 0x01;
//                    switch(Col)
//                    {
//                        case 0:    if(pkey->T_Flg)
//                                    {
//                                      pkey->Data =0x04;pkey->Cmd = 0x00;
//                                    }//A
//                                    else
//                                    {
//                                      pkey->Data = 'a';
//                                    }break;
//                        case 1:     if(pkey->T_Flg)
//                                    {
//                                      pkey->Data =0x05;pkey->Cmd = 0x00;
//                                    }//s
//                                    else
//                                    {
//                                      pkey->Data = 's';
//                                    }break;
//                        case 2:     if(pkey->T_Flg)
//                                    {
//                                      pkey->Data =0x06;pkey->Cmd = 0x00;
//                                    }//d
//                                    else
//                                    {
//                                      pkey->Data = 'd';
//                                    }break;
//                        case 3:     pkey->Data = 'f';break;
//                        case 4:     pkey->Data = 'g';break;
//                        case 5:     pkey->Data = 'h';break;
//                        case 6:     pkey->Data = 'j';break;
//                        case 7:     pkey->Data = 'k';break;
//                        case 8:     pkey->Data = 'l';break;
//                        case 9:     pkey->Data = ';';break;
//                        case 10:    pkey->Data = '\'';break;//单引号
//                        default:    if(pkey->T_Flg)
//                                    {
//                                      pkey->Data =0x02;pkey->Cmd = 0x00;
//                                    }//backspace
//                                    else
//                                    {
//                                      pkey->Cmd = 0x02;pkey->Data = 0x07;
//                                    }break;//enter
//                    }
//                }
//            }break;
//            case 3:
//            {
//                if(x - FKW[Row] < 0)
//                {
//                   pkey->Data = ' ';//Space
//                   pkey->Cmd = 0x01;
//                }
//                else
//                {
//                    Col = (x - FKW[Row])/NKW;
//                    pkey->Cmd = 0x01;
//                    switch(Col)
//                    {
//                        case 0:      pkey->Data = 'z';break;
//                        case 1:      pkey->Data = 'x';break;
//                        case 2:      pkey->Data = 'c';break;
//                        case 3:      pkey->Data = 'v';break;
//                        case 4:      pkey->Data = 'b';break;
//                        case 5:      pkey->Data = 'n';break;
//                        case 6:      pkey->Data = 'm';break;
//                        case 7:      pkey->Data = ',';break;
//                        case 8:      pkey->Data = '.';break;
//                        case 9:      pkey->Data = '/';break;
//                        default:     pkey->Data = ' ';break;//Space
//                    }
//                }
//            }break;
//            default: break;
//        }
//    }
//    return 1;
//}