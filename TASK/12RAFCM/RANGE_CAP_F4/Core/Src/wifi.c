#include "wifi.h"
#include "string.h"
#include "stdio.h"
#include "lcd.h"

const u8* wifista_ssid      =(u8 *)"dxxy16-403-1";			//路由器SSID号
const u8* wifista_password  =(u8 *)"1234567890"; 	
const u8* link_ip = (u8 *)"233";
const u8* portnum = (u8 *)"233";
//向ATK-ESP8266发送命令
//cmd:发送的命令字符串
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 atk_8266_send_cmd(char *cmd,char *ack,u16 waittime)
{
	u8 temp[WIFI_MAX_SEND_LEN];
	u8 res=0;
	WIFI_RX_STA=0;
	
	memset(temp,0,WIFI_MAX_SEND_LEN);
	sprintf((char *)temp,"%s\r\n",cmd);
	memset(WIFI_RX_BUF,0,WIFI_MAX_RECV_LEN);
	u1_printf(temp);
	HAL_UART_Receive(&huart1,WIFI_RX_BUF,WIFI_MAX_RECV_LEN,waittime);
	if(atk_8266_check_cmd(ack)) res=0;
	else res = 1;
	
	return res;
}

//ATK-ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8* atk_8266_check_cmd(char *str)
{
	char *strx=0;
	
	strx=strstr((const char*)WIFI_RX_BUF,(const char*)str);
	
	return (u8*)strx;
}

void atk_8266_Init(void) 
{
	char p[400];
	
	while( atk_8266_send_cmd("AT","OK",20) )
		LCD_ShowString(30,150,200,16,16,(uint8_t *)"ATK-8266: ERROR");
	LCD_Fill(30,150,210,170,WHITE);
	
	if(!atk_8266_send_cmd("AT+CWMODE=1","OK",200))
		LCD_ShowString(30,150,200,16,16,(uint8_t *)"ATK-8266: STA MODE ON");
	LCD_Fill(30,150,210,170,WHITE);
	
	atk_8266_send_cmd("AT+RST","NULL",100);
		LCD_ShowString(30,150,200,16,16,(uint8_t *)"ATK-8266: RESTARTING");
	delay_ms(800);
	LCD_Fill(30,150,210,170,WHITE);
	if(!atk_8266_send_cmd("AT","OK",100))
		LCD_ShowString(30,150,200,16,16,(uint8_t *)"ATK-8266: RST SUC");
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
	
	LCD_Fill(30,150,210,170,WHITE);
	LCD_ShowString(30,150,200,16,16,(uint8_t *)"ATK-8266: WIFI LINKING");
	if(!atk_8266_send_cmd(p,"WIFI GOT IP",10000))
	{
		LCD_Fill(30,150,210,170,WHITE);
		LCD_ShowString(30,150,200,16,16,(uint8_t *)"ATK-8266: WIFI LINKED");
	}
	LCD_ShowString(30,150,200,16,16,(uint8_t *)"ATK-8266: WIFI OK");
}

