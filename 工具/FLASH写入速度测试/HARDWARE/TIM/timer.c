#include "timer.h"
/********************************************************************
** 作者: 9cats
** 创建时间: 2020-11-24 20:10
** 适用环境: ALIENTEK STM32F407开发板
** 作用: 配置定时器和定时器中断服务函数
** 资源: TIM3、TIM3_CH2 & GPIOA_PIN7
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		 //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM3_Int_Init2(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM3_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  ///使能TIM3时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能PORTA时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		   //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	   //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	   //下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);			   //初始化PA0

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3); //复用

	TIM_TimeBaseInitStructure.TIM_Period = arr;						//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;					//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); //初始化TIM3

	//初始化TIM3输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2;				 //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	 //上升沿捕获
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			 //配置输入分频,不分频
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;						 //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	TIM_ITConfig(TIM3, TIM_IT_Update | TIM_IT_CC2, ENABLE); //允许定时器3更新中断 ,允许CC1IE捕获中断
	TIM_Cmd(TIM3, ENABLE);									//使能定时器3

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				 //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		 //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/* 定时器3中断服务函数 */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) || TIM_GetITStatus(TIM3, TIM_IT_CC2)) //溢出中断
	{
	}

	TIM_ClearITPendingBit(TIM3, TIM_IT_CC2 | TIM_IT_Update); //清除中断标志位
}
