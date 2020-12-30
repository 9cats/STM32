#include "usart.h"
#include "delay.h"
#include "lcd.h"
#include "flash.h"
#include "exti.h"
#include "touch.h"
#include "timer.h"
#include "adc.h"
#include "dac.h"
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
#define ADDRBEG 0x08020000
#define ADDREND 0x080FFFFF
//TODO:测试用
int vol = 0;
//TODO:
void showPage(u8 mode);		   //显示静态页面
void consoleLog(char *String); //输出当前进度
u8 currentPage = 0;			   //当前页面 0为主页
u8 page = 0;				   //即将要呈现的页面
u8 presStatus = 0;			   //记录触摸屏的按下情况，用于防止连按
u8 taskStatus = 0;			   //用于标记写入开始 0开始写入 1写入中 2写入完成
u32 adcCount = 0;			   //ADC采样数
u32 addrP = ADDRBEG;		   //当前指向地址
int main(void)
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
	LCD_Init();
	TP_Init();
	EXTIX_Init();
	Adc1_Init();				   //初始化ADC1
	Dac1_Init();				   //初始化DAC1
	TIM3_Int_Init(50 - 1, 84 - 1); //初始化定时器TIM3，溢出频率为20KHz

	showPage(0);
	while (1)
	{
		tp_dev.scan(0);
		if (tp_dev.sta & TP_PRES_DOWN)
		{
			if (presStatus == 0)
			{
				presStatus = 1; //标记已经按下
				switch (currentPage)
				{
				case 0: //主页
					if (tp_dev.x[0] > 25 && tp_dev.y[0] > 195 && tp_dev.x[0] < 115 && tp_dev.y[0] < 295)
					{ //按下Record
						page = 1;
					}
					if (tp_dev.x[0] > 125 && tp_dev.y[0] > 195 && tp_dev.x[0] < 215 && tp_dev.y[0] < 295)
					{ //按下Play
						consoleLog("Playing...");
						TIM_Cmd(TIM3, ENABLE); //使能定时器3
						taskStatus = 0;
						while (taskStatus != 2)
						{
							LCD_ShowNum(20, 130, vol, 16, 16); //测试
							LCD_ShowNum(20, 150, adcCount, 16, 16);
						}
						taskStatus = 0;
						TIM_Cmd(TIM3, DISABLE); //失能定时器3
						consoleLog("Playing finished");
					}
					break;
				case 1: //主页->播放
					if (tp_dev.x[0] > 25 && tp_dev.y[0] > 195 && tp_dev.x[0] < 115 && tp_dev.y[0] < 295)
					{ //按下Confirm
						page = 2;
					}
					if (tp_dev.x[0] > 125 && tp_dev.y[0] > 195 && tp_dev.x[0] < 215 && tp_dev.y[0] < 295)
					{ //按下Cancel
						page = 0;
					}
					break;
				case 2: //主页->播放->录入
					if (tp_dev.x[0] > 25 && tp_dev.y[0] > 195 && tp_dev.x[0] < 115 && tp_dev.y[0] < 295)
					{ //按下Begin
						consoleLog("Sampling and recording...");
						FLASH_Unlock();				 //解锁
						FLASH_DataCacheCmd(DISABLE); //FLASH写入期间,必须禁止数据缓存
						TIM_Cmd(TIM3, ENABLE);		 //使能定时器3
						taskStatus = 0;
						while (taskStatus != 2)
						{
							LCD_ShowNum(20, 130, vol, 16, 16); //测试
							LCD_ShowNum(20, 150, adcCount, 16, 16);
						}
						//TODO:之后显示动态进度条
						taskStatus = 0;				//开始写入标记清零
						TIM_Cmd(TIM3, DISABLE);		//失能定时器3
						FLASH_DataCacheCmd(ENABLE); //FLASH写入结束,开启数据缓存
						FLASH_Lock();				//上锁
						page = 0;
					}
					if (tp_dev.x[0] > 125 && tp_dev.y[0] > 195 && tp_dev.x[0] < 215 && tp_dev.y[0] < 295)
					{ //按下cancel
						page = 0;
					}
					break;
				default:
					break;
				}
			}
		}
		else
			presStatus = 0; //标记没有继续按下

		if (currentPage != page)
		{
			showPage(currentPage = page);
		}
	}
}

void showPage(u8 page)
{
	/* 显示边框和标题 */
	LCD_Fill(20, 20, lcddev.width - 20, lcddev.height - 20, WHITE);
	LCD_DrawRectangle(20, 20, lcddev.width - 20, lcddev.height - 20);
	LCD_ShowString(36, 25, 168, 16, 16, (u8 *)"Voice Playback System");
	LCD_DrawLine(20, 46, 220, 46);
	/* 显示两个圆框 */
	LCD_Draw_Circle(20 + 50, 240, 45);
	LCD_Draw_Circle(20 + 150, 240, 45);
	switch (page)
	{
	case 0: //主页
		LCD_ShowString(20 + 50 - 12 * 3, 240 - 12, 12 * 6, 24, 24, (u8 *)"Record");
		LCD_ShowString(20 + 150 - 12 * 2, 240 - 12, 12 * 4, 24, 24, (u8 *)"Play");
		if (STMFLASH_ReadWord(ADDRBEG))
			consoleLog("Loaded video");
		else
			consoleLog("No video");
		break;
	case 1: //主页->录入
		LCD_ShowString(20 + 50 - 6 * 7, 240 - 12, 12 * 7, 24, 24, (u8 *)"Confirm");
		LCD_ShowString(20 + 150 - 6 * 6, 240 - 12, 12 * 6, 24, 24, (u8 *)"Cancel");
		consoleLog("Sure to erase the Flash?");
		break;
	case 2: //主页->录入->确认
		LCD_ShowString(20 + 50 - 6 * 5, 240 - 12, 12 * 6, 24, 24, (u8 *)"Begin");
		LCD_ShowString(20 + 150 - 6 * 6, 240 - 12, 12 * 6, 24, 24, (u8 *)"Cancel");
		consoleLog("Flash Erasing...");
		STMFLASH_Clear(ADDRBEG, ADDREND);
		consoleLog("Flash deleted");
		break;
	default:
		break;
	}
}

void consoleLog(char *String)
{
	LCD_Fill(25, 60, lcddev.width - 25, 92, WHITE);
	LCD_ShowString(25, 60, 190, 32, 16, (u8 *)String);
}
