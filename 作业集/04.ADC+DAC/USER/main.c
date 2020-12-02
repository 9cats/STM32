#include "lcd.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"
#include "dac.h"
#include "timer.h"
#include "exti.h"
/********************************************************************
** 作者: 9cats🐧
** 创建时间: 📅2020-12-1 🕑22:58
** 适用环境: ALIENTEK STM32F407开发板
** 📕题目基本要求：
**（1）ADC采样直流信号0-3.3V， DAC输出1KHZ的PWM波（峰值和直流电压相等），误差50mV以内。
**（2）ADC采集0-100Hz正弦信号 DAC输出原正弦信号。
**（3）按键ADC控制采样率。
** 📗发挥部分：
**（1）ADC采样交流信号1kHz（0-3.3V）VPP正弦信号显示峰峰值、有效值、偏置。
**（2）能对1Hz-10kHz的方波信号测频。
**（3）在发挥部分（2）的基础上同时可以测量方波占空比。
** 备注: 🈚
*********************************************************************/
u16 adc1 = 0; //adc采样值
u8 mode = 0;
//0：采样直流信号0-3.3V，DAC输出1KHZ的PWM波
//1：ADC采集0-100Hz正弦信号 DAC输出原正弦信号
//2: ADC采样交流信号1kHz（0-3.3V）VPP正弦信号显示峰峰值、有效值、偏置
//3: 对1Hz-10kHz的方波信号测频同时可以测量方波占空比

int main(void)
{
	float temp, temp2;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init(168);								//延时初始化
	uart_init(115200);								//串口初始化波特率为115200
	EXTIX_Init();									//初始化外部中断
	LCD_Init();										//初始化LCD
	Adc1_Init();									//初始化ADC1
	Dac1_Init();									//初始化DAC1

	TIM3_Int_Init(10 - 1, 84 - 1); //初始化定时器TIM3，溢出频率为10000Hz
	LCD_ShowString(30, 130, 200, 16, 16, (u8 *)"ADC1_CH6_VAL:");
	LCD_ShowString(30, 150, 200, 16, 16, (u8 *)"ADC1_CH6_VOL:0.000V"); //先在固定位置显示小数点
	while (1)
	{
		switch (mode)
		{
		case 0:
		{
			adc1 = Get_Adc_Average(ADC_Channel_6, 20); //获取通道6的转换值，20次取平均
			LCD_ShowxNum(134, 130, adc1, 4, 16, 0);	   //显示ADCC采样后的原始值
			temp = (float)adc1 * (3.0 / 4096);   	   //获取计算后的带小数的实际电压值，比如3.1111
			temp2 = (u16)temp;						   //赋值整数部分给adc1变量，因为adc1为u16整形
			LCD_ShowxNum(134, 150, temp2, 1, 16, 0);   //显示电压值的整数部分，3.1111的话，这里就是显示3
			temp -= temp2;							   //把已经显示的整数部分去掉，留下小数部分，比如3.1111-3=0.1111
			temp *= 1000;							   //小数部分乘以1000，例如：0.1111就转换为111.1，相当于保留三位小数。
			LCD_ShowxNum(150, 150, temp, 3, 16, 0X80); //显示小数部分（前面转换为了整形显示），这里显示的就是111.
		}break;
		case 1:
		{
			adc1 = Get_Adc_Average(ADC_Channel_6, 20);
		}break;
		}

		//Dac1_Set_Vol(2000);

		//TODO:表格
	}
}
