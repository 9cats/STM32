#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "output.h"
/********************************************************************
** 作者: 9cats🐧
** 创建时间: 📅2020-11-12 🕑21:30
** 适用环境: ALIENTEK STM32F407开发板
** 📕题目基本要求：
**（1）利用单片机实现跑马灯功能。
**（2）利用按键控制LED灯在跑马灯状态与常亮状态间变换。
**（3）按键切换状态顺畅。
** 📗发挥部分：
**（1）利用单片机I/O口输出两路完全一样的占空比递减的矩形波。（占空比至少递减四次）
**（2）利用按键控制其中一路矩形波的频率和占空比。
**（3）现场改变任意I/O口实现矩形波的输出。（需要自带电脑）
** 📙思考题：
** ♟利用串口通信实现电脑端控制单片机I/O输出。（不参与此次评分）
** 备注: 🈚
*********************************************************************/

void run_hrose(void);//跑马
void output   (void);//输出矩形波

/* 跑马状态，1表示跑马 */
u8 Run_State_change_flag = 0;
u8 key;

int main(void) {
	u32 i = 0;
	
	delay_init(168);
	/* 配置各个GPIO */
	LED_Init();
	KEY_Init();
	BEEP_Init();
	OUTPUT_Init();
	
	while(1) {
		key = KEY_Scan(0);
		delay_ms(10);
		output();
		if(key == KEY0_PRES) Run_State_change_flag = 1;
		if(i++ % 30 == 0) run_hrose();
	}
}

/*------------------------------------------------------*\
*|				    【跑马】
*|输入:无
*|返回:无
*|描述: 🐎ε=ε=ε= 
\*------------------------------------------------------*/
void run_hrose(void) {
	/* 跑马状态,1表示跑马、i为循环变量 */
	static u8 Run_State=1, i = 0;
	if(Run_State_change_flag) {
		Run_State = !Run_State;
		Run_State_change_flag = 0;
	}
	switch(i++%3) {
		case 0: 
			LED0 = 0 & Run_State;
			LED1 = 1 & Run_State;
			LED2 = 1 & Run_State;
			break;
		case 1:
			LED0 = 1 & Run_State;
			LED1 = 0 & Run_State;
			LED2 = 1 & Run_State;
			break;
		case 2: 
			LED0 = 1 & Run_State;
			LED1 = 1 & Run_State;
			LED2 = 0 & Run_State;
			break;
	}
//	LED0 = 1, LED1 = 1, LED2 = 1;
}


/*------------------------------------------------------*\
*|				    【输出信号】
*|输入:无
*|返回:无
*|描述:控制C0,C2两个输出
\*------------------------------------------------------*/
void output(void) {
	/* 频率改变标记和状态改变标记 */
	static u8 output1_cycle = 100;				//默认周期是1s
	static double output1_proportion = 1;        //默认占空比
	static u32 output0_prograss = 0, output1_prograss = 0;	//记录当前进度
	
	/* 进度分别增加 */
	output0_prograss = output0_prograss % (      100     * 5) + 1;         
	output1_prograss = output1_prograss % (output1_cycle * 5) + 1;
	
	/* 按下KEY1改变频率 */
	if(key == KEY1_PRES) {
		/* 周期变化为：100 140 180 220 20 60 -> 100*/
		output1_cycle = output1_cycle > 200? output1_cycle+40: 20;
		output0_prograss = 0;
		output1_prograss = 0;
	}
	/* 按下KEY2改变占空 */
	if(key == KEY2_PRES) {
		/* 占空比强度变化为：1 0.8 0.6 0.4 0.2 -> 1 */
		output1_proportion = output1_proportion - 0.2 ? output1_proportion-0.2: 1;
		output0_prograss = 0;
		output1_prograss = 0;
	}
	
	/* outputx_cycle决定频率，outputx_proportion决定占空比 */
	/* 占空比变化为 90% 70% 50% 30% 10% */
	if(output1_prograss % output1_cycle > (90 - output1_prograss/output1_cycle*20)*output1_proportion) 
		 OUTPUT1 = 0;
	else OUTPUT1 = 1;
	if(output0_prograss %      100      > (90 - output0_prograss/     100     *20)*       1          ) 
		 OUTPUT0 = 0;
	else OUTPUT0 = 1;
	LED2 = OUTPUT1;
}

