#include "led.h"
#include "lcd.h"
#include "usart.h"
#include "delay.h"
#include "key.h"
#include "touch.h"
#include "eval.h"

/******************************************************************************************************************************************
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
*******************************************************************************************************************************************/
void show_page(u8 page); //显示某页面静态信息
void LCD_Show_Image(u16 x,u16 y,u16 width,u16 height,u8 * image); //显示图像
void sans(void); //跑

u8 current_page = 0; //当且页面
u8 image_sans[] = {
0x00,0x00,0x1C,0x00,0x00,0x00,0x62,0x00,0x00,0x00,0x81,0x06,0x00,0x01,0x10,0xB5,0x07,0xBA,0x08,0xFD,0x18,0x67,0x07,0x89,0x20,0x06,0x80,0xF9,0x23,0xB2,0x40,0xF9,0x43,0x9B,0xFF,0xF9,
0x43,0x95,0x45,0xFA,0x40,0x1D,0x41,0xFE,0x40,0x55,0x41,0xF0,0x40,0xDD,0xA1,0xE0,0x40,0x55,0x41,0xF0,0x40,0x1D,0x41,0xFE,0x43,0x95,0x45,0xFA,0x43,0x9B,0xFF,0xF9,0x23,0xB2,0x40,0xF9,
0x20,0x06,0x80,0xF9,0x18,0x67,0x07,0x89,0x07,0xBA,0x08,0xFD,0x00,0x01,0x10,0xB5,0x00,0x00,0x81,0x06,0x00,0x00,0x62,0x00,0x00,0x00,0x1C,0x00};

int main(void)
{ 
	u8 *str = (u8 *)malloc(USART_REC_LEN * sizeof(u8));	//用于存储单次串口接收到的信息
	double num = 50;	//LCD上显示的数字
	u8 key;         //按键值
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置系统中断优先级分组2
	delay_init(168);								//延时初始化
	uart_init(115200);								//串口初始化波特率为115200
	LED_Init();										//初始化与LED连接的硬件接口
	LCD_Init();										//初始化LCD FSMC接口
	TP_Init();										//初始化触摸屏

	/* 在LCD上显示信息 */
	show_page(0);

	/* 向串口输出信息 */
	printf("学号:201921111066\r\n");
	printf("姓名:Huang Yi\r\n");
	printf("P I :3.1415926\r\n");


	while(1) {
		/* 检测串口是否接收完成 */
		if(USART_RX_STA&0x8000) {
			u16 i,j,len;				//i为循环变量,len标记接收到的长度
			u8 flag = 0;
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度

			/* 将接收到的值计算，把计算式及结果赋值给str */
			sprintf((char *)str,"%s = %.0f\n", USART_RX_BUF, eval((char *)USART_RX_BUF));
			num = eval((char *)USART_RX_BUF);

			
			if(USART_RX_BUF[0] == 'p' || USART_RX_BUF[1] == '0') show_page(current_page = 0);
			if(USART_RX_BUF[0] == 'p' || USART_RX_BUF[1] == '1') show_page(current_page = 1);
			/* 清空接收到的信息 */
			for(i=0;i<USART_REC_LEN;i++) USART_RX_BUF[i] = 0;

			if(current_page == 0) {
				for(j=0;j<USART_REC_LEN;j++) {
					if(str[j] == '+' || str[j] == '-' || str[j] == '*' || str[j] == '/') {
						flag = 1;
						break;
					}
				}
				if 	(!flag) {
					LCD_Fill( 30, 150, 210, 180, WHITE);	//先清空该区域
					sprintf((char *)str,"Num: %.2f", num+1);
					LCD_ShowString( 30, 150, 200, 16, 16, str);  //显示+1或者-1
					sprintf((char *)str,"%.2f", num-1);
					LCD_ShowString( 130, 150, 200, 16, 16, str);  //显示+1或者-1
					printf("Num: %.2f  %.2f\r\n",num+1,num-1);
				}
				else { //表达式
					/* 向LCD和串口打印计算式及结果 */
					printf("%s", str);
					LCD_Fill( 30, 180, 210, 210, WHITE);	//先清空该区域
					LCD_ShowString( 30,  180,   200,  16,  16, str);
				}
			}
			
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);	//等待发送结束
			USART_RX_STA=0;										 	//接收状态清空 
		}

		/* 扫描触摸屏是否被按下 */
		tp_dev.scan(0);
//		if(current_page == 0) {
			if(tp_dev.sta&TP_PRES_DOWN) {
				//判断按下位置是否正常
			 	if(tp_dev.x[0]<lcddev.width  &&  tp_dev.y[0]<lcddev.height) {
					//分别按下 Turn on  和  Turn off
					if(tp_dev.x[0]> 30 && tp_dev.y[0]> 100 && tp_dev.x[0]<100 && tp_dev.y[0]<140) LED = 0;
					if(tp_dev.x[0]>140 && tp_dev.y[0]> 100 && tp_dev.x[0]<210 && tp_dev.y[0]<140) LED = 1;
					if(tp_dev.x[0]>220 && tp_dev.y[0]>   0 && tp_dev.x[0]<240 && tp_dev.y[0]< 20) show_page(current_page = 0);
					if(tp_dev.x[0]>220 && tp_dev.y[0]>  20 && tp_dev.x[0]<240 && tp_dev.y[0]< 40) show_page(current_page = 1);
				}
			}else delay_ms(10);
//		}

		/* 页面2动画效果 */
		if(current_page == 1) {
			sans();
		}

		/* 扫描按键确定是否换页 */
		key = KEY_Scan(0);
		delay_ms(10);
		if(key == KEY0_PRES) show_page(current_page = 0);
		if(key == KEY1_PRES) show_page(current_page = 1);
	}
}



void show_page(u8 page) {
	LCD_Clear(WHITE);
	switch (page)
	{
	case 0:
		LCD_DrawRectangle(  20,  20,  lcddev.width-20, lcddev.height-20);
		LCD_ShowString(  30,  30,  200,  16,  16,"Num :201921111066");
		LCD_Show_CH_Font16(30, 30, 0); //学
		LCD_Show_CH_Font16(46, 30, 1); //号
		LCD_ShowString(  30,  50,  200,  16,  16,"Name:Huang Yi");	
		LCD_Show_CH_Font16(30, 50, 2); //学
		LCD_Show_CH_Font16(46, 50, 3); //号
		LCD_ShowString(  30,  70,  200,  16,  16," PI :3.1415926");
		LCD_Fill( 30, 100, 100, 140, GREEN);
		LCD_Fill(140, 100, 210, 140, GRAY);
		LCD_ShowString2(  35, 115,   70,  16,  16,"Turn On");
		LCD_ShowString2( 145, 115,   70,  16,  16,"Turn Off");
		LCD_ShowString (  30, 150,   70,  16,  16,"Num: 50");
		LCD_Fill( lcddev.width-20, 0, lcddev.width, 20, GREEN);
		LCD_ShowString2 (  lcddev.width-15,  2,  20,  16,  16,"1");
		LCD_Fill( lcddev.width-20,20, lcddev.width, 40, YELLOW);
		LCD_ShowString2 (  lcddev.width-15, 22,  20,  16,  16,"2");
		break;
	case 1:
		LCD_Show_Image(100, 50, 25, 32, image_sans);
		LCD_Fill( 220, 0, 240, 20, GREEN);
		LCD_ShowString2 (  lcddev.width-15, 2,  20,  16,  16,"2");
		break;
	}
}

void LCD_Show_Image(u16 x,u16 y,u16 width,u16 height,u8 * image) {
    u8 temp,t,t1;
    u16 y0=y;

	for(t=0;t<width*height/8;t++) {
       	temp=image[t];
       	for(t1=0;t1<8;t1++){
		   if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
		   else LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
		  	y++;

		if((y-y0)==height)
         {
          y=y0;
          x++;
          break;
         }
	   	}
	}
}


void sans(void) {
	/* x,y当前坐标,x-way,y-way方向 */
	static u16 x = 100, y = 20;
	static int x_way = 1, y_way = 1;
	static u8 width = 25, height = 32;
	
	LCD_Clear(WHITE);
	x += x_way;
	y += y_way;
	LCD_Show_Image(x, y, 25, 32, image_sans);
	
	
	//LCD_DrawRectangle(  45, 120,  75, 180);
	//LCD_DrawRectangle( 165, 120, 195, 180);
	
	//改变跑马方向
	if(x_way == -1 && x == 0                     ) x_way = -x_way;
	if(x_way ==  1 && x == lcddev.width - width  ) x_way = -x_way;
	if(y_way == -1 && y == 0                     ) y_way = -y_way;
	if(y_way ==  1 && y == lcddev.height - height) y_way = -y_way;
	
	
	LCD_Fill( lcddev.width-20, 0, lcddev.width, 20, GREEN);
	LCD_ShowString2 (  lcddev.width-15,  2,  20,  16,  16,"1");
	LCD_Fill( lcddev.width-20, 20, lcddev.width, 40, YELLOW);
	LCD_ShowString2 (  lcddev.width-15, 22,  20,  16,  16,"2");

}



