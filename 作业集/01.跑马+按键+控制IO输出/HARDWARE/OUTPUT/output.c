#include "output.h"
/********************************************************************
** 作者: 9cats
** 创建时间: 2020-11-13 20:00
** 适用环境: ALIENTEK STM32F407开发板
** 作用: C0,C2两个端口的输出配置
** 描述: 使能闹钟，调整配置
** 备注: 无
*********************************************************************/ 

/* 端口初始化 */
void OUTPUT_Init(void) {   
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* 使能GPIOF时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	/* 配置GPIOF */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;		//下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);				//初始化GPIO
	
	/* 配置GPIOA */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;		//下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//初始化GPIO
	
	
	/* 初始化值 */
	GPIO_SetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_2);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
}

