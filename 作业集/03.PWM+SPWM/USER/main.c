#include "pwm.h"
#include "delay.h"
#include "exti.h"
#include "timer.h"
#include "stm32f4xx_it.h"

/********************************************************************
** 作者: 9cats🐧
** 创建时间: 📅2020-11-24 🕑20:00
** 适用环境: ALIENTEK STM32F407开发板
** 📕题目基本要求：
**（1）利用定时器输出一路频率1KHZ的PWM波。
**（2）利用外部中断控制按键1改变PWM波的占空比，步进为10％。
**（3）利用外部中断控制按键2可持续性地改变PWM波的占空比，再次按下暂停占空比的改变。
** 📗发挥部分：
**（1）利用外部中断控制按键3改变PWM波的频率，步进为1KHZ。（占空比至少递减四次）
**（2）再生成一路1KHz的PWM波，利用外部中断控制按键4可持续性地改变其相位，再次按下暂停相位的改变。
**（3）利用定时器输出SPWM，频率为50Hz，并自制简易滤波电路验证输出结果。
** 备注: 🈚
*********************************************************************/
/* 从此配置TIM2 */
u32 arr = 1000;		 //TIM2的自动重装载值
u32 psc = 84;		 //TIM2的时钟预分频数
u32 CRRx = 300;		 //TIM2的CRRx,默认为300
u32 arr2 = 500;		 //TIM2的自动重装载值
u32 psc2 = 84;		 //TIM2的时钟预分频数
u32 CRRx2 = 300;	 //TIM2的CRRx,默认为300
u8 CRRx_Change = 0;	 //标记TIM2的CRRX是否持续变化
int CRRx_Way = 1;	 //标记TIM2的变大方向
u8 Pluse_Change = 0; //标记TIM4得Pluse是否持续变化
u32 arr3 = 1000;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2

	delay_init(168);				 //初始化延时函数
//	EXTIX_Init();					 //初始化外部中断输入
//	TIM2_PWM_Init(arr - 1, psc - 1); //初始化定时器TIM2，溢出频率为1kHz
//	TIM_SetCompare1(TIM2, CRRx);
//	TIM3_Int_Init(100 - 1, 8400 - 1);  //初始化定时器TIM3，溢出频率为100Hz
//	TIM4_PWM_Init(arr2 - 1, psc2 - 1); //初始化定时器TIM4，溢出频率为1kHz
	TIM5_PWM_Init(arr3 - 1, 7-1);  	   //初始化定时器TIM4，溢出频率为2kHz
	TIM5_Int_Init();

	while (1)
		;
}
