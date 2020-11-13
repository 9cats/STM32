#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "beep.h"
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

int main(void) {
	/* 跑马状态，1表示跑马 */
    u8 Run_State = 1;
	
	delay_init(168);
	LED_Init();
	KEY_Init();

	while(1) {
		u32 i;
		for(i=0; i<3; i++) {
			/* 扫描KEY0是否按下，改变跑马状态 */
			if(KEY_Scan(0) == KEY0_PRES) Run_State = !Run_State; 
			switch(i) {
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
			delay_ms(300);
		}
	}
}

