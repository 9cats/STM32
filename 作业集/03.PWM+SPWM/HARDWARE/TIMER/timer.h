#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

/* 定时器3、5初始化 */
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM5_Int_Init(u16 arr,u16 psc);
#endif
