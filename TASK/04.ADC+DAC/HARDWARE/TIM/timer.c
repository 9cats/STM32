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
	extern u16 adc1;
	extern u8 mode;
			extern u8 TIM3CH2_CAPTURE_STA;
							extern u32 TIM3CH2_HIGH;
							extern u32 TIM3CH2_LOW;
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
	static u8 i = 0;
	static long long TIM3CH2_CAPTURE_VAL;
	static long long TIM3CH2_CAPTURE_CIR;

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) || TIM_GetITStatus(TIM3, TIM_IT_CC2)) //溢出中断
	{
		switch (mode)
		{
		case 0:
			if (i++ < 5)
				DAC_SetChannel1Data(DAC_Align_12b_R, 0);
			else if (i < 10)
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
		{
			if ((TIM3CH2_CAPTURE_STA & 0X80) == 0) //还未成功捕获
			{
				static u8 captureDirect = 0;		   //捕获方向
				if (TIM_GetITStatus(TIM3, TIM_IT_Update)) //溢出
				{
					if (TIM3CH2_CAPTURE_STA & 0X40) //已经捕获到高电平了
					{
						TIM3CH2_CAPTURE_CIR++;
					}
				}
				if (TIM_GetITStatus(TIM3, TIM_IT_CC2)) //捕获1发生捕获事件
				{
					if (TIM3CH2_CAPTURE_STA & 0X40) //捕获到
					{
						TIM3CH2_CAPTURE_VAL = TIM_GetCapture2(TIM3); //获取当前的捕获值.
						if (!captureDirect)							 //↑-↓
						{
							TIM3CH2_HIGH = TIM3CH2_CAPTURE_CIR * 100 + TIM3CH2_CAPTURE_VAL;
							TIM3CH2_CAPTURE_VAL = TIM3CH2_CAPTURE_CIR = 0;

							captureDirect = !captureDirect;
							TIM_Cmd(TIM3, DISABLE); //关闭定时器5
							TIM_SetCounter(TIM3, 8);
							TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising); //设置捕获方向
							TIM_Cmd(TIM3, ENABLE);
						}
						else //↑-↓-↑
						{
							TIM3CH2_LOW = TIM3CH2_CAPTURE_CIR * 100 + TIM3CH2_CAPTURE_VAL;
							TIM3CH2_CAPTURE_VAL = TIM3CH2_CAPTURE_CIR = 0;

							captureDirect = !captureDirect;
							TIM3CH2_CAPTURE_STA |= 0X80;
						}
					}
					else //还未开始,第一次捕获↑
					{
						TIM3CH2_CAPTURE_STA = 0; //清空
						TIM3CH2_CAPTURE_VAL = 0;
						TIM3CH2_CAPTURE_STA |= 0X40; //标记捕获

						TIM_Cmd(TIM3, DISABLE); //关闭定时器5
						TIM_SetCounter(TIM3, 8);
						TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling); //设置捕获方向
						TIM_Cmd(TIM3, ENABLE);
					} //使能定时器5
				}
			}
			break;
		}
		}
	}

	TIM_ClearITPendingBit(TIM3, TIM_IT_CC2 | TIM_IT_Update); //清除中断标志位
}
