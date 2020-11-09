#include "main.h"
#include "Music.h"
#include "tim.h"

#define DoL 262
#define ReL 294
#define MiL 330
#define FaL 349
#define SoL 392
#define LaL 440
#define SiL 494

#define DoM 523
#define ReM 587
#define MiM 659
#define FaM 698
#define SoM 784
#define LaM 880
#define SiM 988

#define DoH 1047
#define ReH 1175
#define MiH 1319
#define FaH 1397
#define SoH 1568
#define LaH 1760
#define SiH 1967

uint8_t Song_Num=0;
uint8_t Music_Flg=1;

typedef struct My_Music
{
  uint16_t M_Fre;
  uint16_t M_Beat;
  uint8_t M_Con;
}Music_TypeDef;

Music_TypeDef Tigers[33]={DoM,4,0,ReM,4,0,MiM,4,0,DoM,4,0,
DoM,4,0,ReM,4,0,MiM,4,0,DoM,4,0,
MiM,4,0,FaM,4,0,SoM,8,0,
MiM,4,0,FaM,4,0,SoM,8,0,
SoM,3,0,LaM,1,0,SoM,3,0,FaM,1,0,MiM,4,0,DoM,4,0,
SoM,3,0,LaM,1,0,SoM,3,0,FaM,1,0,MiM,4,0,DoM,4,0,
DoM,4,0,SoM,4,0,DoM,8,0,
DoM,4,0,SoM,4,0,DoM,8,0,
0,0,0};
uint16_t Tigers_Num=33;

Music_TypeDef Little_Star[43]={DoM,2,0,DoM,2,0,SoM,2,0,SoM,2,0,LaM,2,0,LaM,2,0,SoM,4,0,FaM,2,0,FaM,2,0,MiM,2,0,MiM,2,0,ReM,2,0,ReM,2,0,DoM,4,0,SoM,2,0,SoM,2,0,FaM,2,0,FaM,2,0,MiM,2,0,MiM,2,0,ReM,4,0,SoM,2,0,SoM,2,0,FaM,2,0,FaM,2,0,MiM,2,0,MiM,2,0,ReM,4,0,DoM,2,0,DoM,2,0,SoM,2,0,SoM,2,0,LaM,2,0,LaM,2,0,SoM,4,0,FaM,2,0,FaM,2,0,MiM,2,0,MiM,2,0,ReM,2,0,ReM,2,0,DoM,4,0,0,0,0};
uint16_t Little_Star_Num=43;

Music_TypeDef Happy_B[27]={SoM,1,0,SoM,1,0,LaM,2,0,SoM,2,0,DoH,2,0,SiM,4,0,SoM,1,0,SoM,1,0,LaM,2,0,SoM,2,0,ReH,2,0,DoH,4,0,SoM,1,0,SoM,1,0,SoH,2,0,MiH,2,0,DoH,2,0,SiM,2,1,LaM,4,0,0,1,0,FaH,1,0,FaH,1,0,MiH,2,0,DoH,2,0,ReH,2,0,DoH,4,0,0,0,0};
uint16_t Happy_B_Num=27;

Music_TypeDef My_Deskmate[72]={
 SoM,2,0,SoM,2,0,SoM,2,0,SoM,2,0,MiM,2,0,FaM,2,0,SoM,6,0,DoH,6,0,
 LaM,2,0,LaM,2,0,LaM,2,0,LaM,2,0,FaM,2,0,LaM,2,0,SoM,6,1,SoM,2,0,0,4,0,
 SoM,2,0,SoM,2,0,SoM,2,0,SoM,2,0,SiM,2,0,LaM,2,0,FaM,6,0,FaM,4,0,0,2,0,
 FaM,2,0,FaM,2,0,FaM,2,0,FaM,2,0,MiM,3,0,ReM,1,0,DoM,3,1,DoM,4,0,0,2,0,
 DoH,2,0,DoH,2,0,DoH,2,0,DoH,2,0,SoM,2,0,LaM,2,0,DoH,6,0,DoH,2,0,MiH,4,0,
 ReH,2,0,ReH,2,0,ReH,2,0,ReH,3,0,DoH,1,0,SiM,2,0,LaM,5,1,LaM,4,0,0,2,0,
 SiM,2,0,SiM,2,0,SiM,2,0,SiM,2,0,SiM,2,0,DoH,2,0,ReH,6,0,SoM,4,0,0,2,0,
 SiM,2,0,SiM,2,0,DoH,2,0,ReH,2,0,DoH,2,0,SiM,2,0,DoH,6,1,DoH,4,0,0,2,0,0,0,0};
uint16_t My_Deskmate_Num=72;

Music_TypeDef BaiYang[74]={
SoL,3,0, SoL,3,0, SoL,3,0, SoL,3,0, MiM,3,0, ReM,3,1, ReM,6,0, DoM,6,0, DoM,6,0, SiL,3,0, DoM,3,0, ReM,3,0, DoM,3,1, DoM,6,0, DoM,3,0, DoM,3,0, SiL,3,0, DoM,6,0, MiM,3,1, MiM,6,0, ReM,3,0, DoM,3,1, DoM,6,0, SiL,3,0, DoM,3,1,
DoM,6,0, DoM,3,0, DoM,3,0, DoM,3,0, ReM,9,0, SiL,6,0, SiL,3,0, DoM,3,1, DoM,3,0, ReM,9,0, 0,3,0, SoL,3,0, SoL,3,0, SoL,3,0, SoL,3,0, MiM,3,0, ReM,3,1, ReM,6,0, DoM,3,0, DoM,3,0, SiL,3,0, DoM,3,0, ReM,3,0, DoM,3,1, 
DoM,6,0, DoM,3,0, DoM,3,0, SiL,3,0, DoM,6,0, MiM,3,1, MiM,6,0, ReM,3,0, DoM,3,1, DoM,3,0, SiL,3,0, DoM,3,1, DoM,6,0, DoM,3,0, DoM,3,0, DoM,3,0, ReM,9,0, SiL,4,1, SiL,4,1, DoM,4,0, ReM,4,1, DoM,4,1, SiL,4,0, DoM,12,0, 0,0,0};
uint16_t BaiYang_Num=74;

void PWMGenFreSet(uint16_t Fre)
{
  htim2.Init.Prescaler=Fre;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
}

void PWM_Play_Music(Music_TypeDef Song[],uint16_t Num,uint16_t Single_Beat)
{
  uint16_t M_Fre;
  for(int i=0;i<Num;i++)
  {
    if(!Music_Flg){HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1); return;}
    if(Song[i].M_Fre)
    {
      M_Fre=168000/Song[i].M_Fre;
      PWMGenFreSet(M_Fre);
      HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
      HAL_Delay(Single_Beat*Song[i].M_Beat);
      if(!Song[i].M_Con)
      {
        HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);
        HAL_Delay(25);
      }
    }
    else if(Song[i].M_Beat)
    {
       HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);
       HAL_Delay(50*Song[i].M_Beat);
    }
    else
    {
      HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);
      return;
    }
  }
}

void Play_My_Deskmate()
{
  PWM_Play_Music(My_Deskmate,My_Deskmate_Num,150);
}

void Play_Happy_B()
{
  PWM_Play_Music(Happy_B,Happy_B_Num,150);
}

void Play_Little_Star()
{
  PWM_Play_Music(Little_Star,Little_Star_Num,150);
}

void PlayMusicOrNot()
{
  
  switch(Music_Flg)
    {
      case 1:
      {
        Play_My_Deskmate();
        HAL_Delay(1000);
        break;
      }
      case 2:
      {
        Play_Happy_B();
        HAL_Delay(1000);
        break;
      }
      case 3:
      {
        Play_Little_Star();
        HAL_Delay(1000);
        break;
      }
    }
}
