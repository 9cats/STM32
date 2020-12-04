#include "lcd.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"
#include "dac.h"
#include "timer.h"
#include "exti.h"
#include "math.h"
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
void showPage(u8 mode); //显示静态页面

u16 adc1 = 0; //adc采样值
u8 mode = 0;
//0：采样直流信号0-3.3V，DAC输出1KHZ的PWM波
//1：ADC采集0-100Hz正弦信号 DAC输出原正弦信号
//2: ADC采样交流信号1kHz（0-3.3V）VPP正弦信号显示峰峰值、有效值、偏置
//3: 对1Hz-10kHz的方波信号测频同时可以测量方波占空比
u8 toGet = 0; //表示正弦波时是否采样
//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
u8 TIM3CH2_CAPTURE_STA = 0; //输入捕获状态
long long TIM3CH2_HIGH = 0;
long long TIM3CH2_LOW = 0;

int main(void)
{
	float temp;
	u8 currentMode = 0; //当前正在经行的模式

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init(168);								//延时初始化
	uart_init(115200);								//串口初始化波特率为115200
	EXTIX_Init();									//初始化外部中断
	LCD_Init();										//初始化LCD
	Adc1_Init();									//初始化ADC1
	Dac1_Init();									//初始化DAC1

	showPage(mode);
	while (1)
	{
		switch (mode)
		{
		case 0: //0：采样直流信号0-3.3V，DAC输出1KHZ的PWM波
		{
			adc1 = Get_Adc_Average(ADC_Channel_6, 20);					   //获取通道6的转换值，20次取平均
			LCD_ShowxNum(62, 100, adc1, 4, 16, 0);						   //显示ADCC采样后的原始值
			temp = (float)adc1 * (3.0 / 4096);							   //获取计算后的带小数的实际电压值，比如3.1111
			LCD_ShowxNum(62, 120, (u16)temp, 1, 16, 0);					   //显示电压值的整数部分，3.1111的话，这里就是显示3
			LCD_ShowxNum(78, 120, (temp - (u16)temp) * 1000, 3, 16, 0X80); //显示小数部分（前面转换为了整形显示），这里显示的就是111.
		}
		break;
		case 1: //1：ADC采集0-100Hz正弦信号 DAC输出原正弦信号
		{
			adc1 = Get_Adc(6);
		}
		break;
		case 2: //2: ADC采样交流信号1kHz（0-3.3V）VPP正弦信号显示峰峰值、有效值、偏置
		{
			static u8 i = 0;
			static double VMAX = 0, VMIN = 3, VRMS = 0, OFFSET = 0;
			if (toGet)
			{
				adc1 = Get_Adc(6);
				temp = (float)adc1 * (3.0 / 4096);
				if (temp > VMAX)
					VMAX = temp;
				if (temp < VMIN)
					VMIN = temp;
				VRMS += temp * temp;
				OFFSET += temp;
				toGet = !toGet;
				if (++i == 100)
				{
					VRMS = sqrt(VRMS / 100);
					OFFSET = OFFSET / 100;
					LCD_ShowxNum(86, 100, (u16)(VMAX - VMIN), 1, 16, 0);							  //显示电压值的整数部分，3.1111的话，这里就是显示3
					LCD_ShowxNum(102, 100, ((VMAX - VMIN) - (u16)(VMAX - VMIN)) * 1000, 3, 16, 0X80); //显示小数部分（前面转换为了整形显示），这里显示的就是111.
					LCD_ShowxNum(86, 120, (u16)VRMS, 1, 16, 0);										  //显示电压值的整数部分，3.1111的话，这里就是显示3
					LCD_ShowxNum(102, 120, (VRMS - (u16)VRMS) * 1000, 3, 16, 0X80);					  //显示小数部分（前面转换为了整形显示），这里显示的就是111.
					LCD_ShowxNum(86, 140, (u16)OFFSET, 1, 16, 0);									  //显示电压值的整数部分，3.1111的话，这里就是显示3
					LCD_ShowxNum(102, 140, (OFFSET - (u16)OFFSET) * 1000, 3, 16, 0X80);				  //显示小数部分（前面转换为了整形显示），这里显示的就是111.
					VMAX = VRMS = OFFSET = i = 0;
					VMIN = 3;
					delay_ms(300);
				}
			}
		}
		break;
		case 3:
		{
			if (TIM3CH2_CAPTURE_STA & 0x80)
			{
				temp = (float)21e6 / (TIM3CH2_HIGH + TIM3CH2_LOW);
				LCD_ShowxNum(110, 100, (u32)temp, 5, 16, 0);
				temp = (u32)((temp - (u32)temp) * 100);
				if (temp < 10)
				{
					LCD_ShowxNum(158, 100, 0, 1, 16, 0);
					LCD_ShowxNum(166, 100, temp, 1, 16, 0);
				}
				else
					LCD_ShowxNum(158, 100, temp, 1, 16, 0);
				temp = (float)TIM3CH2_HIGH / (TIM3CH2_HIGH + TIM3CH2_LOW) * 100;
				LCD_ShowxNum(110, 120, (u32)temp, 5, 16, 0);
				temp = (u32)((temp - (u32)temp) * 100);
				if (temp < 10)
				{
					LCD_ShowxNum(158, 120, 0, 1, 16, 0);
					LCD_ShowxNum(166, 120, temp, 1, 16, 0);
				}
				else
					LCD_ShowxNum(158, 120, temp, 1, 16, 0);
				delay_ms(600);
				TIM3CH2_CAPTURE_STA = 0;
			}
		}
		break;
		}
		if (mode != currentMode)
		{
			temp = 0;
			showPage(currentMode = mode);
		}
		//Dac1_Set_Vol(2000);
		//TODO:表格
	}
}

void showPage(u8 mode)
{
	LCD_Fill(20, 20, lcddev.width - 20, lcddev.height - 20, WHITE);
	LCD_DrawRectangle(20, 20, lcddev.width - 20, lcddev.height - 20);
	switch (mode)
	{
	case 0:
		TIM3_Int_Init(100 - 1, 84 - 1); //初始化定时器TIM3，溢出频率为100000Hz
		LCD_ShowString(30, 30, 200, 16, 16, (u8 *)"Mode:0");
		LCD_ShowString(30, 50, 200, 16, 16, (u8 *)"Sample DC singal and");
		LCD_ShowString(30, 70, 200, 16, 16, (u8 *)"Output PWM");
		LCD_ShowString(30, 100, 200, 16, 16, (u8 *)"VAL:");
		LCD_ShowString(30, 120, 200, 16, 16, (u8 *)"VOL:0.000V"); //先在固定位置显示小数点
		break;
	case 1:
		TIM3_Int_Init(40 - 1, 21 - 1); //初始化定时器TIM3，溢出频率为100000Hz
		LCD_ShowString(30, 30, 200, 16, 16, (u8 *)"Mode:1");
		LCD_ShowString(30, 50, 200, 16, 16, (u8 *)"Sample Sin singal and");
		LCD_ShowString(30, 70, 200, 16, 16, (u8 *)"Output Sin");
		LCD_ShowString(30, 100, 200, 16, 16, (u8 *)" SAMPLING...");
		LCD_ShowString(30, 120, 200, 16, 16, (u8 *)"OUTPUTING...");
		break;
	case 2:
		TIM3_Int_Init(10 - 1, 84 - 1); //初始化定时器TIM3，溢出频率为100000Hz
		LCD_ShowString(30, 30, 200, 16, 16, (u8 *)"Mode:2");
		LCD_ShowString(30, 50, 200, 16, 16, (u8 *)"Sample Sin singal and");
		LCD_ShowString(30, 70, 200, 16, 16, (u8 *)"Calculate");
		LCD_ShowString(30, 100, 200, 16, 16, (u8 *)"VPP   :0.000V");
		LCD_ShowString(30, 120, 200, 16, 16, (u8 *)"VRMS  :0.000V"); //先在固定位置显示小数点
		LCD_ShowString(30, 140, 200, 16, 16, (u8 *)"OFFSET:0.000V"); //先在固定位置显示小数点
		break;
	case 3:
		TIM3CH2_CAPTURE_STA = 0;
		TIM3_Int_Init2(100 - 1, 4 - 1); //初始化定时器TIM3，溢出频率为0.25MHz
		LCD_ShowString(30, 30, 200, 16, 16, (u8 *)"Mode:3");
		LCD_ShowString(30, 50, 200, 16, 16, (u8 *)"Sample wave singal and");
		LCD_ShowString(30, 70, 200, 16, 16, (u8 *)"Calculate");
		LCD_ShowString(30, 100, 200, 16, 16, (u8 *)"frequency:     .  Hz");
		LCD_ShowString(30, 120, 200, 16, 16, (u8 *)"dutycycle:     .  %"); //先在固定位置显示小数点
		break;
	default:
		break;
	}
}
