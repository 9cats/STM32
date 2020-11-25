#include "timer.h"

/********************************************************************
** 作者: 9cats
** 创建时间: 2020-11-24 20:10
** 适用环境: ALIENTEK STM32F407开发板
** 作用: 配置PWM输出
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

    TIM_TimeBaseInitStructure.TIM_Period = arr;                     //自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                  //定时器分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); //初始化TIM3

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //允许定时器3更新中断
    TIM_Cmd(TIM3, ENABLE);                     //使能定时器3

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;              //定时器3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;        //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    extern int CRRx_Way;
    extern u8 CRRx_Change;
    extern u32 arr;
    extern u32 CRRx;
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //溢出中断
    {
        if (CRRx_Change)
        {
            if(CRRx == arr) CRRx_Way = -1;
			if(CRRx == 0  ) CRRx_Way =  1;
            CRRx += CRRx_Way;
            
            TIM_SetCompare1(TIM2, CRRx);
        }
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除中断标志位
}
