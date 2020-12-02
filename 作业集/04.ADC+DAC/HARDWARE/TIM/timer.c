#include "timer.h"
#include "adc.h"
/********************************************************************
** 作者: 9cats
** 创建时间: 2020-11-24 20:10
** 适用环境: ALIENTEK STM32F407开发板
** 作用: 配置定时器和定时器中断服务函数
** 资源: TIM3、TIM5
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

/* 定时器3中断服务函数 */
void TIM3_IRQHandler(void)
{
  extern u16 adc1;
  extern u8 mode;
  static u8 i = 0;

  switch (mode)
  {
  case 0:
  {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //溢出中断
    {
      if (i++ < 5)     DAC_SetChannel1Data(DAC_Align_12b_R,  0  );
      else if (i < 9)  DAC_SetChannel1Data(DAC_Align_12b_R, adc1);
      else i = 0;
    }
  }
  break;
  case 1:
  {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //溢出中断
    {
		adc1 = Get_Adc(6);
		DAC_SetChannel1Data(DAC_Align_12b_R, adc1);
    }
  }
  break;
  case 2:
  {
  }break;
  default:
    break;
  }

  TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除中断标志位
}
