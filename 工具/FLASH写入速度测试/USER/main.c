#include "flash.h"
#include "lcd.h"
#include "usart.h"
#include "delay.h"

int main(void)
{
	u8 datatemp[20];

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);   //延时初始化
	uart_init(115200); //串口初始化波特率为115200
	LCD_Init();		   //初始化LCD FSMC接口

	LCD_ShowString(30, 170, 200, 16, 16, "Start Read FLASH.... ");
	STMFLASH_Read(0X0800C004, (u32 *)datatemp, 20);
	LCD_ShowString(30, 170, 200, 16, 16, "The Data Readed Is:  "); //提示传送完成
	LCD_ShowString(30, 190, 200, 16, 16, datatemp);
	while (1)
	{
	}
}
