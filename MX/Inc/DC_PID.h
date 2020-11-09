#ifndef _DC_PID_H
#define _DC_PID_H

void PWM_Limiting(int16_t *PWM_L, int16_t *PWM_R);
int16_t Blance_PWM_Cal(float Angle, float Gyro, int16_t Speed_L, int16_t Speed_R);

extern float Pitch,Roll,Yaw;
extern int16_t Gyro[3];
extern double Kp_Blance;
extern double Kd_Blance;
extern double Err_Blance;
extern double Kp_Speed;
extern double Ki_Speed;
extern double Integral_Speed;
extern int16_t PWM_L, PWM_R;
extern double Intergral_Limit;
//int16_t PWMCal_Blance(float Pitch_Angle);
//int16_t dPWMCal_Speed_L(float Target_Speed);
//int16_t dPWMCal_Speed_R(float Target_Speed);
//float PWMCal_Speed(float Target_Speed);

#endif