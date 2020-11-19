#include "led.h"
#include "lcd.h"
#include "usart.h"
#include "delay.h"
#include "key.h"
#include "touch.h"
#include "eval.h"

/********************************************************************
** 作者: 9cats🐧
** 创建时间: 📅2020-11-12 🕑21:30
** 适用环境: ALIENTEK STM32F407开发板
** 📕题目基本要求：
** (1) 在LCD屏和电脑串口上显示自己的学号、姓名的拼音和圆周率数字π(精确到小数后7位) 
** (2) 通过触屏控制一个led灯:在屏幕上设计一个”on”按钮和”off”按钮，按”on”按钮，亮；按”off”按钮，灯灭。
** (3) 通过串口发送数字实现任意实数加一减一，并在LCD屏和电脑串口上显示出来。
** (4) 用串口发送指令和触屏实现二级界面功能及其切换。
** 📗发挥部分：
** (5) 实现实数加减乘除运算功能：在串口中输出运算数A、运算符、运算数B，在电脑串口上显示结果，同时能在屏幕上显示“A运算符B=结果”
** (6) 使用汉字显示
** (7) 自由发挥
** 备注: 🈚
*********************************************************************/


/* 求PI函数，eps表示精度 */
/*
double GetPI(double eps) {
    double n=1,i=1;
    double PI=0;
    while(1/n>=eps){
        PI += 1/n*i;
		n  += 2;
		i  = -i;
    }
    return 4*PI;
}
*/


int main(void)
{ 
	u32 len;
	u8 *str = (u8 *)malloc(USART_REC_LEN + 1);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
	LED_Init();
	LCD_Init();
	TP_Init();

	
	LCD_ShowString(  30,   0,  200,  16,  16,"Num :201921111066");	
	LCD_ShowString(  30,  20,  200,  16,  16,"Name:Huang Yi");	
	LCD_ShowString(  30,  40,  200,  16,  16," PI :3.1415926");
	LCD_Fill( 30, 60, 100, 100, GREEN);
	LCD_Fill(140, 60, 210, 100, GRAY);
	LCD_ShowString2(  35,  75,   70,  16,  16,"Turn On");
	LCD_ShowString2( 145,  75,   70,  16,  16,"Turn Off");
	printf("学号:201921111066\r\n");
	printf("姓名:Huang Yi\r\n");
	printf("P I :3.1415926\r\n");
	while(1) {
		/* 检测串口是否检测到 */
		if(USART_RX_STA&0x8000) {
			u16 i;				   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			sprintf((char *)str,"%s=%.0f\n", USART_RX_BUF, eval((char *)USART_RX_BUF));
			for(i=0;i<USART_REC_LEN;i++) USART_RX_BUF[i] = 0;
			printf("%s", str);
			LCD_Fill( 30, 120, 210, 140, WHITE);
			LCD_ShowString2( 30,  120,   200,  16,  16, str);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			USART_RX_STA=0;
		}


		tp_dev.scan(0);
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
		 	if(tp_dev.x[0]<lcddev.width  &&  tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]> 30 && tp_dev.y[0]> 60 && tp_dev.x[0]<100 && tp_dev.y[0]<100) LED = 0;
				if(tp_dev.x[0]>140 && tp_dev.y[0]> 60 && tp_dev.x[0]<210 && tp_dev.y[0]<100) LED = 1;
			}
		}else delay_ms(10);
	}
	

	// 串口通信
	// u8 t;
	// u8 len;	
	// u16 times=0;  
	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	// delay_init(168);		//延时初始化 
	// uart_init(115200);	//串口初始化波特率为115200
	// LED_Init();		  		//初始化与LED连接的硬件接口  
	// while(1)
	// {
	// 	if(USART_RX_STA&0x8000)
	// 	{					   
	// 		len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
	// 		printf("\r\n您发送的消息为:\r\n");
	// 		for(t=0;t<len;t++)
	// 		{
	// 			USART_SendData(USART1, USART_RX_BUF[t]);         //向串口1发送数据
	// 			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	// 		}
	// 		printf("\r\n\r\n");//插入换行
	// 		USART_RX_STA=0;
	// 	}else
	// 	{
	// 		times++;
	// 		if(times%5000==0)
	// 		{
	// 			printf("\r\nALIENTEK 探索者STM32F407开发板 串口实验\r\n");
	// 			printf("正点原子@ALIENTEK\r\n\r\n\r\n");
	// 		}
	// 		if(times%200==0)printf("请输入数据,以回车键结束\r\n");  
	// 		if(times%30==0)LED0=!LED0;//闪烁LED,提示系统正在运行.
	// 		delay_ms(10);   
	// 	}
	// }






	// LCD
	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	// delay_init(168);  //初始化延时函数
	// uart_init(115200);		//初始化串口波特率为115200
	
	// LED_Init();					//初始化LED 
 	// LCD_Init();					//LCD初始化 
	// KEY_Init(); 				//按键初始化  
	// tp_dev.init();				//触摸屏初始化
 	// POINT_COLOR=RED;//设置字体为红色 
	// LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	// LCD_ShowString(30,70,200,16,16,"TOUCH TEST");	
	// LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	// LCD_ShowString(30,110,200,16,16,"2014/5/7");
   	// if(tp_dev.touchtype!=0XFF)LCD_ShowString(30,130,200,16,16,"Press KEY0 to Adjust");//电阻屏才显示
	// delay_ms(1500);
 	// Load_Drow_Dialog();
	
	// if(tp_dev.touchtype&0X80)ctp_test();//电容屏测试
	// else rtp_test(); 					//电阻屏测试











	//TOUCH
	// /* LCD基本代码  */
 	// u8 x=0;
	// u8 lcd_id[12];				//存放LCD ID字符串
	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	// delay_init(168);      //初始化延时函数
	// uart_init(115200);		//初始化串口波特率为115200
	
	// LED_Init();					  //初始化LED
 	// LCD_Init();           //初始化LCD FSMC接口
	// POINT_COLOR=RED;      //画笔颜色：红色
	// sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。				 	
  	// while(1) 
	// {		 
	// 	switch(x)
	// 	{
	// 		case 0:LCD_Clear(WHITE);break;
	// 		case 1:LCD_Clear(BLACK);break;
	// 		case 2:LCD_Clear(BLUE);break;
	// 		case 3:LCD_Clear(RED);break;
	// 		case 4:LCD_Clear(MAGENTA);break;
	// 		case 5:LCD_Clear(GREEN);break;
	// 		case 6:LCD_Clear(CYAN);break; 
	// 		case 7:LCD_Clear(YELLOW);break;
	// 		case 8:LCD_Clear(BRRED);break;
	// 		case 9:LCD_Clear(GRAY);break;
	// 		case 10:LCD_Clear(LGRAY);break;
	// 		case 11:LCD_Clear(BROWN);break;
	// 	}
	// 	POINT_COLOR=RED;	  
	// 	LCD_ShowString(30,40,210,24,24,"Explorer STM32F4");	
	// 	LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
	// 	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
 	// 	LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID	      					 
	// 	LCD_ShowString(30,130,200,12,12,"2014/5/4");			 
	//   	x++;
	// 	if(x==12)x=0;
	// 	LED0=!LED0;	 
	// 	delay_ms(1000);	
	// } 
}



//清空屏幕并在右上角显示"RST"
void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);//清屏   
 	POINT_COLOR=BLUE;//设置字体为蓝色 
	LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域
  	POINT_COLOR=RED;//设置画笔蓝色 
}
////////////////////////////////////////////////////////////////////////////////
//电容触摸屏专有部分
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}
//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  
//两个数之差的绝对值 
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  
//画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
//size：线条的粗细程度
//color：线条的颜色
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		gui_fill_circle(uRow,uCol,size,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   
////////////////////////////////////////////////////////////////////////////////
 //5个触控点的颜色(电容触摸屏用)												 
const u16 POINT_COLOR_TBL[OTT_MAX_TOUCH]={RED,GREEN,BLUE,BROWN,GRED};  
//电阻触摸屏测试函数
void rtp_test(void)
{
	u8 key;
	u8 i=0;	  
	while(1)
	{
	 	key=KEY_Scan(0);
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]>(lcddev.width-24)&&tp_dev.y[0]<16)Load_Drow_Dialog();//清除
				else TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//画图	  			   
			}
		}else delay_ms(10);	//没有按键按下的时候 	    
		if(key==KEY0_PRES)	//KEY0按下,则执行校准程序
		{
			LCD_Clear(WHITE);	//清屏
		    TP_Adjust();  		//屏幕校准 
			TP_Save_Adjdata();	 
			Load_Drow_Dialog();
		}
		i++;
		if(i%20==0)LED=!LED;
	}
}
//电容触摸屏测试函数
void ctp_test(void)
{
	u8 t=0;
	u8 i=0;	  	    
 	u16 lastpos[5][2];		//最后一次的数据 
	while(1)
	{
		tp_dev.scan(0);
		for(t=0;t<5;t++)
		{
			if((tp_dev.sta)&(1<<t))
			{
				if(tp_dev.x[t]<lcddev.width&&tp_dev.y[t]<lcddev.height)
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}
					lcd_draw_bline(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],2,POINT_COLOR_TBL[t]);//画线
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
					if(tp_dev.x[t]>(lcddev.width-24)&&tp_dev.y[t]<20)
					{
						Load_Drow_Dialog();//清除
					}
				}
			}else lastpos[t][0]=0XFFFF;
		}
		
		delay_ms(5);i++;
		if(i%20==0)LED=!LED;
	}	
}







