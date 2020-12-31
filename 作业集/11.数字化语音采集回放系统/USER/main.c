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
u32 vol = 0;
//TODO:
void showPage(u8 mode);						 //显示静态页面
void consoleLog(char *String);				 //输出当前进度
void Play(void);							 //播放
void Record(void);							 //录音
u8 TP_CHECK(u16 x0, u16 y0, u16 x1, u16 y1); //检测是否在某区域按下
u8 currentPage = 0;							 //当前页面 0为主页
u8 page = 0;								 //即将要呈现的页面
u8 presStatus = 0;							 //记录触摸屏的按下情况，用于防止连按
u8 taskStatus = 0;							 //用于标记写入开始 0未完成 1完成
u32 adcCount = 0;							 //ADC采样数
u32 addrP = ADDRBEG;						 //当前指向地址
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
					if (TP_CHECK(25, 195, 115, 295))
					{ //按下Record
						page = 1;
					}
					if (TP_CHECK(125, 195, 215, 295))
					{ //按下Play
						Play();
					}
					break;
				case 1: //主页->播放
					if (TP_CHECK(25, 195, 115, 295))
					{ //按下Confirm
						page = 2;
					}
					if (TP_CHECK(125, 195, 215, 295))
					{ //按下Cancel
						page = 0;
					}
					break;
				case 2: //主页->播放->录入
					if (TP_CHECK(25, 195, 115, 295))
					{ //按下Begin
						Record();
					}
					if (TP_CHECK(125, 195, 215, 295))
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
/* 播放 */
void Play(void)
{
	//TODO:解码方式修改
	consoleLog("Playing...");
	LCD_ShowString(25, 150, 48, 16, 16, (u8 *)" VOL :");
	LCD_ShowString(25, 170, 48, 16, 16, (u8 *)"COUNT:");
	LCD_DrawLine(40, 100, 40, 120);
	LCD_DrawLine(200, 100, 200, 120);

	TIM_Cmd(TIM3, ENABLE); //使能定时器3
	taskStatus = 0;

	while (taskStatus != 2)
	{
		LCD_ShowNum(73, 150, vol, 6, 16); //测试
		LCD_ShowNum(73, 170, adcCount, 6, 16);
		LCD_DrawLine(40 + adcCount * 160 / 300000, 100, 40 + adcCount * 160 / 300000, 120);
	}
	taskStatus = 0;
	TIM_Cmd(TIM3, DISABLE); //失能定时器3

	consoleLog("Playing finished");
	LCD_Fill(40, 100, 200, 120, WHITE);
	LCD_Fill(21, 150, 219, 190, WHITE);
}

/* 录音 */
void Record(void)
{
	//TODO:存码方式待修改
	consoleLog("Sampling and recording...");
	LCD_ShowString(25, 150, 48, 16, 16, (u8 *)" VOL :");
	LCD_ShowString(25, 170, 48, 16, 16, (u8 *)"COUNT:");
	LCD_DrawLine(40, 100, 40, 120);
	LCD_DrawLine(200, 100, 200, 120);

	FLASH_Unlock();				 //解锁
	FLASH_DataCacheCmd(DISABLE); //FLASH写入期间,必须禁止数据缓存
	TIM_Cmd(TIM3, ENABLE);		 //使能定时器3
	taskStatus = 0;

	while (taskStatus != 2)
	{
		LCD_ShowNum(73, 150, vol, 6, 16); //测试
		LCD_ShowNum(73, 170, adcCount, 6, 16);
		LCD_DrawLine(40 + adcCount * 160 / 300000, 100, 40 + adcCount * 160 / 300000, 120);
	}

	//TODO:之后显示动态进度条
	taskStatus = 0;				//开始写入标记清零
	TIM_Cmd(TIM3, DISABLE);		//失能定时器3
	FLASH_DataCacheCmd(ENABLE); //FLASH写入结束,开启数据缓存
	FLASH_Lock();				//上锁
	page = 0;
}

/* 检测是否在某区域按下 */
u8 TP_CHECK(u16 x0, u16 y0, u16 x1, u16 y1)
{
	return (tp_dev.x[0] > x0 && tp_dev.y[0] > y0 && tp_dev.x[0] < x1 && tp_dev.y[0] < y1);
}

/* 显示不同页面 */
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
		if (STMFLASH_ReadWord(ADDRBEG) != 0xffffffff)
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

/* 打印信息 */
void consoleLog(char *String)
{
	LCD_Fill(25, 60, lcddev.width - 25, 92, WHITE);
	LCD_ShowString(25, 60, 190, 32, 16, (u8 *)String);
}
