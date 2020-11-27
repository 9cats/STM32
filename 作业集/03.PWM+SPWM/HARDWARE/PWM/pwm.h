#ifndef _PWM_H
#define _PWM_H
#include "sys.h"

/* 定时器2初始化 */
void TIM2_PWM_Init(u32 arr, u32 psc);
/* 定时器4初始化 */
void TIM4_PWM_Init(u32 arr, u32 psc);
/* 定时器5初始化 */
void TIM5_PWM_Init(u32 arr, u32 psc);
#endif
