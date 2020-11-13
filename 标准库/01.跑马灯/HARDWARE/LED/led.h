#ifndef __LED_H
#define __LED_H
#include "sys.h"

/* 端口宏定义 */
#define LED0 PFout(8) //DS0
#define LED1 PFout(9) //DS1
#define LED2 PFout(10)//DS2

void LED_Init(void);  //初始化		 				    
#endif
