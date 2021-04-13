/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "base.h"
#include "lcd.h"
#include "touch.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);//濞撳懎鐫�?   
 	POINT_COLOR=BLUE;//鐠佸墽鐤嗙€涙ぞ缍嬫稉楦挎憫閿燂�?? 
	LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//閺勫墽銇氬�?�鍛潌閸栧搫鐓�?
  	POINT_COLOR=RED;//鐠佸墽鐤嗛悽鑽ょ應閽冩繆澹� 
}
////////////////////////////////////////////////////////////////////////////////
//閻㈤潧顔愮憴锔芥嚋鐏炲繋绗撻張澶愬劥閿燂�??
//閻㈢粯鎸夐獮宕囧�?
//x0,y0:閸ф劖鐖�
//len:缁惧潡鏆遍敓锟�?
//color:妫版粏澹�?
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}
//閻㈣鐤勮箛鍐ㄦ�?
//x0,y0:閸ф劖鐖�
//r:閸楀﹤绶�?
//color:妫版粏澹�?
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
//娑撱倓閲滈弫棰佺�?�割喚娈戠紒婵嗩嚠閿燂拷? 
//x1,x2閿涙岸娓堕崣鏍ф▕閸婅偐娈戞稉銈勯嚋閿燂拷?
//鏉╂柨娲栭崐纭风窗|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  
//閻㈣绔撮弶锛勭煐閿燂拷?
//(x1,y1),(x2,y2):缁炬寧娼惃鍕崳婵娼楅敓锟�??
//size閿涙氨鍤庨弶锛勬畱缁绮忕粙�?��?
//color閿涙氨鍤庨弶锛勬畱妫版粏澹�
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //鐠侊紕鐣婚崸鎰垼婢х偤鍣�? 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //鐠佸墽鐤嗛崡鏇燁劄閺傜懓鎮� 
	else if(delta_x==0)incx=0;//閸ㄥ倻娲块敓锟�? 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//濮樻潙閽╅敓锟�? 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //闁褰囬崺鐑樻拱婢х偤鍣洪崸鎰垼閿燂拷? 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//閻㈣崵鍤庢潏鎾冲�? 
	{  
		gui_fill_circle(uRow,uCol,size,color);//閻㈣崵鍋�? 
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
 //5娑擃亣袝閹貉呭仯閻ㄥ嫰顤�?敓锟�??(閻㈤潧顔愮憴锔芥嚋鐏炲繒鏁�)												 
const u16 POINT_COLOR_TBL[OTT_MAX_TOUCH]={RED,GREEN,BLUE,BROWN,GRED};  
//閻㈢敻妯嗙憴锔芥嚋鐏炲繑绁寸拠鏇炲毐閿燂�??
void rtp_test(void)
{
	u8 key;
	u8 i=0;	  
	while(1)
	{
	 	key=KEY_Scan(0);
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//鐟欙附鎳滅仦蹇氼潶閹稿绗�
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]>(lcddev.width-24)&&tp_dev.y[0]<16)Load_Drow_Dialog();//濞撳懘娅�?
				else TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//閻㈣娴�?	  			   
			}
		}else delay_ms(10);	//濞屸剝婀�?幐澶愭暛閹稿绗呴惃鍕閿燂拷? 	    
		if(key==KEY0_PRES)	//KEY0閹稿绗�?,閸掓瑦澧界悰灞剧墡閸戝棛鈻奸敓锟�??
		{
			LCD_Clear(WHITE);	//濞撳懎鐫�?
		    TP_Adjust();  		//鐏炲繐绠烽弽鈥冲�? 
			TP_Save_Adjdata();	 
			Load_Drow_Dialog();
		}
		i++;
		if(i%20==0)LED0_T;
	}
}
//閻㈤潧顔愮憴锔芥嚋鐏炲繑绁寸拠鏇炲毐閿燂�??
void ctp_test(void)
{
	u8 t=0;
	u8 i=0;	  	    
 	u16 lastpos[5][2];		//閿燂�??閸氬簼绔村▎锛勬畱閺佺増宓� 
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
					lcd_draw_bline(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],2,POINT_COLOR_TBL[t]);//閻㈣崵鍤�?
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
					if(tp_dev.x[t]>(lcddev.width-24)&&tp_dev.y[t]<20)
					{
						Load_Drow_Dialog();//濞撳懘娅�?
					}
				}
			}else lastpos[t][0]=0XFFFF;
		}
		
		delay_ms(5);i++;
		if(i%20==0)LED0_T;
	}	
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  delay_init(168);
	LCD_Init();
	tp_dev.init();

 	POINT_COLOR=RED;//
	LCD_ShowString(30,40,210,24,24,(uint8_t *)"MINI STM32F4");	
	LCD_ShowString(30,70,200,16,16,(uint8_t *)"TFTLCD TEST");
	LCD_ShowString(30,90,200,16,16,(uint8_t *)"ATOM@ALIENTEK");
 	LCD_ShowString(30,110,200,16,16,(uint8_t *)"The First Test");     					 
	LCD_ShowString(30,130,200,12,12,(uint8_t *)"2021/4/8");

  if(tp_dev.touchtype!=0XFF)LCD_ShowString(30,130,200,16,16,"Press KEY0 to Adjust");//閻㈢敻妯嗙仦蹇斿閺勫墽銇�
	delay_ms(1500);
 	Load_Drow_Dialog();	 	
	
	if(tp_dev.touchtype&0X80)ctp_test();//閻㈤潧顔愮仦蹇旂ゴ鐠囷拷
	else rtp_test(); 					//閻㈢敻妯嗙仦蹇旂ゴ鐠囷拷
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
