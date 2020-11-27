#include "pwm.h"

/********************************************************************
** 作者: 9cats
** 创建时间: 2020-11-24 20:10
** 适用环境: ALIENTEK STM32F407开发板
** 作用: 配置PWM输出
** 资源: TIM2 GPIOA PA0 /TIM4 GPIOB PB6/ 定时器5 GPIOA PA1
** 备注: 
*********************************************************************/

extern u32 CRRx;
//TIM2 PWM部分初始化
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数

/* 定时器2初始化 */
void TIM2_PWM_Init(u32 arr, u32 psc)
{
	//此部分需手动修改IO口设置

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //TIM2时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能PORTF时钟

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2); //GPIOF9复用为定时器14

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		   //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	   //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);			   //初始化PF9

	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr;						//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //初始化定时器14

	//初始化TIM2 Channel1 PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);					  //根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能TIM2在CCR1上的预装载寄存器

	TIM_ARRPreloadConfig(TIM2, ENABLE); //ARPE使能

	TIM_Cmd(TIM2, ENABLE); //使能TIM2
}


/* 定时器4初始化 */
void TIM4_PWM_Init(u32 arr, u32 psc)
{
	//此部分需手动修改IO口设置

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //TIM2时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能PORTF时钟

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4); //GPIOF9复用为定时器14

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;		   //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	   //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   //上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);			   //初始化PF9

	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr;						//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //初始化定时器14

	//初始化TIM2 Channel1 PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;			  //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  //输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);					  //根据T指定的参数初始化外设TIM1 4OC1
	

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); //使能TIM2在CCR1上的预装载寄存器

	TIM_ARRPreloadConfig(TIM4, ENABLE); //ARPE使能

	TIM_Cmd(TIM4, ENABLE); //使能TIM2
}

/* 定时器5初始化 */
void TIM5_PWM_Init(u32 arr, u32 psc)
{
	//此部分需手动修改IO口设置

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //TIM2时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能PORTF时钟

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5); //GPIOF9复用为定时器14

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		   //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	   //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);			   //初始化PF9

	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr;						//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //初始化定时器14

	//初始化TIM2 Channel1 PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  //输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_Pulse = CRRx;
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);					  //根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable); //使能TIM2在CCR1上的预装载寄存器

	TIM_ARRPreloadConfig(TIM5, ENABLE); //ARPE使能

	TIM_Cmd(TIM5, ENABLE); //使能TIM2
}



