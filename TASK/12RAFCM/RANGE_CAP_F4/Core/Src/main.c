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
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "base.h"
#include "lcd.h"
#include "touch.h"
#include "24l01.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TP_CHECK(x0,y0,x1,y1) tp_dev.x[0] > x0 && tp_dev.y[0] > y0 && tp_dev.x[0] < x1 && tp_dev.y[0] < y1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t Mode = 0;         //当前模式：0-均不开启 1-开启示波器显示 2-wifi模块
uint8_t NRF24L01_STA = 0; //NRF24L01_状态 0-未成功启用
uint8_t presStatus = 0;   //标记按下
uint8_t DAC_FRE = 0;
uint32_t ADC_CAP = 0; 
extern uint16_t tick_hu;
extern uint8_t tick_dr;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void modeOn(uint8_t mode){
  switch (mode)
  {
    //示波器
    case 1:{
      LCD_Fill(40,30,200,60,RED);
 	    POINT_COLOR=GREEN;
      LCD_ShowString(40+37,30+7,200,60,16,(uint8_t *)"Osc Mode On");
			NRF24L01_RX_Mode();
      //TODO:初始化
    }break;
    //WIFI
    case 2:{
      LCD_Fill(40,80,200,110,RED);
      POINT_COLOR=GREEN;
      LCD_ShowString(40+37,80+7,200,110,16,(uint8_t *)"WiF Mode On");
    }break;
  default:
    break;
  }
};
void modeOff(uint8_t mode){
  switch (mode)
  {
    //示波器
    case 1:{
      LCD_Fill(40,30,200,60,GREEN);
 	    POINT_COLOR=RED;
      LCD_ShowString(40+33,30+7,200,60,16,(uint8_t *)"Osc Mode Off");
      //TODO:初始化
    }break;
    //WIFI
    case 2:{
      LCD_Fill(40,80,200,110,GREEN);
      POINT_COLOR=RED;
      LCD_ShowString(40+33,80+7,200,110,16,(uint8_t *)"WiF Mode Off");
    }break;
  default:
    break;
  }
}
void modeSwitchTo(uint8_t nextMode){
  //如果Mode不为0，则关掉当前模式
  if(Mode)    modeOff(Mode);
  //如果nextMode不为0，则开启当前模式
  if(nextMode)modeOn(nextMode);
  Mode = nextMode;
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
  uint8_t nextMode;     //预备的模式

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
  MX_DMA_Init();
  MX_FSMC_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  delay_init(168);
	LCD_Init();
	tp_dev.init();
 	NRF24L01_Init();
  HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_CAP,1);
  HAL_DAC_Start(&hdac,DAC1_CHANNEL_1);
  HAL_DAC_Start(&hdac,DAC1_CHANNEL_2);
  HAL_TIM_Base_Start_IT(&htim2);

 	POINT_COLOR=RED;

  modeOff(1);
  modeOff(2);

  //查看NRF24L01初始化情况并进入接收状态，若没有成功初始化则进入死循环
  LCD_ShowString(30,130,200,16,16,(uint8_t *)"NRF24L01:");
	while(!(NRF24L01_STA = !NRF24L01_Check()))
	{
		LCD_ShowString(103,130,200,16,16,(uint8_t *)"Error");
		delay_ms(200);
		LCD_Fill(103,130,200,150,WHITE);
 		delay_ms(200);
	}
	LCD_ShowString(103,130,200,16,16,(uint8_t *)"Ready");
  NRF24L01_RX_Mode();
	LCD_Fill(103,130,200,150,WHITE);
	LCD_ShowString(103,130,200,16,16,(uint8_t *)"Receiving..");
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    //扫描并且切换模式
		if(Mode == 1) {
			while(1)
			{
				while(NRF24L01_RxPacket(&DAC_FRE));
				tick_hu = 2750;
				tick_dr = 0;
			}
		}
		
		
    tp_dev.scan(0);
    if(TP_CHECK(40,30,200,60))  nextMode = 1;
    if(TP_CHECK(40,80,200,130)) nextMode = 2;
    if(Mode != nextMode) modeSwitchTo(nextMode);
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
