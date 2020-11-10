#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"

//STM32F4工程模板-库函数版本

int main(void)
{
 
	delay_init(168);		  //???????
	LED_Init();
	
  /**?????????????????IO??**/	
	
	while(1)
	{
	GPIO_ResetBits(GPIOF,GPIO_Pin_9);  //LED0????GPIOF.9??,?  ??LED0=0;
	GPIO_SetBits(GPIOF,GPIO_Pin_10);   //LED1????GPIOF.10??,? ??LED1=1;
	delay_ms(500);  		   //??300ms
	GPIO_SetBits(GPIOF,GPIO_Pin_9);	   //LED0????GPIOF.0??,?  ??LED0=1;
	GPIO_ResetBits(GPIOF,GPIO_Pin_10); //LED1????GPIOF.10??,? ??LED1=0;
	delay_ms(500);                     //??300ms
	}
}

/*
手册中讲解到步骤15的时候的main.c源码如下：
#include "stm32f4xx.h"

//STM32F4工程模板-库函数版本
  
void Delay(__IO uint32_t nCount);

void Delay(__IO uint32_t nCount)
{
  while(nCount--){}
}

int main(void)
{

  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  while(1){
		GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
		Delay(0x7FFFFF);
		GPIO_ResetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
		Delay(0x7FFFFF);
	
	}
}
*/
