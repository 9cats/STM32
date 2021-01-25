#include "lcd.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "BongoJams.h"
/******************************************************************************************************************************************
** 作者: 9cats🐧
** 创建时间: 📅2020-11-12 🕑21:30
** 适用环境: ALIENTEK STM32F407开发板
** Bongo Jams💻🐱
** 备注: 只适用于320*240 TFT LCD
*******************************************************************************************************************************************/
u16 count = 0;
u8 codeIt = 0;

int main(void)
{
	u16 i,j=0;
	delay_init(168);   //延时初始化
	uart_init(115200); //串口初始化波特率为115200
	LCD_Init();		   //初始化LCD FSMC接口
	TIM3_Int_Init(100, 840); //溢出频率10Hz

	LCD_Clear(0x18e5);

	for (i = 0; i < sizeof(MAIN) / 6; i++)
		LCD_Fast_DrawPoint(lcddev.width - MAIN[i].x, MAIN[i].y, MAIN[i].color);
	while (1)
	{
		if (count%500/250)
			for (i = 0; i < sizeof(HAND1) / 6; i++)
				LCD_Fast_DrawPoint(lcddev.width - HAND1[i].x, HAND1[i].y, HAND1[i].color);
		else
			for (i = 0; i < sizeof(HAND2) / 6; i++)
				LCD_Fast_DrawPoint(lcddev.width - HAND2[i].x, HAND2[i].y, HAND2[i].color);
		if (codeIt)
		{
			codeIt = 0;
			LCD_Fast_DrawPoint(lcddev.width - CODE[j].x, CODE[j].y, CODE[j].color);
			if(++j == sizeof(CODE)/6) 
			{
				for (i = 0; i < sizeof(CODE) / 6; i++)
					LCD_Fast_DrawPoint(lcddev.width - CODE[i].x, CODE[i].y, 0x18e5);
				j=0;
			}
		}
	}
}
