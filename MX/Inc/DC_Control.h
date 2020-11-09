#ifndef DC_CONTROL_H
#define DC_CONTROL_H

#define Dir_N   0
#define Dir_S   1
#define Dir_F   2
#define Dir_B   3
#define DisEngage   0
#define Engage      1

extern int16_t L_CNT;
extern int16_t R_CNT;

void DM_DIR_CTL_L(uint8_t Dir);
void DM_DIR_CTL_R(uint8_t Dir);
void DM_SetPWM_L(uint16_t PWM);
void DM_SetPWM_R(uint16_t PWM);
void DM_WheelControl_L(int16_t PWM);
void DM_WheelControl_R(int16_t PWM);
void DM_WheelControl_All(int16_t PWM);
void DM_PWM_Engage(uint8_t EngageCMD);
void DM_EncodeStart(void);
void DM_EncodeStop(void);
int16_t DM_ReadCNT_L(void);
int16_t DM_ReadCNT_R(void);
#endif