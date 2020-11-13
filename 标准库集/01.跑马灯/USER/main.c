#include "usart.h"
#include "delay.h"
#include "led.h"

int main(void) {
	
	delay_init(168);
	LED_Init();
	
	while(1) {
		LED0 = 0, LED1 = 1, LED2 = 1;
		delay_ms(500);
		LED1 = 0, LED0 = 1, LED2 = 1;
		delay_ms(500);
		LED2 = 0, LED0 = 1, LED1 = 1;
		delay_ms(500);
	}
}
