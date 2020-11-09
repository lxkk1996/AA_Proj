#include "mpu6050.h"
#include "math.h"

int16_t ACC_DATA[3];
double  ACC_Real[3];
int16_t GYRO_DATA[3];
double  GYRO_Real[3];

#define q30  1073741824.0f

//陀螺仪方向设置
static signed char gyro_orientation[9] = { 1, 0, 0,
                                           0, 1, 0,
                                           0, 0, 1};

/*Before Use This .c
 *USER CODE BEGIN
 *Change code between these(BEGIN_END) first
 *USER CODE END
 *NOTE: Don't forget .h file
*/
uint8_t MPU_Writ_Data(uint8_t ucSlave_Addr, uint8_t ucReg_Addr,uint16_t usData_Size, uint8_t *pData)
{
  // USER CODE BEGIN
  return HAL_I2C_Mem_Write(&hi2c2, ucSlave_Addr, ucReg_Addr,I2C_MEMADD_SIZE_8BIT , pData, usData_Size, MPU_Time_Out);
  //USER CODE END
}

uint8_t MPU_Read_Data(uint8_t ucSlave_Addr, uint8_t ucReg_Addr,uint16_t usData_Size, uint8_t *pData)
{
  // USER CODE BEGIN
  return HAL_I2C_Mem_Read(&hi2c2, ucSlave_Addr, ucReg_Addr, I2C_MEMADD_SIZE_8BIT, pData, usData_Size, MPU_Time_Out);
  //USER CODE END
}

void MPU_GetMs(unsigned long *count)
{
    
}

/*Reset MPU6050 Registers to default
 *The reset value is 0x00 for all registers other than the registers below
 *Register 0x72(PWR_MGMT1): 0x40
 *Register 0x75(WHO_AM_I): 0x68
 *NOTE: After I2C Write, wait 100ms for Registers to defautl
 *Return: Anything you want(Other funtion Same)
*/
uint8_t MPU_Reset(void)
{
  uint8_t MPU_CMD[]={0x80};
  return MPU_Writ_Data(MPU_ADDR, MPU_PWR_MGMT1, 1, MPU_CMD);
}

/*Set MPU6050 Power Mode
 *Parameter: PowerMode
 *It can be MPU_SLEEP MPU_CYCLE MPU_NOMAL 
 *NOTE: Sleep, MPU just do nothing(After Reset, it's already sleep mode)
 *      Nomal, aways wake up
 *      Cycle, between sleep and wake mode
 *      It can combine with MPU_TEMP_EN, MPU_TEMP_DIS
 *When set Cycle Mode, use MPU_PowerConfig(...) to config it
*/
uint8_t MPU_PowerManage(uint8_t PowerMode)
{
  return MPU_Writ_Data(MPU_ADDR, MPU_PWR_MGMT1, 1, &PowerMode);
}

/*Disable or Enable temperature sensor
 *Parameter: State
 *MPU_TEMP_EN, MPU_TEMP_DIS
*/
uint8_t MPU_Set_TempSensorSet(uint8_t State)
{
  uint8_t Tmp;
  if(MPU_Read_Data(MPU_ADDR, MPU_PWR_MGMT1, 1, &Tmp))
    return 1;
  Tmp |= State;
  if(MPU_Writ_Data(MPU_ADDR, MPU_PWR_MGMT1, 1, &Tmp))
    return 2;
  return 0;
}

/*Config MPU6050 Power Mode
 *Parameter: PowerConfig
 *It contains "Two" Parameters, Wake-up Frequency and Ban some Accelerometer or gyroscope
 *MPU_1_25Hz MPU_5Hz MPU_20Hz MPU_40Hz
 *MPU_BAN_XA, MPU_BAN_YA, MPU_BAN_ZA, MPU_BAN_XG, MPU_BAN_YG, MPU_BAN_ZG
 *E.g: MPU_5Hz|MPU_BAN_XA, MPU will wake up every 0.2s to get some data(except XA) then sleep
*/
uint8_t MPU_PowerConfig(uint8_t PowerConfig)
{
  return MPU_Writ_Data(MPU_ADDR, MPU_PWR_MGMT2, 1, &PowerConfig);
}

/*Set MPU6050 Sample Rate Divide
 *The sensor register output, FIFO output, and DMP sampling are all based on the Sample Rate.
 *Parameter: Sample_DIV(0x00 ~ 0xFF)
 *Sample Rate = Gyroscope Output Rate / (1 + Sample_DIV)
 *Gyroscope Output Rate = 8kHz when the DLPF(Digital Low Pass Filter) is disabled, 1kHz when the DLPF is enable
 *DLPF Set use MPU_DLPF_Set(...)
*/
uint8_t MPU_SampleRate_Set(uint8_t Sample_DIV)
{
  return MPU_Writ_Data(MPU_ADDR, MPU_SMPLRT_DIV, 1, &Sample_DIV);
}

/*Set MPU6050 DLPF(Digital Low Pass Filter)
 *Parameter: LPF
 *It can be MPU_LPF_None, MPU_LPF_1, MPU_LPF_2, MPU_LPF_3, MPU_LPF_4, MPU_LPF_5, MPU_LPF_6
*/
uint8_t MPU_DLPF_Set(uint8_t LPF)
{
  uint8_t tmp;
  if(MPU_Read_Data(MPU_ADDR, MPU_CONFIG, 1, &tmp))
    return 1;
  tmp &= 0xF8;
  tmp |= LPF;
  if(MPU_Writ_Data(MPU_ADDR, MPU_CONFIG, 1, &tmp))
    return 2;
  else
    return 0;
}

/*Set Gyroscope full Scale range
 *Parameter: Gyro_Range 
 *It can be MPU_250dps, MPU_500dps, MPU_1000dps, MPU_2000dps
 *NOTE: MPU_250dps(131 LSB/°/s), MPU_500dps(65.5 LSB/°/s), MPU_1000dps(32.8 LSB/°/s), MPU_2000dps(16.4 LSB/°/s)
*/
uint8_t MPU_Gyro_RangeSet(uint8_t Gyro_Range)
{
  return MPU_Writ_Data(MPU_ADDR, MPU_GYRO_CFG, 1, &Gyro_Range);
}

/*Set Accelerometer full Scale range
 *Parameter: Accel_Range
 *MPU_2G, MPU_4G, MPU_8G, MPU_16G
 *NOTE: MPU_2G(16384 LSB/g), MPU_4G(8192 LSB/g), MPU_8G(4096 LSB/g), MPU_16G(2048 LSB/g)
*/
uint8_t MPU_Accel_RangeSet(uint8_t Accel_Range)
{
  return MPU_Writ_Data(MPU_ADDR, MPU_ACCEL_CFG, 1, &Accel_Range);
}

/*Read Accelerometer original data
 *Parameter: XYZ_Accel
 *receive data pointer
*/
uint8_t MPU_Read_AccleData(int16_t *XYZ_Accel)
{
  uint8_t Data_Tmp[6];
  if(MPU_Read_Data(MPU_ADDR, MPU_ACCEL_XOUTH, 6, Data_Tmp))
    return 1;
  XYZ_Accel[0] = (int16_t)(((uint16_t)Data_Tmp[0] << 8) | Data_Tmp[1]);
  XYZ_Accel[1] = (int16_t)(((uint16_t)Data_Tmp[2] << 8) | Data_Tmp[3]);
  XYZ_Accel[2] = (int16_t)(((uint16_t)Data_Tmp[4] << 8) | Data_Tmp[5]);
  return 0;
}

/*Read Gyroscope original data
 *Parameter: XYZ_GYRO
 *receive data pointer
*/
uint8_t MPU_Read_GyroData(int16_t *XYZ_GYRO)
{
  uint8_t Data_Tmp[6];
  if(MPU_Read_Data(MPU_ADDR, MPU_GYRO_XOUTH, 6, Data_Tmp))
    return 1;
  XYZ_GYRO[0] = (int16_t)(((uint16_t)Data_Tmp[0] << 8) | Data_Tmp[1]);
  XYZ_GYRO[1] = (int16_t)(((uint16_t)Data_Tmp[2] << 8) | Data_Tmp[3]);
  XYZ_GYRO[2] = (int16_t)(((uint16_t)Data_Tmp[4] << 8) | Data_Tmp[5]);
  return 0;
}

/*Read Temperature  data
 *Parameter: Temp
 *receive data pointer
*/
uint8_t MPU_Read_TempData(int16_t *Temp)
{
  uint8_t Data_Tmp[2];
  if(MPU_Read_Data(MPU_ADDR, MPU_TEMP_OUTH, 2, Data_Tmp))
    return 1;
  Temp[0] = (int16_t)(((uint16_t)Data_Tmp[0] << 8) | Data_Tmp[1]);
  return 0;
}

/*Set INT
 *Parameter: INT_CMD
 *MPU_NONE_INT, MPU_FIFO_OFLOW_EN, MPU_I2C_MST_INT_EN, MPU_DATA_RDY_EN
 *NOTE: None
*/
uint8_t MPU_INT_ENSet(uint8_t INT_CMD)
{
    if(MPU_Writ_Data(MPU_ADDR, MPU_INT_ENABLE, 1, &INT_CMD))
        return 1;
    return 0;
}

uint8_t MPU_INTPin_Cfg(uint8_t Cgf_CMD)
{
    if(MPU_Writ_Data(MPU_ADDR, MPU_INT_PIN_CFG, 1, &Cgf_CMD))
        return 1;
    return 0;
}

//Get MPU_ID (0X68)
uint8_t MPU_WhoAreyou()
{
  uint8_t MPU_ID = 0;
  if(MPU_Read_Data(MPU_ADDR, MPU_WHO_AM_I, 1, &MPU_ID))
    return 1;
  return MPU_ID;
}

uint8_t MPU_Init(void)
{
  MPU_Reset();
  MPU_Delay(100);
  MPU_PowerManage(MPU_NOMAL);
  MPU_INT_ENSet(MPU_NONE_INT);
  MPU_INTPin_Cfg(MPU_INT_LEVEL_LOW);
  
  MPU_SampleRate_Set(9);
  MPU_DLPF_Set(MPU_LPF_5Hz);
  
  return 0;
}

uint8_t MPU_Run_Self_Test(void)
{
    int result;
    long gyro[3], accel[3];
    
    result = mpu_run_self_test(gyro, accel);
    
    if (result == 0x7) {
//	MPL_LOGI("Passed!\n");
//        MPL_LOGI("accel: %7.4f %7.4f %7.4f\n",
//                    accel[0]/65536.f,
//                    accel[1]/65536.f,
//                    accel[2]/65536.f);
//        MPL_LOGI("gyro: %7.4f %7.4f %7.4f\n",
//                    gyro[0]/65536.f,
//                    gyro[1]/65536.f,
//                    gyro[2]/65536.f);
        /* Test passed. We can trust the gyro data here, so now we need to update calibrated data*/

#ifdef USE_CAL_HW_REGISTERS
        /*
         * This portion of the code uses the HW offset registers that are in the MPUxxxx devices
         * instead of pushing the cal data to the MPL software library
         */
        unsigned char i = 0;

        for(i = 0; i<3; i++) {
        	gyro[i] = (long)(gyro[i] * 32.8f); //convert to +-1000dps
        	accel[i] *= 2048.f; //convert to +-16G
        	accel[i] = accel[i] >> 16;
        	gyro[i] = (long)(gyro[i] >> 16);
        }

        mpu_set_gyro_bias_reg(gyro);
        mpu_set_accel_bias_6050_reg(accel);
#else
        /* Push the calibrated data to the MPL library.
         *
         * MPL expects biases in hardware units << 16, but self test returns
		 * biases in g's << 16.
		 */
    	unsigned short accel_sens;
    	float gyro_sens;

		mpu_get_accel_sens(&accel_sens);
        accel_sens = 0;
		accel[0] *= accel_sens;
		accel[1] *= accel_sens;
		accel[2] *= accel_sens;
		dmp_set_accel_bias(accel);
		mpu_get_gyro_sens(&gyro_sens);
		gyro[0] = (long) (gyro[0] * gyro_sens);
		gyro[1] = (long) (gyro[1] * gyro_sens);
		gyro[2] = (long) (gyro[2] * gyro_sens);
		dmp_set_gyro_bias(gyro);
        return 0;
#endif
    }
    else {
//            if (!(result & 0x1))
//                MPL_LOGE("Gyro failed.\n");
//            if (!(result & 0x2))
//                MPL_LOGE("Accel failed.\n");
//            if (!(result & 0x4))
//                MPL_LOGE("Compass failed.\n");
        return 1;
     }

}

//方向转换
unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

//陀螺仪方向控制
unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{
    unsigned short scalar; 
    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;
    return scalar;
}

//mpu6050,dmp初始化
//返回值:0,正常
//    其他,失败
uint8_t mpu_dmp_init(void)
{
	uint8_t res=0;
	if(mpu_init()==0)	//初始化MPU6050
	{	 
		res=mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);//设置所需要的传感器
		if(res)return 1;
		res=mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);//设置FIFO
		if(res)return 2;
		res=mpu_set_sample_rate(DEFAULT_MPU_HZ);	//设置采样率
		if(res)return 3;
		res=dmp_load_motion_driver_firmware();		//加载dmp固件
		if(res)return 4;
		res=dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));//设置陀螺仪方向
		if(res)return 5;
		res=dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_TAP|	//设置dmp功能
		    DMP_FEATURE_ANDROID_ORIENT|DMP_FEATURE_SEND_RAW_ACCEL|DMP_FEATURE_SEND_CAL_GYRO|
		    DMP_FEATURE_GYRO_CAL);
		if(res)return 6;
		res=dmp_set_fifo_rate(DEFAULT_MPU_HZ);	//设置DMP输出速率(最大不超过200Hz)
		if(res)return 7;
		res=MPU_Run_Self_Test();		//自检
		if(res)return 8;
		res=mpu_set_dmp_state(1);	//使能DMP
		if(res)return 9;
	}
	return 0;
}
//得到dmp处理后的数据(注意,本函数需要比较多堆栈,局部变量有点多)
//pitch:俯仰角 精度:0.1°   范围:-90.0° <---> +90.0°
//roll:横滚角  精度:0.1°   范围:-180.0°<---> +180.0°
//yaw:航向角   精度:0.1°   范围:-180.0°<---> +180.0°
//返回值:0,正常
//    其他,失败
uint8_t mpu_dmp_get_data(float *pitch,float *roll,float *yaw)
{
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	unsigned long sensor_timestamp;
	short gyro[3], accel[3], sensors;
	unsigned char more;
	long quat[4]; 
	if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more))return 1;	 
	/* Gyro and accel data are written to the FIFO by the DMP in chip frame and hardware units.
	 * This behavior is convenient because it keeps the gyro and accel outputs of dmp_read_fifo and mpu_read_fifo consistent.
	**/
	/*if (sensors & INV_XYZ_GYRO )
	send_packet(PACKET_TYPE_GYRO, gyro);
	if (sensors & INV_XYZ_ACCEL)
	send_packet(PACKET_TYPE_ACCEL, accel); */
	/* Unlike gyro and accel, quaternions are written to the FIFO in the body frame, q30.
	 * The orientation is set by the scalar passed to dmp_set_orientation during initialization. 
	**/
	if(sensors&INV_WXYZ_QUAT)
	{
		q0 = quat[0] / q30;	//q30格式转换为浮点数
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30; 
		//计算得到俯仰角/横滚角/航向角
		*pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	// pitch
		*roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;	// roll
		*yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
	}else return 2;
	return 0;
}