#include "flash.h"
#include "lcd.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"

u32 count = 0;

int main(void)
{
	FLASH_Status status = FLASH_COMPLETE;
	u32 addrx = 0x0800c004;
	u32 WriteAddr = 0x0800c004;
	u32 endaddr = 0x080FFFFF + 1;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);   //延时初始化
	uart_init(115200); //串口初始化波特率为115200
	LCD_Init();		   //初始化LCD FSMC接口
	TIM3_Int_Init(10000 - 1, 84 - 1);//溢出频率为100

	if (addrx < STM32_FLASH_BASE || addrx % 4)
		return 0;					 //非法地址
	FLASH_Unlock();				 //解锁
	FLASH_DataCacheCmd(DISABLE); //FLASH擦除期间,必须禁止数据缓存

	if (addrx < 0X1FFF0000)				  //只有主存储区,才需要执行擦除操作!!
	{
		while (addrx < endaddr) //扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
		{
			if (STMFLASH_ReadWord(addrx) != 0XFFFFFFFF) //有非0XFFFFFFFF的地方,要擦除这个扇区
			{
				status = FLASH_EraseSector(STMFLASH_GetFlashSector(addrx), VoltageRange_3); //VCC=2.7~3.6V之间!!
				if (status != FLASH_COMPLETE)
					break; //发生错误了
			}
			else
				addrx += 4;
		}
	}
	if (status == FLASH_COMPLETE)
	{
		while (WriteAddr < endaddr) //写数据
		{
			if (FLASH_ProgramWord(WriteAddr, 0) != FLASH_COMPLETE) //写入数据
			{
				break; //写入异常
			}
			WriteAddr += 4;
		}
		FLASH_DataCacheCmd(ENABLE); //FLASH擦除结束,开启数据缓存
		FLASH_Lock();				//上锁
	}

	LCD_ShowNum(30,30,count,5,16);
	while (1)
	{
	}
}
