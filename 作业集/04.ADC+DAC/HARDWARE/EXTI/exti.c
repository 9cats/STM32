#include "exti.h"
#include "key.h"
#include "delay.h"
#include "lcd.h"
#include "timer.h"
/********************************************************************
** 作者: 9cats
** 创建时间: 2020-12-2 19:36
** 适用环境: ALIENTEK STM32F407开发板
** 作用: 中断函数及配置
** 资源: EXTI0,2,3,4
** 备注: 无
*********************************************************************/

//外部中断初始化程序
//初始化PE2~4,PA0为中断输入.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	KEY_Init(); //按键对应的IO口初始化

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //使能SYSCFG时钟

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2); //PE2 连接到中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3); //PE3 连接到中断线3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4); //PE4 连接到中断线4
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); //PA0 连接到中断线0

	/* 配置EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;			   //LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	   //中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;			   //使能LINE0
	EXTI_Init(&EXTI_InitStructure);						   //配置

	/* 配置EXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3 | EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//中断线使能
	EXTI_Init(&EXTI_InitStructure);							//配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			 //外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		 //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);								 //配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			 //外部中断2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		 //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);								 //配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			 //外部中断3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		 //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);								 //配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			 //外部中断4
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		 //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);								 //配置
}

u8 i;
/* 外部中断0服务程序-按下WK_UP-切换模式 */
void EXTI0_IRQHandler(void)
{
    extern u8 mode;
	delay_ms(10); //消抖
	if (WK_UP == 1)
	{
		i = 1;
        if (mode == 3) mode = 0;
        else  mode++;
	}
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位
}

/* 外部中断2服务程序-按下KEY2 */
void EXTI2_IRQHandler(void)
{
	delay_ms(10); //消抖
	if (KEY2 == 0)
	{
		if(i++ ==                                              10) i=0;
		TIM3_Int_Init(40*(i+1) - 1, 21 - 1); //初始化定时器TIM3，溢出频率为100000Hz
	}
	EXTI_ClearITPendingBit(EXTI_Line2); //清除LINE2上的中断标志位
}

/* 外部中断3服务程序-按下KEY1 */
void EXTI3_IRQHandler(void)
{
	delay_ms(10); //消抖
	if (KEY1 == 0){

    }
	EXTI_ClearITPendingBit(EXTI_Line3); //清除LINE3上的中断标志位
}

/* 外部中断4服务程序-按下KEY0 */
void EXTI4_IRQHandler(void)
{
	delay_ms(10); //消抖
	if (KEY0 == 0)
	{
	}
	EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE4上的中断标志位
}



