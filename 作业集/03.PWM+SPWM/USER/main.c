#include "pwm.h"
#include "delay.h"
#include "usart.h"

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

int main(void)
{
	u16 led0pwmval = 0;
	u8 dir = 1;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init(168);								//初始化延时函数
	uart_init(115200);								//初始化串口波特率为115200
	TIM2_PWM_Init(500 - 1, 84 - 1);					//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.
	while (1)										//实现比较值从0-300递增，到300后从300-0递减，循环
	{}
}

// /*------------------------------------------------------*\
// *|				    【跑马】
// *|输入:无
// *|返回:无
// *|描述: 🐎ε=ε=ε=
// \*------------------------------------------------------*/
// void run_hrose(void) {
// 	/* 跑马状态,1表示跑马、i为循环变量 */
// 	static u8 Run_State=1, i = 0;
// 	if(Run_State_change_flag) {
// 		Run_State = !Run_State;
// 		Run_State_change_flag = 0;
// 	}
// 	switch(i++%3) {
// 		case 0:
// 			LED0 = 0 & Run_State;
// 			LED1 = 1 & Run_State;
// 			LED2 = 1 & Run_State;
// 			break;
// 		case 1:
// 			LED0 = 1 & Run_State;
// 			LED1 = 0 & Run_State;
// 			LED2 = 1 & Run_State;
// 			break;
// 		case 2:
// 			LED0 = 1 & Run_State;
// 			LED1 = 1 & Run_State;
// 			LED2 = 0 & Run_State;
// 			break;
// 	}
// //	LED0 = 1, LED1 = 1, LED2 = 1;
// }

// /*------------------------------------------------------*\
// *|				    【输出信号】
// *|输入:无
// *|返回:无
// *|描述:控制C0,C2两个输出
// \*------------------------------------------------------*/
// void output(void) {
// 	/* 频率改变标记和状态改变标记 */
// 	static u32 output1_cycle = 100;				//默认周期是1s
// 	static double output1_proportion = 1;        //默认占空比
// 	static u32 output0_prograss = 0, output1_prograss = 0, output2_prograss;	//记录当前进度

// 	/* 进度分别增加 */
// 	output0_prograss = output0_prograss % (      100     * 5) + 1;
// 	output1_prograss = output1_prograss % (output1_cycle * 5) + 1;
// 	output2_prograss = output2_prograss % (      100     * 5) + 1;

// 	/* 按下KEY1改变频率 */
// 	if(key == KEY1_PRES) {
// 		/* 周期变化为：100 140 180 220 20 60 -> 100*/
// 		output1_cycle = (output1_cycle > 200)? 20 : output1_cycle+40;
// 		output0_prograss = 0;
// 		output1_prograss = 0;
// 	}
// 	/* 按下KEY2改变占空 */
// 	if(key == KEY2_PRES) {
// 		/* 占空比强度变化为：1 0.8 0.6 0.4 0.2 -> 1 */
// 		output1_proportion = output1_proportion - 0.2 ? output1_proportion-0.2: 1;
// 		output0_prograss = 0;
// 		output1_prograss = 0;
// 	}

// 	/* outputx_cycle决定频率，outputx_proportion决定占空比 */
// 	/* 占空比变化为 90% 70% 50% 30% 10% */
// 	if(output1_prograss % output1_cycle > (90 - output1_prograss/output1_cycle*20)*output1_proportion)
// 		 OUTPUT1 = 0;
// 	else OUTPUT1 = 1;
// 	if(output0_prograss %      100      > (90 - output0_prograss/     100     *20)*       1          )
// 		 OUTPUT0 = 0;
// 	else OUTPUT0 = 1;
// 	if(output2_prograss %      100      > (50                                     )*       1          )
// 		 OUTPUT2 = 0;
// 	else OUTPUT2 = 1;

// }
