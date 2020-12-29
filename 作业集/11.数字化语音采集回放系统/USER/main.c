#include "usart.h"
#include "delay.h"
#include "lcd.h"
#include "flash.h"
#include "exti.h"
/********************************************************************\
** 作者: 9cats🐧
** 创建时间: 📅2020-12-27 🕑13:56
** 适用环境: ALIENTEK STM32F407开发板
** 📕题目基本要求：
**（1）带通滤波器：通带为 300Hz～3.4kHz ；
**（2）ADC：采样频率 f s≥8kHz，字长≥8 位；
**（3）语音存储时间≥10 秒；
**（4）DAC：变换频率 f c≥8kHz，字长≥8 位；
**（5）回放语音质量良好。
** 📗发挥部分：
**（1）减少系统噪声电平，回放的语音清晰；
**（2）增加掉电存储数据不丢失功能，再次上电后系统可直接运行上一次存储的语言数据。
**（3）增加回放音量手动控制功能，可调节回放音量大小；
** 备注: 🈚
\********************************************************************/
void showPage(u8 mode); //显示静态页面

u8 currentPage = 0; //当前页面 0为主页
u8 page = 0;		//即将要呈现的页面
int main(void)
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
	LCD_Init();
	EXTIX_Init();

	while (1)
	{

		if (currentPage != page)
		{
			showPage(currentPage = page);
		}
	}
}

void showPage(u8 mode)
{
	/* 显示边框和标题 */
	LCD_Fill(20, 20, lcddev.width - 20, lcddev.height - 20, WHITE);
	LCD_DrawRectangle(20, 20, lcddev.width - 20, lcddev.height - 20);
	LCD_ShowString(36, 25, 168, 16, 16, (u8 *)"Voice Playback System");
	LCD_DrawLine(20, 46, 220, 46);

	switch (mode)
	{
	case 0: //主页
		break;
	case 1: //主页->播放
		break;
	case 2: //主页->播放->录入
		break;
	default:
		break;
	}
}
