#ifndef __OUTPUT_H
#define __OUTPUT_H
#include "sys.h"

/* 端口宏定义 */
#define OUTPUT0 PCout(0)
#define OUTPUT1 PCout(2)
#define OUTPUT2 PAout(1)

void OUTPUT_Init(void);	//出口端口初始化
#endif
