// base.c⽂件
#include "base.h"
static u8 fac_us = 0; //us延时倍乘数
//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init(u8 SYSCLK)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); //SysTick频率为HCLK
    fac_us = SYSCLK;
}
void delay_ns(u8 t)
{
    do
    {
        ;
    } while (--t);
}
void delay_us(u32 nus)
{
    u32 ticks;
    u32 told, tnow, tcnt = 0;
    u32 reload = SysTick->LOAD; //LOAD的值
    ticks = nus * fac_us;       //需要的节拍数
    told = SysTick->VAL;        //刚进⼊时的计数器值
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow; //这⾥注意⼀下SYSTICK是⼀个递减的计数器就可以了.
            else
                tcnt += reload - tnow + told;
            told = tnow;
            if (tcnt >= ticks)
                break; //时间超过/等于要延迟的时间,则退出.
        }
    };
}
//延时nms
//nms:要延时的ms数
void delay_ms(u16 nms)
{
    u32 i;
    for (i = 0; i < nms; i++)
        delay_us(1000);
}
uint8_t KEY_Scan(uint8_t mode)
{
static uint8_t key_up=1;//按键按松开标志
if(mode)key_up=1; //⽀持连按
if(key_up&&(KEY0==0||KEY1==0||KEY2==0))
{
HAL_Delay(10);//去抖动
key_up=0;
if(KEY0==0)return KEY0_PRES;
else if(KEY1==0)return KEY1_PRES;
else if(KEY2==0)return KEY2_PRES;
}else if(KEY0==1&&KEY1==1&&KEY2==1)key_up=1;
return 0;// ⽆按键按下
}
