/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "ff.h"
#include "LCD_TFT.h"
#include "OLED.h"
#include "MPU6050.h"
#include "DC_Control.h"
#include "BLE_Control.h"
#include "Warning.h"
#include "Music.h"
#include "DC_PID.h"
#include "LCD_RTS.h"
#include "Tim_Delay.h"
#include "RGB_LED.h"
#include "Key.h"
#include "Menu.h"
#include "Call_Back.h"
#include "Load.h"
#include "My_UI.h"
#include "TMP_Control.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint8_t MPU_DMP_Flg;
extern uint8_t Show_Flg;
extern float Pitch,Roll,Yaw;
extern int16_t Blance_PWM;
uint8_t InterfaceCmd[15] = {0};
uint8_t InterfaceData[15] = {0};
int16_t XXTMP = 2100;
uint8_t XXflg = 1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_FSMC_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM8_Init();
  MX_TIM9_Init();
  MX_USART1_UART_Init();
  MX_TIM5_Init();
  MX_SPI1_Init();
  MX_TIM6_Init();
  MX_SDIO_SD_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  MX_TIM7_Init();
  MX_DMA_Init();
  /* USER CODE BEGIN 2 */
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
  
  UI_init();
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  DM_DIR_CTL_L(Dir_N);
  DM_DIR_CTL_R(Dir_N);
  DM_PWM_Engage(DisEngage);
  DM_EncodeStop();
  BLE_Power(BLE_OFF);
  
  while (1)
  {
      if(MPU_DMP_Flg)
      {
          MPU_DMP_Flg = 0;
          LCD_ShowFNum(&LCD_Prop, 48, 0, Pitch, 4, 8, 1);
      }
      if(TMP_StartFlg && TMP_ShowFlg)
      {
          TMP_ShowFlg = 0;
          LCD_ShowString(&LCD_Prop, 0, 0, "Tmp Set:", 8, 1);
          LCD_ShowString(&LCD_Prop, 0, 16, "Tmp Now:", 8, 1);
          LCD_ShowFNum(&LCD_Prop, 64, 0, TMP_Set, 4, 8, 1);
          LCD_ShowFNum(&LCD_Prop, 64, 16, TMP_Now, 4, 8, 1);
      }
      Menu_Execute(Key_StateData());
      Menu_Execute(BLE_GetRemoteState());
      if(!Keyboard_StateData(&My_Keyboard, InterfaceCmd))
      {
          if(InterfaceCmd[0] == 0x00 || InterfaceCmd[0] == 0x01 || InterfaceCmd[0] == 0x02)
          {
              Menu_Execute(InterfaceCmd);
          }
//          else if(InterfaceCmd[0] == 0x01)
//          {
//              memset(InterfaceData, 0x00, 15);
//              InterfaceData[0] = InterfaceCmd[1];
//              LCD_ShowString(&LCD_Prop, 16, 0, (char*)InterfaceData, 8, 1);
//          }
//          else if(InterfaceCmd[0] == 0x02)
//          {
//          }
          else if(InterfaceCmd[0] == 0x03)
          {
              LCD_SetColor(&LCD_Prop, BLUE, BLACK);
              if(My_Keyboard.T_Flg)
              {
                  LCD_ShowString(&LCD_Prop, 0, 144, "T", 8, 1);
              }
              else if(My_Keyboard.S_Flg)
              {
                  LCD_ShowString(&LCD_Prop, 0, 144, "S", 8, 1);
              }
              else
              {
                  LCD_ShowString(&LCD_Prop, 0, 144, " ", 8, 1);
              }
          }
          
      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
