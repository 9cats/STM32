#ifndef __WIFI_H__
#define __WIFI_H__	
#include "base.h"
#include "usart.h"


 
extern const u8* wifista_ssid;		//WIFI STA SSID
extern const u8* wifista_password; 	//WIFI STA 密码
extern const u8* link_ip;           //连接IP
extern const u8* portnum;			//连接端口			//连接密码

u8 atk_8266_send_cmd(char *cmd,char *ack,u16 waittime); 
u8* atk_8266_check_cmd(char *str);		
void	atk_8266_Init(void);
#endif
