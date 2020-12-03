#include "timer.h"
#include "adc.h"
/********************************************************************
** 作者: 9cats
** 创建时间: 2020-11-24 20:10
** 适用环境: ALIENTEK STM32F407开发板
** 作用: 配置定时器和定时器中断服务函数
** 资源: TIM3、TIM3_CH2 & GPIOA_PIN7
** 备注: 
*********************************************************************/
extern u8 toGet;
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

void TIM3_Int_Init(u16 arr, u16 psc)
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

	TIM_TimeBaseInitStructure.TIM_Period = arr;						//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;					//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); //初始化TIM3

	//初始化TIM5输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2;				 //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	 //上升沿捕获
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			 //配置输入分频,不分频
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;						 //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	TIM_ITConfig(TIM3, TIM_IT_Update | TIM_IT_CC1, ENABLE); //允许定时器3更新中断 ,允许CC1IE捕获中断
	TIM_Cmd(TIM3, ENABLE);									//使能定时器3

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				 //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		 //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
u8 TIM5CH1_CAPTURE_STA = 0; //输入捕获状态
u32 TIM5CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
/* 定时器3中断服务函数 */
void TIM3_IRQHandler(void)
{
	extern u16 adc1;
	extern u8 mode;
	static u8 i = 0;

	if (TIM_GetITStatus(TIM3, TIM_IT_Update)||TIM_GetITStatus(TIM3, TIM_IT_CC1)) //溢出中断
	{
		switch (mode)
		{
		case 0:
			if (i++ < 5)
				DAC_SetChannel1Data(DAC_Align_12b_R, 0);
			else if (i < 9)
				DAC_SetChannel1Data(DAC_Align_12b_R, adc1);
			else
				i = 0;
			break;
		case 1:
			DAC_SetChannel1Data(DAC_Align_12b_R, adc1);
			break;
		case 2:
			toGet = 1;
			break;
		case 3:

			break;
		}
	}

	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 | TIM_IT_Update); //清除中断标志位
}
