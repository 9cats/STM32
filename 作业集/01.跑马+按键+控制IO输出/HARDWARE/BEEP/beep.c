#include "beep.h" 
/********************************************************************
** 作者: 9cats
** 创建时间: 2020-11-13 10:40
** 适用环境: ALIENTEK STM32F407开发板
** 作用: 蜂鸣器驱动
** 描述: 使能闹钟，调整配置
** 备注: 无
*********************************************************************/ 

/* 蜂鸣器初始化 */
void BEEP_Init(void) {   
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* 使能GPIOF时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	/* 配置GPIOF */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;		//下拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);				//初始化GPIO
	
	/* 初始化值 */
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);  //蜂鸣器对应引脚GPIOF8拉低， 
}






