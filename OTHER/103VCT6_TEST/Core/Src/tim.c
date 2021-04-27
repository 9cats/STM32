/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "base.h"
unsigned char dis_code[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
uint16_t tick = 0;
/* USER CODE END 0 */

TIM_HandleTypeDef htim2;

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 720;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();

    /* TIM2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void Display_7Seg(unsigned char data)
{
			
	    GPIOD->ODR = dis_code[data];
			delay_ms(200);
}	


void Display_7Seg_Single(unsigned char data,unsigned char index)
{
			switch(index)
			{
				case 1: HAL_GPIO_WritePin(LCD0_GPIO_Port,LCD0_Pin,GPIO_PIN_RESET); break;
				case 2: HAL_GPIO_WritePin(LCD1_GPIO_Port,LCD1_Pin,GPIO_PIN_RESET); break;
				case 3: HAL_GPIO_WritePin(LCD2_GPIO_Port,LCD2_Pin,GPIO_PIN_RESET); break;
				case 4: HAL_GPIO_WritePin(LCD3_GPIO_Port,LCD3_Pin,GPIO_PIN_RESET); break;
				default: GPIOC->ODR&=0xF0FF; break;  //PC.15-PC.8输出�??,4个共阴LED全亮
				
			}
	    GPIOD->ODR = dis_code[data];
			delay_ms(200);
}	


// 动�?�扫�??
void Display_7Seg_Multi(unsigned int data)
{
	if(data<9999)
	{ 
		unsigned int i;
		unsigned int s;	
		s = data;
		i=0;
		for (i=0;i<4;i++)  // 4位轮流扫
		{
			GPIOC->ODR|=0x0F00;  // 全灭
			switch(i)      
			{
				case 0: HAL_GPIO_WritePin(LCD0_GPIO_Port,LCD0_Pin,GPIO_PIN_RESET); break;
				case 1: HAL_GPIO_WritePin(LCD1_GPIO_Port,LCD1_Pin,GPIO_PIN_RESET); break;
				case 2: HAL_GPIO_WritePin(LCD2_GPIO_Port,LCD2_Pin,GPIO_PIN_RESET); break;
				case 3: HAL_GPIO_WritePin(LCD3_GPIO_Port,LCD3_Pin,GPIO_PIN_RESET); break;
      }
			GPIOD->ODR = dis_code[s%10]; 
			s = s/10;
			delay_us(2480);
			if (s==0)
				 break;
		}	 
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==&htim2)
    {
      Display_7Seg_Multi(tick++);
    }
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
