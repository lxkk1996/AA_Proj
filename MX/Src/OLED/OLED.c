/*For SSD1306 128*64OLED Display*/

/*include---------------------------------------------------------------------*/
#include "stdio.h"
#include "OLED.h"
#include "OLED_Font.h"
/*define----------------------------------------------------------------------*/

void OLED_Transmit_CMD(I2C_HandleTypeDef *hi2c, uint8_t *OLED_CMD, uint16_t CMD_Size)
{
  HAL_I2C_Master_Transmit(hi2c,OLED_ADDR,OLED_CMD,CMD_Size,OLED_Time_Out);
}

void OLED_Transmit_DATA(I2C_HandleTypeDef *hi2c, uint8_t *OLED_DATA, uint16_t Data_Size)
{
  HAL_I2C_Mem_Write(hi2c, OLED_ADDR, OLED_PREDATA, I2C_MEMADD_SIZE_8BIT, OLED_DATA, Data_Size, OLED_Time_Out);
}

//Time & Driving Scheme Setting Command 时钟设置命令
/*******************************************************************************
@brief    Set Display Clock Divide Ratio/Oscillator Frequency
@func     OLED_Set_Clock(I2C_HandleTypeDef *hi2c, uint8_t Fosc, uint8_t DCLK)
@param    hi2c: hi2c handle e.g. hi2c1, hi2c2 etc.
          Fosc: Set the OSC Frequency, it can be 0000b~1111b, 
                Frequency increases as setting value increases.(RESET 1000b)
          DCLK: Frequancy divede ratio, it can be 0000b~1111b.(RESet 0000b,ratio=1)
*******************************************************************************/
//配置时钟及分频
void OLED_Set_Clock(I2C_HandleTypeDef *hi2c, uint8_t Fosc, uint8_t DCLK)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,0xD5,DCLK|(Fosc<<4)};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

/*******************************************************************************
@brief    Set Pre-charge Period
@func     OLED_Set_PCP(I2C_HandleTypeDef *hi2c, uint8_t Phase_1, uint8_t Phase_2)
@param    hi2c: hi2c handle e.g. hi2c1, hi2c2 etc.
          Phase 1 period of up to 15 DCLK
          Phase 2 period of up to 15 DCLK
          clocks 0 is invalid entry(RESET 2h)
*******************************************************************************/
//设置预充电时间
void OLED_Set_PCP(I2C_HandleTypeDef *hi2c, uint8_t Phase_1, uint8_t Phase_2)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,0xD9, (Phase_2<<4)|Phase_1};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

/*******************************************************************************
@brief    Set V_COMH Deselect Level
@func     OLED_Set_VCOMH(I2C_HandleTypeDef *hi2c, uint8_t VCOMH)
@param    hi2c: hi2c handle e.g. hi2c1, hi2c2 etc.
         ------------------------------------------
         -HEX_code(V_COMH) - V_COMH deselect level-
         ------------------------------------------
         -   0x00          -     ~0.65*Vcc        -
         ------------------------------------------   
         -   0x20          -     ~0.77*Vcc        -
         ------------------------------------------       
         -   0x30          -     ~0.83*Vcc        -
         ------------------------------------------
*******************************************************************************/
//调整V_COMH输出
void OLED_Set_VCOMH(I2C_HandleTypeDef *hi2c, uint8_t VCOMH)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,0xDB,VCOMH<<4};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

//Hardware Configuration 硬件配置
/*******************************************************************************
@func     OLED_Set_Start_Line(I2C_HandleTypeDef *hi2c,uint8_t DSL)
@brief    Set display start line(0~63)
@param    hi2c: hi2c handle e.g. hi2c1, hi2c2 etc.
          DSL: Display Start Line, it can be 000000b~111111b(0d~63d)(RESET 000000b)
*******************************************************************************/
//设置起始行
void OLED_Set_Start_Line(I2C_HandleTypeDef *hi2c,uint8_t DSL)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,0x40|DSL};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

/*******************************************************************************
@func     OLED_Set_SEG_Remap(I2C_HandleTypeDef *hi2c,uint8_t Remap)
@brief    Set segment Re-map
@param    hi2c: hi2c handle e.g. hi2c1, hi2c2 etc.
          Remap: it can be Set or Reset
                 Reset: column address 0 is mapped to SEG0(RESET)
                 Set: column address 127 is mapped to SEG0
*******************************************************************************/
//列重映射
void OLED_Set_SEG_Remap(I2C_HandleTypeDef *hi2c, Remap_TypeDef Remap)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,(Remap?0xA1:0xA0)};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

/*******************************************************************************
@func     OLED_Set_Multiplex_Ratio(I2C_HandleTypeDef *hi2c,uint8_t Ratio)
@brief    Set multiplex ratio, from 16MUX to 64MUX, RESET=111111b (i.e. 63d, 64MUX)
@param    hi2c: hi2c handle e.g. hi2c1, hi2c2 etc.
          Remap: it can be 0x0f~0x3f 15d~63d
*******************************************************************************/
void OLED_Set_Multiplex_Ratio(I2C_HandleTypeDef *hi2c, uint8_t Ratio)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,0xA8,Ratio};  
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

/*******************************************************************************
@func     OLED_Set_COM_OD(I2C_HandleTypeDef *hi2c, uint8_t Direction)
@brief    Set COM output scan direction
@param    hi2c: hi2c handle e.g. hi2c1, hi2c2 etc.
          Direction: it can be Reset or Set
          Reset: normal mode (RESET) Scan from COM0 to COM[N –1] 
          Set: remapped mode. Scan from COM[N-1] to COM0 
          Where N is the Multiplex ratio. 
*******************************************************************************/
void OLED_Set_COM_OD(I2C_HandleTypeDef *hi2c, Direction_TypeDef Direction)
{ 
  uint8_t OLED_CMD[]={OLED_PRECMD,(Direction?0xC0:0xC8)};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

/*******************************************************************************
@func     OLED_Set_Display_Offset(I2C_HandleTypeDef *hi2c, uint8_t Offset)
@brief    Set Display Offset 
@param    hi2c: hi2c handle e.g. hi2c1, hi2c2 etc.
          Offset: Set vertical shift by COM from 0d~63d 
          The value is reset to 00h after RESET.
*******************************************************************************/
void OLED_Set_Display_Offset(I2C_HandleTypeDef *hi2c, uint8_t Offset)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,0xD3,Offset};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

/*******************************************************************************
@func     OLED_Set_COM_PHC(I2C_HandleTypeDef *hi2c, uint8_t PHC)
@brief    Set COM Pins Hardware Configuration
@param    hi2c: hi2c handle e.g. hi2c1, hi2c2 etc.
          PHC: it can be 0d~3d
          High bit 0: (RESET), Disable COM Left/Right remap 
                   1: Enable COM Left/Right remap
          Low bit 0: Sequential COM pin configuration
                  1: (RESET), Alternative COM pin configuration
*******************************************************************************/
void OLED_Set_COM_PHC(I2C_HandleTypeDef *hi2c, uint8_t PHC)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,0xDA,(PHC<<4)|0x02};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

/*******************************************************************************
@func     OLED_Set_ChargePump(I2C_HandleTypeDef *hi2c, Switch_Type Switch)
@brief    Charge pump Settinig
@param    hi2c: hi2c handle e.g. hi2c1, hi2c2 etc.
          Switch: it can be On or Off(i.e. On enable Charge pump,Off disable charge pump)
*******************************************************************************/
//充电泵开启
void OLED_Set_Charge_Pump(I2C_HandleTypeDef *hi2c, Switch_TypeDef Switch)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,0x8D,Switch?0x14:0x10};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}


//Addressing Setting Command 地址设置命令
/*******************************************************************************
@func     OLED_Set_ADDR_Mode(I2C_HandleTypeDef *hi2c, ADDR_Mode_TypeDef Mode)
@brief    Addressing Mode Setting
@param    hi2c: hi2c handle e.g. hi2c1, hi2c2 etc.
          Mode:  Horizontal_ADDR_Mode,
                 Vertical_ADDR_Mode,
                 Page_ADDR_Mode
*******************************************************************************/
void OLED_Set_ADDR_Mode(I2C_HandleTypeDef *hi2c, ADDR_Mode_TypeDef Mode)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,0x20,Mode};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

//Page_ADDR_Mode Only
void OLED_Set_StartPage(I2C_HandleTypeDef *hi2c, uint8_t Page)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,0xB0|Page};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

//Page_ADDR_Mode Only
void OLED_Set_StartColumn(I2C_HandleTypeDef *hi2c, uint8_t Column)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,0x0F&Column, OLED_PRECMD, ((Column&0xF0)>>4)|0x10};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

//Horizontal_ADDR_Mode OR Vertical_ADDR_Mode Only
void OLED_Set_StartEnd_Page(I2C_HandleTypeDef *hi2c, uint8_t Page_S, uint8_t Page_E)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,0x22, Page_S, Page_E};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

//Horizontal_ADDR_Mode OR Vertical_ADDR_Mode Only
void OLED_Set_StartEnd_Column(I2C_HandleTypeDef *hi2c, uint8_t Column_S, uint8_t Column_E)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,0x21, Column_S, Column_E};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

//Fundamental Command 基本命令
/*******************************************************************************
@func     OLED_Set_Contrast(I2C_HandleTypeDef *hi2c, uint8_t Contrast)
@brief    Set contrast control
@param   hi2c: hi2c handle e.g. hi2c1, hi2c2 etc.
         Contrast: it can be 0x00~0xff(RESET 0x7f)
*******************************************************************************/
//对比度设置
void OLED_Set_Contrast(I2C_HandleTypeDef *hi2c, uint8_t Contrast)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,0x81,Contrast};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

/*******************************************************************************
          Set Display ON/OFF

@func     OLED_Display_On(I2C_HandleTypeDef *hi2c)
@brief    Open display in normal mode(weak up from sleep mode)
@param    hi2c: hi2c handle e.g. hi2c1, hi2c2 etc.
          Switch: it can be Set or Reset
                  Set: Display on
                  Reset: Display off
*******************************************************************************/
//屏幕休眠开关
void OLED_Display_On(I2C_HandleTypeDef *hi2c, Switch_TypeDef Switch)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,Switch?0xAF:0xAE};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

/*******************************************************************************
@func     OLED_Set_Entire_On(I2C_HandleTypeDef *hi2c, Set_TypeDef Set)
@brief    Output ignore display RAM content(light up all pixel) or normal display mode
@param    hi2c: hi2c handle e.g. hi2c1, hi2c2 etc.
          Set: it can be Set or Reset.
               Set: light up all pixel.
               Reset: normal Display.
*******************************************************************************/
//设置全亮屏
void OLED_Set_Entire_On(I2C_HandleTypeDef *hi2c, Set_TypeDef Set)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,Set?0xA5:0xA4};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

/*******************************************************************************
@func     OLED_Set_Inverse(I2C_HandleTypeDef *hi2c, Set_TypeDef Set)
@brief    Set Normal/Inverse Display.
@param    hi2c: hi2c handle e.g. hi2c1, hi2c2 etc.
          Set: it can be Set or Reset.
               Set: inverse display.
               Reset: normal display.
*******************************************************************************/
//反色设置
void OLED_Set_Inverse_On(I2C_HandleTypeDef *hi2c, Set_TypeDef Set)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,Set?0xA7:0xA6};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

//Clear All
void OLED_Clear(I2C_HandleTypeDef *hi2c)
{
  OLED_Set_ADDR_Mode(hi2c, Vertical_ADDR_Mode);
  OLED_Set_StartEnd_Page(hi2c, 0, 7);
  OLED_Set_StartEnd_Column(hi2c, 0, 127);
  //uint8_t Clear[1024]={OLED_L_DATA};/*Need More Stack*/
  OLED_Transmit_DATA(hi2c, Clear, OLED_Picture_Size);
}

//Clear 2 Continued Page
void OLED_Clear_Page(I2C_HandleTypeDef *hi2c, uint8_t Page)
{
  OLED_Set_ADDR_Mode(hi2c, Horizontal_ADDR_Mode);
  OLED_Set_StartEnd_Page(hi2c, Page, Page+1);
  OLED_Set_StartEnd_Column(hi2c, 0, 127);
  //uint8_t Clear[256]={OLED_L_DATA};
  OLED_Transmit_DATA(hi2c, Clear, 256);
}

//Clear  Rectangle
void OLED_Clear_Rec(I2C_HandleTypeDef *hi2c, uint8_t Page_S ,uint8_t Page_E, uint8_t Col_S, uint8_t Col_E)
{
  OLED_Set_ADDR_Mode(hi2c, Horizontal_ADDR_Mode);
  OLED_Set_StartEnd_Page(hi2c, Page_S, Page_E);
  OLED_Set_StartEnd_Column(hi2c, Col_S, Col_E);
  //uint8_t Clear[1024]={OLED_L_DATA};/*Need More Stack*/
  OLED_Transmit_DATA(hi2c, Clear, (Page_E-Page_S+1)*(Col_E-Col_S+1));
}


void OLED_Progress_Bar(I2C_HandleTypeDef *hi2c, uint8_t Page, uint8_t Num)
{
  uint8_t Bar[128]={OLED_L_DATA};
  OLED_Set_ADDR_Mode(hi2c, Page_ADDR_Mode);
  OLED_Set_StartPage(hi2c, Page);
  OLED_Set_StartColumn(hi2c, 0);
  OLED_Transmit_DATA(hi2c, Bar, 128);
  for(uint8_t i=0;i<=Num;i++) Bar[i]=OLED_H_DATA;
  OLED_Transmit_DATA(hi2c, Bar, Num);
}

void OLED_Picture(I2C_HandleTypeDef *hi2c, uint8_t *Picture)
{
  OLED_Set_ADDR_Mode(hi2c, Horizontal_ADDR_Mode);
  OLED_Set_StartEnd_Page(hi2c, 0, 7);
  OLED_Set_StartEnd_Column(hi2c, 0, 127);
  OLED_Transmit_DATA(hi2c, Picture, OLED_Picture_Size);
}

void OLED_String(I2C_HandleTypeDef *hi2c, uint8_t Row, uint8_t Col, char *String)
{
  uint8_t Count=0;
  Row*=2;
  Col*=8;
  OLED_Set_ADDR_Mode(hi2c, Horizontal_ADDR_Mode);
  while((String[Count]!=0) && (String[Count]>=0x20) && (String[Count]<=0x7F))
  {
    OLED_Set_StartEnd_Column(hi2c, Col, Col+7);
    OLED_Set_StartEnd_Page(hi2c, Row, Row+1);
    OLED_Transmit_DATA(hi2c, &ASCII[16*(String[Count]-0x20)], 16);
    Count++;
    if((Col+=8)>120)
    {
      Col=0;
      Row+=2;
    }
  }
}

void OLED_String16(I2C_HandleTypeDef *hi2c, uint8_t Row, uint8_t Col, char *String)
{
  uint8_t Count=0;
  Row*=2;
  Col*=16;
  OLED_Set_ADDR_Mode(hi2c, Horizontal_ADDR_Mode);
  while((String[Count]!=0) && (String[Count]>=0x20) && (String[Count]<=0x7F))
  {
    OLED_Set_StartEnd_Column(hi2c, Col, Col+15);
    OLED_Set_StartEnd_Page(hi2c, Row, Row+1);
    OLED_Transmit_DATA(hi2c, &ASCII16_16[32*(String[Count]-0x20)], 32);
    Count++;
    if((Col+=16)>112)
    {
      Col=0;
      Row+=2;
    }
  }
}
void OLED_String32(I2C_HandleTypeDef *hi2c, uint8_t Row, uint8_t Col, char *String)
{
  uint8_t Count=0;
  Row*=4;
  Col*=16;
  OLED_Set_ADDR_Mode(hi2c, Horizontal_ADDR_Mode);
  while((String[Count]!=0) && (String[Count]>=0x20) && (String[Count]<=0x7F))
  {
    OLED_Set_StartEnd_Column(hi2c, Col, Col+15);
    OLED_Set_StartEnd_Page(hi2c, Row, Row+3);
    OLED_Transmit_DATA(hi2c, &ASCII32_16[64*(String[Count]-0x20)], 64);
    Count++;
    if((Col+=16)>112)
    {
      Col=0;
      Row+=4;
    }
  }
}

void OLED_Num(I2C_HandleTypeDef *hi2c, uint8_t Row, uint8_t Col, int32_t Num)
{
  uint8_t Count=0;
  char String[8]={0};
  sprintf(String,"%d",Num);
  Row*=2;
  Col*=8;
  OLED_Set_ADDR_Mode(hi2c, Horizontal_ADDR_Mode);
  while((String[Count]!=0) && (String[Count]>=0x20) && (String[Count]<=0x7F)) 
  {
    OLED_Set_StartEnd_Column(hi2c, Col, Col+7);
    OLED_Set_StartEnd_Page(hi2c, Row, Row+1);
    OLED_Transmit_DATA(hi2c, &ASCII[16*(String[Count]-0x20)], 16);
    Count++;
    if((Col+=8)>120)
    {
      Col=0;
      Row+=2;
    }
  }
}

void OLED_FNum(I2C_HandleTypeDef *hi2c, uint8_t Row, uint8_t Col, double Num, uint8_t Precision)
{
  uint8_t Count=0;
  char String[8]={0};
  snprintf(String,Precision+2,"%lf",Num);
  Row*=2;
  Col*=8;
  OLED_Set_ADDR_Mode(hi2c, Horizontal_ADDR_Mode);
  while((String[Count]!=0) && (String[Count]>=0x20) && (String[Count]<=0x7F)) 
  {
    OLED_Set_StartEnd_Column(hi2c, Col, Col+7);
    OLED_Set_StartEnd_Page(hi2c, Row, Row+1);
    OLED_Transmit_DATA(hi2c, &ASCII[16*(String[Count]-0x20)], 16);
    Count++;
    if((Col+=8)>120)
    {
      Col=0;
      Row+=2;
    }
  }
}

/*******************************************************************************
@func     OLED_Init(I2C_HandleTypeDef *hi2c)
@brief    Initialization OLED module.
@param    hi2c: hi2c handle e.g. hi2c1, hi2c2 etc.
*******************************************************************************/
//OLED 初始化
void OLED_Init(I2C_HandleTypeDef *hi2c)
{
  HAL_Delay(50);//For Charge
  OLED_Display_On(hi2c, Reset);
  OLED_Set_Multiplex_Ratio(hi2c, 63);
  OLED_Set_Display_Offset(hi2c, 0x00);
  OLED_Set_Start_Line(hi2c, 0x00);
  OLED_Set_SEG_Remap(hi2c, Set);
  OLED_Set_COM_OD(hi2c, Reset);
  OLED_Set_COM_PHC(hi2c, 1);
  OLED_Set_Contrast(hi2c, 0x80);
  OLED_Set_Entire_On(hi2c, Reset);
  OLED_Set_Inverse_On(hi2c, Reset);
  OLED_Set_Clock(hi2c, 0x0f, 0x00);
  OLED_Set_Charge_Pump(hi2c, Set);
  OLED_Picture(hi2c, PowerOn);
  OLED_Display_On(hi2c, Set);
  OLED_Set_ADDR_Mode(hi2c, Page_ADDR_Mode);
  OLED_Set_StartPage(hi2c, 7);
  OLED_Set_StartColumn(hi2c, 0);
  uint8_t Bar=0xff;
  for(uint8_t i=0;i<128;i++)
  {
    HAL_Delay(5);
    OLED_Transmit_DATA(hi2c, &Bar, 1);
  }
  OLED_Clear(hi2c);
}

/*For Test New Function*/
void OLED_Test(I2C_HandleTypeDef *hi2c)
{
  OLED_Set_ADDR_Mode(hi2c, Vertical_ADDR_Mode);
  OLED_Set_StartEnd_Page(hi2c, 0, 7);
  OLED_Set_StartEnd_Column(hi2c, 63,63);
  uint8_t OLED_DATA[]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
  OLED_Transmit_DATA(hi2c, OLED_DATA, 8);
}

void OLED_DisableScroll(I2C_HandleTypeDef *hi2c)
{
  uint8_t OLED_CMD[]={OLED_PRECMD, 0x2E};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

void OLED_EnableScroll(I2C_HandleTypeDef *hi2c)
{
  uint8_t OLED_CMD[]={OLED_PRECMD, 0x26,0x2F};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
}

void OLED_SetArea(I2C_HandleTypeDef *hi2c)
{
  uint8_t OLED_CMD[]={OLED_PRECMD,0x2e};
  OLED_Transmit_CMD(hi2c, OLED_CMD, sizeof(OLED_CMD));
  uint8_t OLED_CMD1[]={OLED_PRECMD,0x26, 0x00, 0x00, 0x00,0x07,0x00,0xff};
  OLED_Transmit_CMD(hi2c, OLED_CMD1, sizeof(OLED_CMD1));
  uint8_t OLED_CMD2[]={OLED_PRECMD,0x2f};
  OLED_Transmit_CMD(hi2c, OLED_CMD2, sizeof(OLED_CMD2));
}