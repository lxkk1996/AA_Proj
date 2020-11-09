#ifndef LCD_TFT_H
#define LCD_TFT_H

typedef struct
{
    volatile uint16_t CMD;
    volatile uint16_t DATA;
}LCD_TypeDef;
#define LCD_CMD_ADDR            ((uint32_t)(0x60000000 | 0x00001FFE))
#define LCD                     ((LCD_TypeDef *) LCD_CMD_ADDR)

#define LCD_Delay(Ms)           HAL_Delay(Ms)

typedef struct
{
    union
    {
        uint32_t Status;
        struct
        {
            uint32_t No_used0:5;
            uint32_t Tearing_Effect_Mode:1;
            uint32_t Gamma_Curve:3;
            uint32_t Tearing_Effect_On:1;
            uint32_t Display_On:1;
            uint32_t Not_Used1:5;
            uint32_t Display_Mode:1;
            uint32_t Sleep_Mode:1;
            uint32_t Partial_Mode:1;
            uint32_t Idle_Mode:1;
            uint32_t Interface_Color_Format:3;
            uint32_t Not_Used2:2;
            uint32_t Horizontal_Refresh:1;
            uint32_t RGB_Order:1;
            uint32_t Vertical_Refresh:1;
            uint32_t Row_Col_Exchage:1;
            uint32_t Col_Addr_Order:1;
            uint32_t Row_Addr_Order:1;
            uint32_t Booster_Status:1;
        };
    };
}LCD_Status_TypeDef;

typedef struct
{
    uint16_t ID;
    uint16_t Pixel_Color;
    uint16_t Back_Color;
    uint16_t Max_H;
    uint16_t Max_W;
    LCD_Status_TypeDef Display_Status;
}LCD_Property_Typedef;
extern  LCD_Property_Typedef    LCD_Prop;

#define LCD_ON      1
#define LCD_OFF     0
typedef enum
{
    LCD_FHOR = 1,
    LCD_FVER = 2,
    LCD_RRA  = 4,
}LCD_FR_ENUM;


#define CMD_NOP     0x0000

#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define BRED        0XF81F
#define GRED        0XFFE0
#define GBLUE       0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN       0XBC40
#define BRRED       0XFC07
#define GRAY        0X8430


extern uint8_t gImage_ED[];
void LCD_FuncTest(void);

uint8_t LCD_Init(LCD_Property_Typedef* lcd);
void LCD_Cmd(uint16_t Cmd);
void LCD_WriteBulk_DATA(uint16_t* DataBuff, uint32_t BuffSize);
void LCD_WriteBulk_Color(uint16_t Color, uint32_t ColorNum);
void LCD_Set_Cursor(uint16_t SCol, uint16_t SRow);
uint8_t LCD_SetArea(LCD_Property_Typedef* lcd, uint16_t SCol, uint16_t SRow, uint16_t ECol, uint16_t ERow);
void LCD_Reset_Cursor();
void LCD_SetColor(LCD_Property_Typedef* lcd, uint16_t pixColor, uint16_t BackColor);
void LCD_ClearAll(uint16_t Color);
void LCD_ShowPic(void* pdata);
void LCD_WritePrepare();
void LCD_ShowString(LCD_Property_Typedef* lcd, uint16_t SCol, uint16_t SRow, const char* String, uint8_t FontWidth, uint8_t WriteMode);
void LCD_ShowFNum(LCD_Property_Typedef* lcd, uint16_t SCol, uint16_t SRow, double FNum, uint8_t Precision ,uint8_t FontWidth, uint8_t WriteMode);
void LCD_ShowNum(LCD_Property_Typedef* lcd, uint16_t SCol, uint16_t SRow, int Num, uint8_t FontWidth, uint8_t WriteMode);
void LCD_ShowPoint(LCD_Property_Typedef* lcd, uint16_t CCol, uint16_t CRow);
void LCD_ShowCorss(LCD_Property_Typedef* lcd, uint16_t CCol, uint16_t CRow, uint16_t Radius);
void LCD_ShowBit(uint8_t* BitBuff, uint32_t BuffSize);
void LCD_FilpRotate(LCD_Property_Typedef* pLcd_Prop, LCD_FR_ENUM hv);
#endif