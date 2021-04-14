/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_2
#define LED3_GPIO_Port GPIOC
#define LED4_Pin GPIO_PIN_3
#define LED4_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_4
#define KEY1_GPIO_Port GPIOC
#define KEY2_Pin GPIO_PIN_5
#define KEY2_GPIO_Port GPIOC
#define DATA8_Pin GPIO_PIN_8
#define DATA8_GPIO_Port GPIOD
#define DATA9_Pin GPIO_PIN_9
#define DATA9_GPIO_Port GPIOD
#define DATA10_Pin GPIO_PIN_10
#define DATA10_GPIO_Port GPIOD
#define DATA11_Pin GPIO_PIN_11
#define DATA11_GPIO_Port GPIOD
#define DATA12_Pin GPIO_PIN_12
#define DATA12_GPIO_Port GPIOD
#define DATA13_Pin GPIO_PIN_13
#define DATA13_GPIO_Port GPIOD
#define DATA14_Pin GPIO_PIN_14
#define DATA14_GPIO_Port GPIOD
#define DATA15_Pin GPIO_PIN_15
#define DATA15_GPIO_Port GPIOD
#define KEY3_Pin GPIO_PIN_6
#define KEY3_GPIO_Port GPIOC
#define KEY4_Pin GPIO_PIN_7
#define KEY4_GPIO_Port GPIOC
#define LCD3_Pin GPIO_PIN_8
#define LCD3_GPIO_Port GPIOC
#define LCD2_Pin GPIO_PIN_9
#define LCD2_GPIO_Port GPIOC
#define LCD1_Pin GPIO_PIN_10
#define LCD1_GPIO_Port GPIOC
#define LCD0_Pin GPIO_PIN_11
#define LCD0_GPIO_Port GPIOC
#define DATA0_Pin GPIO_PIN_0
#define DATA0_GPIO_Port GPIOD
#define DATA1_Pin GPIO_PIN_1
#define DATA1_GPIO_Port GPIOD
#define DATA2_Pin GPIO_PIN_2
#define DATA2_GPIO_Port GPIOD
#define DATA3_Pin GPIO_PIN_3
#define DATA3_GPIO_Port GPIOD
#define DATA4_Pin GPIO_PIN_4
#define DATA4_GPIO_Port GPIOD
#define DATA5_Pin GPIO_PIN_5
#define DATA5_GPIO_Port GPIOD
#define DATA6_Pin GPIO_PIN_6
#define DATA6_GPIO_Port GPIOD
#define DATA7_Pin GPIO_PIN_7
#define DATA7_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
