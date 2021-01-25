#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h" 

/* 端口宏定义 */
#define BEEP PDout(11)	// 蜂鸣器控制IO 

void BEEP_Init(void);	// 蜂鸣器初始化		 				    
#endif

















