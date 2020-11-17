#include "sys.h"
#include "led.h"
#include "lcd.h"
#include "usart.h"
#include "delay.h"

/********************************************************************
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
*********************************************************************/

int main(void)
{ 























	/* LCD基本代码  */
 	u8 x=0;
	u8 lcd_id[12];				//存放LCD ID字符串
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);      //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	
	LED_Init();					  //初始化LED
 	LCD_Init();           //初始化LCD FSMC接口
	POINT_COLOR=RED;      //画笔颜色：红色
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。				 	
  	while(1) 
	{		 
		switch(x)
		{
			case 0:LCD_Clear(WHITE);break;
			case 1:LCD_Clear(BLACK);break;
			case 2:LCD_Clear(BLUE);break;
			case 3:LCD_Clear(RED);break;
			case 4:LCD_Clear(MAGENTA);break;
			case 5:LCD_Clear(GREEN);break;
			case 6:LCD_Clear(CYAN);break; 
			case 7:LCD_Clear(YELLOW);break;
			case 8:LCD_Clear(BRRED);break;
			case 9:LCD_Clear(GRAY);break;
			case 10:LCD_Clear(LGRAY);break;
			case 11:LCD_Clear(BROWN);break;
		}
		POINT_COLOR=RED;	  
		LCD_ShowString(30,40,210,24,24,"Explorer STM32F4");	
		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
		LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
 		LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID	      					 
		LCD_ShowString(30,130,200,12,12,"2014/5/4");	      					 
	  x++;
		if(x==12)x=0;
		LED0=!LED0;	 
		delay_ms(1000);	
	} 
}


