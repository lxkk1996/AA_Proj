#include "main.h"
#include "stdint.h"
#include "DC_Control.h"
#include "DC_PID.h"

float Pitch,Roll,Yaw;
int16_t Gyro[3];

double Kp_Blance = 100;
double Kd_Blance = 0.3;

double Err_Blance = 0.0;

double Kp_Speed = 0.01;
double Ki_Speed = 0.00005;
double Integral_Speed = 0;
double Intergral_Limit = 10000;
int16_t PWM_L, PWM_R;

//float Kp_Blance = 95;
//float Kd_Blance = 0.2;
//
//float Err_Blance = 0.0;
//
//float Kp_Speed = 0.15;
//float Ki_Speed = 0.001;
//float Integral_Speed = 0.0;
//int Intergral_Limit = 5;
//int16_t PWM_L, PWM_R;

void PWM_Limiting(int16_t *PWM_L, int16_t *PWM_R)
{
    if(*PWM_L > 0)
    {
        *PWM_L += 2249;
    }
    if(*PWM_L < 0)
    {
        *PWM_L -= 2249;
    }
    if(*PWM_R > 0)
    {
        *PWM_R += 2249;
    }
    if(*PWM_R < 0)
    {
        *PWM_R -= 2249;
    }
    
    if(*PWM_L > 4199)
    {
        *PWM_L = 4199;
    }
    else if(*PWM_L < -4199)
    {
        *PWM_L = -4199;
    }
    
    if(*PWM_R > 4199)
    {
        *PWM_R = 4199;
    }
    else if(*PWM_R < -4199)
    {
        *PWM_R = -4199;
    }
}

int16_t Blance_PWM_Cal(float Angle, float Gyro, int16_t Speed_L, int16_t Speed_R)
{
    int16_t Err_Speed, Err_SpeedLowPass_Out;
    static int16_t Err_SpeedLowPass_LastOut;
    static float Speed_LowPass_k = 0.7;
    float Intergral_Cal;
    int16_t PWM_Out;
    Err_Speed= Speed_L + Speed_R;
    Err_SpeedLowPass_Out = (int16_t)((1 - Speed_LowPass_k) * Err_Speed + Speed_LowPass_k * Err_SpeedLowPass_LastOut);
    Err_SpeedLowPass_LastOut = Err_SpeedLowPass_Out;
    Integral_Speed += Err_SpeedLowPass_Out;
    if(Integral_Speed > Intergral_Limit)
    {
        Integral_Speed = Intergral_Limit;
    }
    else if(Integral_Speed < -Intergral_Limit)
    {
        Integral_Speed = -Intergral_Limit;
    }
    Intergral_Cal = Ki_Speed * Integral_Speed;
    
    PWM_Out = (int16_t)(-Kp_Blance * Angle - Kd_Blance * Gyro + Kp_Blance*(Kp_Speed* Err_Speed + Intergral_Cal));
    return PWM_Out;
}