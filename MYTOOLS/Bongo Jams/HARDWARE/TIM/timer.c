#include "timer.h"
#include "lcd.h"
/********************************************************************
** 作者: 9cats
** 创建时间: 2020-11-24 20:10
** 适用环境: ALIENTEK STM32F407开发板
** 作用: 配置定时器和定时器中断服务函数
** 资源: TIM3
** 备注: 
*********************************************************************/
//arr：自动重装值
//psc：时钟预分频数

/* 定时器3初始化 */
void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); ///使能TIM3时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr;						//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;					//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); //初始化TIM3

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3, ENABLE);					   //使能定时器3

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				 //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		 //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

extern u16 count;
extern u8 codeIt;
/* 定时器3中断服务函数 */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update)) //溢出中断
	{
		if(++count == 1000)count = 0;
		if (!(count%3))codeIt = 1;
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除中断标志位
}
