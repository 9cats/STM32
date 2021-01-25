#include "lcd.h"
#include "delay.h"
#include "usart.h"
#include "touch.h"
#include "eval.h"
#include "string.h"
#include "math.h"
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
const u16 MAX = 999;
u8 buttonValue1[3][8] = {
	{'q', 'e', 'r', 't', 'y', 'u', 'i', 'o'},
	{'a', 's', 'd', 'f', 'g', 'h', 'j', 'k'},
	{'x', 'c', 'v', 'b', 'n', 'm', 'p', 'l'}};
u8 buttonValue2[5][4] = {
	{'A', '<', '(', ')'},
	{'7', '8', '9', '+'},
	{'4', '5', '6', '-'},
	{'1', '2', '3', '*'},
	{'0', '.', '=', '/'}};
u8 expression[MAX] = {0};
double result = 0;
int main(void)
{
	u16 i, j;
	char s[] = "sin(3.14)";
	delay_init(168);   //延时初始化
	uart_init(115200); //串口初始化波特率为115200
	LCD_Init();		   //初始化LCD FSMC接口
	TP_Init();		   //初始化触摸屏
	LCD_ShowString2(10,10,320,20,16,(u8*)s);
	result =  sqrt(4);
	LCD_ShowNum(10, 50, eval("SQRT(12)"), 28, 16);
	for (i = 0; i < sizeof(buttonValue1) / sizeof(buttonValue1[0]); i++)
	{
		for (j = 0; j < sizeof(buttonValue1[i]); j++)
		{
			LCD_ShowChar(j * 30 + 9, 80 + 3 + i * 30, buttonValue1[i][j], 24, 0);
		}
	}
	for (i = 0; i < sizeof(buttonValue2) / sizeof(buttonValue2[0]); i++)
	{
		for (j = 0; j < sizeof(buttonValue2[i]); j++)
		{
			LCD_ShowChar(j * 60 + 18, 170 + 3 + i * 30, buttonValue2[i][j], 24, 0);
		}
	}
	while (1)
	{
		/* 扫描触摸屏是否被按下 */
		tp_dev.scan(0);
		if (tp_dev.sta & TP_PRES_DOWN)
		{
			for (i = 0; i < sizeof(buttonValue1) / sizeof(buttonValue1[0]); i++)
			{
				for (j = 0; j < sizeof(buttonValue1[i]); j++)
				{
					if(tp_dev.x[0]>j*30     &&  tp_dev.y[0]>80+i*30 &&
					   tp_dev.x[0]<j*30+30  &&  tp_dev.y[0]<80+i*30+30)
					{
						expression[strlen((char *)expression)] = buttonValue1[i][j];
						LCD_ShowString2(10,10,320,20,16,expression);
						delay_ms(500);
						break;
					}
				}
			}
			for (i = 0; i < sizeof(buttonValue2) / sizeof(buttonValue2[0]); i++)
			{
				for (j = 0; j < sizeof(buttonValue2[i]); j++)
				{
					if(tp_dev.x[0]>j*60     &&  tp_dev.y[0]>170+i*30 &&
					   tp_dev.x[0]<j*60+60  &&  tp_dev.y[0]<170+i*30+30)
					{
						if(buttonValue2[i][j] == 'A') {
							memset(expression, 0, sizeof(expression));
							result = 0;
							LCD_Fill(0,0,320,80,WHITE);
						}
						else if(buttonValue2[i][j] == '<') {
							expression[strlen((char *)expression)-1] = 0;
							LCD_Fill(0,0,320,80,WHITE);
							LCD_ShowString2(10,10,240,20,16,expression);
						}
						else if(buttonValue2[i][j] == '=') {
							result = eval((char *)expression);
							LCD_ShowNum(10, 50, result, 28, 16);
						}
						else {
							expression[strlen((char *)expression)] = buttonValue2[i][j];
							LCD_ShowString2(10,10,240,20,16,expression);
						}
						delay_ms(500);
						break;
					}
				}
			}
		}
		else
		delay_ms(10);
	}
}
