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
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */
   __HAL_SPI_ENABLE(&hspi1);                    //??SPI1
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
    GPIO_InitStruct.Pull = GPIO_PULLUP;
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
//以下是SPI模块的初始化代码，配置成主机模式
//SPI口初始化
//这里针是对SPI1的初始化
void SPI1_Init(void)
{
	// GPIO_InitTypeDef GPIO_InitStructure;
	// SPI_InitTypeDef SPI_InitStructure;

	// RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  //使能SPI1时钟

	// //GPIOFB3,4,5初始化设置
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; //PB3~5复用功能输出
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;						//复用功能
	// GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//推挽输出
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//100MHz
	// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;						//上拉
	// GPIO_Init(GPIOB, &GPIO_InitStructure);								//初始化

	// GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1); //PB3复用为 SPI1
	// GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1); //PB4复用为 SPI1
	// GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1); //PB5复用为 SPI1

	// //这里只针对SPI口初始化
	// RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);  //复位SPI1
	// RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE); //停止复位SPI1

	// SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	 //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	// SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						 //设置SPI工作模式:设置为主SPI
	// SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					 //设置SPI的数据大小:SPI发送接收8位帧结构
	// SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;							 //串行同步时钟的空闲状态为高电平
	// SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;						 //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	// SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	// SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //定义波特率预分频的值:波特率预分频值为256
	// SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					 //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	// SPI_InitStructure.SPI_CRCPolynomial = 7;							 //CRC值计算的多项式
	// SPI_Init(SPI1, &SPI_InitStructure);									 //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	// SPI_Cmd(SPI1, ENABLE); //使能SPI外设

  //以上内容都已通过CubeMX配置
	SPI1_ReadWriteByte(0xff); //启动传输
}
//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256
//fAPB2时钟一般为84Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  __HAL_SPI_DISABLE(&hspi1);                    //??SPI1
	SPI1->CR1 &= 0XFFC7;											//位3-5清零，用来设置波特率
	SPI1->CR1 |= SPI_BaudRatePrescaler;								//设置SPI1速度
  __HAL_SPI_ENABLE(&hspi1);                    //??SPI1
}
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{
	while (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE) == RESET);

	SPI1->DR = TxData;

	while (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_RXNE) == RESET); //等待接收完一个byte

	return SPI1->DR; //返回通过SPIx最近接收的数据
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
