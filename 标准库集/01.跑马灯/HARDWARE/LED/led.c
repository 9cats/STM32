#include "led.h"

/* LED IO初始化 */
void LED_Init(void) {
	/* 定义GPIO结构体 */
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* 使能GPIOF时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	/* 配置GPIO */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;			//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);					//初始化

	/* 初始化值 */
	GPIO_SetBits(GPIOF, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);//GPIOF8,F9,F10设置高，灯灭
}

