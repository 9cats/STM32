/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_ENABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PB3     ------> SPI1_SCK
    PB4     ------> SPI1_MISO
    PB5     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PB3     ------> SPI1_SCK
    PB4     ------> SPI1_MISO
    PB5     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);

  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void SPI1_Init(void)
{
	// GPIO_InitTypeDef GPIO_InitStructure;
	// SPI_InitTypeDef SPI_InitStructure;

	// RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //ʹ��GPIOAʱ��
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  //ʹ��SPI1ʱ��

	// //GPIOFB3,4,5��ʼ����??
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; //PB3~5���ù������
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;						//���ù���
	// GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//�������
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//100MHz
	// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;						//����
	// GPIO_Init(GPIOB, &GPIO_InitStructure);								//��ʼ??

	// GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1); //PB3����?? SPI1
	// GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1); //PB4����?? SPI1
	// GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1); //PB5����?? SPI1

	// //����ֻ���SPI�ڳ�ʼ��
	// RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);  //��λSPI1
	// RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE); //ֹͣ��λSPI1

	// SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	 //����SPI�����???˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	// SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						 //����SPI����ģʽ:����Ϊ��SPI
	// SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					 //����SPI�����ݴ�??:SPI��???��??8λ֡�ṹ
	// SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;							 //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�??
	// SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;						 //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ���??
	// SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							 //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ��??
	// SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //���岨����Ԥ��Ƶ��???:������Ԥ��ƵֵΪ256
	// SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					 //ָ�����ݴ����MSBλ����LSBλ��??:���ݴ����MSBλ��??
	// SPI_InitStructure.SPI_CRCPolynomial = 7;							 //CRCֵ����Ķ���??
	// SPI_Init(SPI1, &SPI_InitStructure);									 //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ�??

	// SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����

	SPI1_ReadWriteByte(0xff); //��������
}
//SPI1�ٶ����ú���
//SPI�ٶ�=fAPB2/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256
//fAPB2ʱ��??��Ϊ84Mhz??
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	// assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); //�ж���Ч??
	// SPI1->CR1 &= 0XFFC7;											//??3-5���㣬�������ò�����
	// SPI1->CR1 |= SPI_BaudRatePrescaler;								//����SPI1�ٶ�
	// SPI_Cmd(SPI1, ENABLE);											//ʹ��SPI1
}
//SPI1 ��д??����??
//TxData:Ҫд����ֽ�
//����??:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{
  u8 chr = 0;
	while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_RESET)
	{
	} //�ȴ���???��??

  HAL_SPI_Transmit(&hspi1, &chr, 50, 10);
	// SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx��???һ��byte  ����

	while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_RESET)
	{
	} //�ȴ�������һ��byte

  return HAL_SPI_Receive(&hspi1, &chr, 1, 10);
  return chr;
	// return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx??�����յ�����
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
