#ifndef __LED_H
#define __LED_H
#include "sys.h"

/* 端口宏定义 */
#define LED PFout(9)

void LED_Init(void);  //初始化		 				    
#endif
