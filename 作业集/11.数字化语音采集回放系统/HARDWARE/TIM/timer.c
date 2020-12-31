#include "timer.h"
#include "adc.h"
#include "dac.h"
#include "flash.h"
/********************************************************************
** 作者: 9cats
** 创建时间: 2020-11-24 20:10
** 适用环境: ALIENTEK STM32F407开发板
** 作用: 配置定时器和定时器中断服务函数
** 资源: TIM3、TIM3_CH2 & GPIOA_PIN7
** 备注: 
*********************************************************************/
#define ADDRBEG 0x08020000
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
	TIM_Cmd(TIM3, DISABLE);					   //失能定时器3

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				 //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		 //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

extern u32 addrP;
extern u32 adcCount;
extern u8 taskStatus;
extern u8 currentPage;
//TODO:测试
extern u32 vol;
/* 定时器3中断服务函数 */
void TIM3_IRQHandler(void)
{
	static u8 step = 0;
	static u16 DATE_BUFF[2] = {0};
	if (TIM_GetITStatus(TIM3, TIM_IT_Update)) //溢出中断
	{
		if (!taskStatus)
		{
			addrP = ADDRBEG; //地址回到最开始
			step = 0;
			*(u32 *)DATE_BUFF = 0;
		}
		switch (currentPage)
		{
		case 0: //播放
			taskStatus = 1;
			if (!step)
			{
				*(u32 *)DATE_BUFF = STMFLASH_ReadWord(addrP);
			}
			//TODO:vol测试用
			DAC_SetChannel1Data(DAC_Align_12b_R, vol = DATE_BUFF[step]);
			if (step++)
			{
				step = 0;
				addrP += 4;
			}
			break;
		case 2: //录入
			taskStatus = 1;
			//TODO:vol测试用
			vol = DATE_BUFF[step] = Get_Adc(6);
			//TODO:测试
			if (step++)
			{
				FLASH_ProgramWord(addrP, *(u32 *)DATE_BUFF);
				step = 0;
				addrP += 4;
			}
			break;
		default:
			break;
		}
		if (adcCount++ > 300000)
		{
			TIM_Cmd(TIM3, DISABLE);
			adcCount = 0;
			taskStatus = 2;
		}
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除中断标志位
}
