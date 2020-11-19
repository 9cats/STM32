#include "led.h"
#include "lcd.h"
#include "usart.h"
#include "delay.h"
#include "key.h"
#include "touch.h"
#include "eval.h"

/******************************************************************************************************************************************
** 作者: 9cats🐧
** 创建时间: 📅2020-11-12 🕑21:30
** 适用环境: ALIENTEK STM32F407开发板
** 📕题目基本要求：
** (1) 在LCD屏和电脑串口上显示自己的学号、姓名的拼音和圆周率数字π(精确到小数后7位) 
** (2) 通过触屏控制一个led灯:在屏幕上设计一个”on”按钮和”off”按钮，按”on”按钮，亮；按”off”按钮，灯灭。
** (3) 通过串口发送数字实现任意实数加一减一，并在LCD屏和电脑串口上显示出来。
** (4) 用串口发送指令和触屏实现二级界面功能及其切换。
** 📗发挥部分：
** (5) 实现实数加减乘除运算功能：在串口中输出运算数A、运算符、运算数B，在电脑串口上显示结果，同时能在屏幕上显示“A运算符B=结果”
** (6) 使用汉字显示
** (7) 自由发挥
** 备注: 🈚
*******************************************************************************************************************************************/

int main(void)
{ 
	u8 *str = (u8 *)malloc(USART_REC_LEN * sizeof(u8));	//用于存储单次串口接收到的信息
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置系统中断优先级分组2
	delay_init(168);								//延时初始化
	uart_init(115200);								//串口初始化波特率为115200
	LED_Init();										//初始化与LED连接的硬件接口
	LCD_Init();										//初始化LCD FSMC接口
	TP_Init();										//初始化触摸屏

	/* 在LCD上显示信息 */
	LCD_ShowString(  30,   0,  200,  16,  16,"Num :201921111066");	
	LCD_ShowString(  30,  20,  200,  16,  16,"Name:Huang Yi");	
	LCD_ShowString(  30,  40,  200,  16,  16," PI :3.1415926");
	LCD_Fill( 30, 60, 100, 100, GREEN);
	LCD_Fill(140, 60, 210, 100, GRAY);
	LCD_ShowString2(  35,  75,   70,  16,  16,"Turn On");
	LCD_ShowString2( 145,  75,   70,  16,  16,"Turn Off");

	/* 向串口输出信息 */
	printf("学号:201921111066\r\n");
	printf("姓名:Huang Yi\r\n");
	printf("P I :3.1415926\r\n");


	while(1) {
		/* 检测串口是否接收完成 */
		if(USART_RX_STA&0x8000) {
			u16 i,len;				//i为循环变量,len标记接收到的长度
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度

			/* 将接收到的值计算，把计算式及结果赋值给str */
			sprintf((char *)str,"%s=%.0f\n", USART_RX_BUF, eval((char *)USART_RX_BUF));

			/* 清空接收到的信息 */
			for(i=0;i<USART_REC_LEN;i++) USART_RX_BUF[i] = 0;

			/* 向LCD和串口打印计算式及结果 */
			printf("%s", str);
			LCD_Fill( 30, 120, 210, 140, WHITE);	//先清空该区域
			LCD_ShowString2( 30,  120,   200,  16,  16, str);


			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);	//等待发送结束
			USART_RX_STA=0;										 	//接收状态清空 
		}

		/* 扫描触摸屏是否被按下 */
		tp_dev.scan(0);
		if(tp_dev.sta&TP_PRES_DOWN) {
			//判断按下位置是否正常
		 	if(tp_dev.x[0]<lcddev.width  &&  tp_dev.y[0]<lcddev.height) {
				//分别按下 Turn on  和  Turn off
				if(tp_dev.x[0]> 30 && tp_dev.y[0]> 60 && tp_dev.x[0]<100 && tp_dev.y[0]<100) LED = 0;
				if(tp_dev.x[0]>140 && tp_dev.y[0]> 60 && tp_dev.x[0]<210 && tp_dev.y[0]<100) LED = 1;
			}
		}else delay_ms(10);
	}
}

