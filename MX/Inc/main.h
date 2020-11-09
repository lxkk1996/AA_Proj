/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Test_Pin GPIO_PIN_5
#define Test_GPIO_Port GPIOE
#define RGB_G_Pin GPIO_PIN_0
#define RGB_G_GPIO_Port GPIOC
#define RGB_R_Pin GPIO_PIN_1
#define RGB_R_GPIO_Port GPIOC
#define RGB_B_Pin GPIO_PIN_2
#define RGB_B_GPIO_Port GPIOC
#define BEEP_Pin GPIO_PIN_0
#define BEEP_GPIO_Port GPIOA
#define CS_Pin GPIO_PIN_4
#define CS_GPIO_Port GPIOA
#define LCD_PEN_INT_Pin GPIO_PIN_4
#define LCD_PEN_INT_GPIO_Port GPIOC
#define LCD_PEN_INT_EXTI_IRQn EXTI4_IRQn
#define MPU_INT_Pin GPIO_PIN_12
#define MPU_INT_GPIO_Port GPIOB
#define MPU_INT_EXTI_IRQn EXTI15_10_IRQn
#define LCD_BL_Pin GPIO_PIN_13
#define LCD_BL_GPIO_Port GPIOB
#define BLE_POW_Pin GPIO_PIN_11
#define BLE_POW_GPIO_Port GPIOD
#define LCD_RST_Pin GPIO_PIN_3
#define LCD_RST_GPIO_Port GPIOG
#define KEY_OK_Pin GPIO_PIN_4
#define KEY_OK_GPIO_Port GPIOG
#define KEY_LEFT_Pin GPIO_PIN_5
#define KEY_LEFT_GPIO_Port GPIOG
#define KEY_DOWN_Pin GPIO_PIN_6
#define KEY_DOWN_GPIO_Port GPIOG
#define KEY_RIGHT_Pin GPIO_PIN_7
#define KEY_RIGHT_GPIO_Port GPIOG
#define KEY_UP_Pin GPIO_PIN_8
#define KEY_UP_GPIO_Port GPIOG
#define BLE_AT_Pin GPIO_PIN_12
#define BLE_AT_GPIO_Port GPIOA
#define INPUT_B2_Pin GPIO_PIN_12
#define INPUT_B2_GPIO_Port GPIOG
#define INPUT_B1_Pin GPIO_PIN_13
#define INPUT_B1_GPIO_Port GPIOG
#define INPUT_A2_Pin GPIO_PIN_14
#define INPUT_A2_GPIO_Port GPIOG
#define INPUT_A1_Pin GPIO_PIN_15
#define INPUT_A1_GPIO_Port GPIOG
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
