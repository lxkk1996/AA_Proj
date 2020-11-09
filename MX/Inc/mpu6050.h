#ifndef __MPU6050_H
#define __MPU6050_H

#include <stdint.h>
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
/*include your I2C drive head file
 *The following functions must be defined
 *MPU_Delay(uint32_t(or other) num_ms)
 *I2C read writ define in MPU6050.c
*/
#include "i2c.h"

#define   MPU_ADDR              0XD0
#define   MPU_Delay             HAL_Delay
#define   MPU_Time_Out          5
//Self Test REG
#define   MPU_SELF_TESTX        0X0D
#define   MPU_SELF_TESTY        0X0E
#define   MPU_SELF_TESTZ		    0X0F
#define   MPU_SELF_TESTA		    0X10
//Sample Rate Divider REG
#define   MPU_SMPLRT_DIV	    	0X19
//Configuration REG
#define   MPU_CONFIG			    	0X1A
//Configuration BITMASK
#define   MPU_LPF_None          0x00
#define   MPU_LPF_184Hz         0x01
#define   MPU_LPF_94_Hz         0x02
#define   MPU_LPF_44Hz          0x03
#define   MPU_LPF_21Hz          0x04
#define   MPU_LPF_10Hz          0x05
#define   MPU_LPF_5Hz           0x06
//Gyroscope Configuration REG
#define   MPU_GYRO_CFG          0X1B
//Gyroscope Configuration BITMASK
#define   MPU_250dps            0x00
#define   MPU_500dps            0x08
#define   MPU_1000dps           0x10
#define   MPU_2000dps           0x18
//Accelerometer Configuration REG
#define   MPU_ACCEL_CFG		      0X1C
//Accelerometer Configuration BITMASK
#define   MPU_2G                0x00
#define   MPU_4G                0x08
#define   MPU_8G                0x10
#define   MPU_16G               0x18

#define   MPU_FIFO_EN           0X23	//FIFO使能寄存器

#define   MPU_I2C_MST_CTRL	  	0X24	//IIC主机控制寄存器
#define   MPU_I2C_SLV0_ADDR	    0X25	//IIC从机0器件地址寄存器
#define   MPU_I2C_SLV0_REG      0X26	//IIC从机0数据地址寄存器
#define   MPU_I2C_SLV0_CTRL	    0X27	//IIC从机0控制寄存器
#define   MPU_I2C_SLV1_ADDR   	0X28	//IIC从机1器件地址寄存器
#define   MPU_I2C_SLV1_REG      0X29	//IIC从机1数据地址寄存器
#define   MPU_I2C_SLV1_CTRL   	0X2A	//IIC从机1控制寄存器
#define   MPU_I2C_SLV2_ADDR	    0X2B	//IIC从机2器件地址寄存器
#define   MPU_I2C_SLV2_REG      0X2C	//IIC从机2数据地址寄存器
#define   MPU_I2C_SLV2_CTRL	    0X2D	//IIC从机2控制寄存器
#define   MPU_I2C_SLV3_ADDR	    0X2E	//IIC从机3器件地址寄存器
#define   MPU_I2C_SLV3_REG      0X2F	//IIC从机3数据地址寄存器
#define   MPU_I2C_SLV3_CTRL   	0X30	//IIC从机3控制寄存器
#define   MPU_I2C_SLV4_ADDR	    0X31	//IIC从机4器件地址寄存器
#define   MPU_I2C_SLV4_REG      0X32	//IIC从机4数据地址寄存器
#define   MPU_I2C_SLV4_DO		    0X33	//IIC从机4写数据寄存器
#define   MPU_I2C_SLV4_CTRL   	0X34	//IIC从机4控制寄存器
#define   MPU_I2C_SLV4_DI		    0X35	//IIC从机4读数据寄存器
#define   MPU_I2C_MST_STATUS    0X36	//IIC主机状态寄存器

#define   MPU_INT_PIN_CFG       0X37	//中断/旁路设置寄存器
#define   MPU_INT_LEVEL_LOW     0x80

#define   MPU_INT_ENABLE        0X38	//中断使能寄存器
#define   MPU_NONE_INT          0x00
#define   MPU_FIFO_OFLOW_EN     0x10
#define   MPU_I2C_MST_INT_EN    0x08
#define   MPU_DATA_RDY_EN       0x01

#define   MPU_INT_STATUS        0X3A	//中断状态寄存器

//Accelerometer Measurements REG
#define   MPU_ACCEL_XOUTH		    0X3B
#define   MPU_ACCEL_XOUTL	    	0X3C
#define   MPU_ACCEL_YOUTH       0X3D
#define   MPU_ACCEL_YOUTL	    	0X3E
#define   MPU_ACCEL_ZOUTH       0X3F
#define   MPU_ACCEL_ZOUTL		    0X40
//Temperature Measurement REG
#define   MPU_TEMP_OUTH         0X41
#define   MPU_TEMP_OUTL         0X42
//Gyroscope Measurements REG
#define   MPU_GYRO_XOUTH		    0X43
#define   MPU_GYRO_XOUTL	    	0X44
#define   MPU_GYRO_YOUTH	    	0X45
#define   MPU_GYRO_YOUTL		    0X46
#define   MPU_GYRO_ZOUTH	    	0X47
#define   MPU_GYRO_ZOUTL	    	0X48

#define   MPU_I2C_SLV0_DO		    0X63	//IIC从机0数据寄存器
#define   MPU_I2C_SLV1_DO	    	0X64	//IIC从机1数据寄存器
#define   MPU_I2C_SLV2_DO		    0X65	//IIC从机2数据寄存器
#define   MPU_I2C_SLV3_DO		    0X66	//IIC从机3数据寄存器

#define   MPU_I2C_MST_DELAY	    0X67	//IIC主机延时管理寄存器
#define   MPU_SIGPATH_RST		    0X68	//信号通道复位寄存器

#define   MPU_USER_CTRL         0X6A	//用户控制寄存器

#define   MPU_PWR_MGMT1	        0X6B	//电源管理寄存器1
#define   MPU_SLEEP             0x40
#define   MPU_CYCLE             0x20
#define   MPU_NOMAL             0x01
#define   MPU_TEMP_EN           0x00
#define   MPU_TEMP_DIS          0x08

#define   MPU_PWR_MGMT2         0X6C	//电源管理寄存器2 
#define   MPU_1_25Hz            0X00
#define   MPU_5Hz               0x40
#define   MPU_20Hz              0x80
#define   MPU_40Hz              0xD0

#define   MPU_BAN_XA            0x20
#define   MPU_BAN_YA            0x10
#define   MPU_BAN_ZA            0x08
#define   MPU_BAN_XG            0x04
#define   MPU_BAN_YG            0x02
#define   MPU_BAN_ZG            0x01

#define   MPU_FIFO_CNTH         0X72	//FIFO计数寄存器高八位
#define   MPU_FIFO_CNTL         0X73	//FIFO计数寄存器低八位
#define   MPU_FIFO_RW           0X74	//FIFO读写寄存器
#define   MPU_WHO_AM_I          0X75	//器件ID寄存器


extern int16_t ACC_DATA[3];
extern double  ACC_Real[3];
extern int16_t GYRO_DATA[3];
extern double  GYRO_Real[3];
uint8_t MPU_Writ_Data(uint8_t ucSlave_Addr, uint8_t ucReg_Addr,uint16_t usData_Size, uint8_t *pData);
uint8_t MPU_Read_Data(uint8_t ucSlave_Addr, uint8_t ucReg_Addr,uint16_t usData_Size, uint8_t *pData);
void MPU_GetMs(unsigned long *count);
uint8_t MPU_Reset();
uint8_t MPU_WhoAreyou();
uint8_t MPU_Init();
uint8_t MPU_Read_AccleData(int16_t *XYZ_Accel);
uint8_t MPU_Read_GyroData(int16_t *XYZ_GYRO);
uint8_t mpu_dmp_get_data(float *pitch,float *roll,float *yaw);

uint8_t mpu_dmp_init(void);

#endif