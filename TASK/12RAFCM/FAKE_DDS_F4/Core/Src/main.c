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
#include "dac.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "touch.h"
#include "24l01.h"
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
/* é”Ÿï¿½?å¨´å?æ§¸éšï¹?æ¹?Œæ„?°¯é©ç†¸å¯œé”Ÿï¿?? */
u8 TP_CHECK(u16 x0, u16 y0, u16 x1, u16 y1);
void DAC_VAL_Change(void);
void AMP_MUL_Change(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern uint8_t DAC_STA;
extern uint8_t DAC_FRE;
extern uint8_t DAC_VAL;
// extern uint8_t AMP_MUL; 
extern uint32_t TimeOffset;
extern uint16_t Wavetable[];
extern uint16_t Sin[];
extern uint16_t pressTime;
uint8_t presStatus = 0; //ç’æ¿ç¶ç‘™ï¸½æ‡œçžå¿•æ®‘éŽ¸å¤‰ç¬…éŽ?‘­å–Œé”›å²?æ•¤æµœåº¨æ§»å§ãˆ£ç¹›é”Ÿï¿??
uint16_t PRE_DAC_VAL;
uint8_t RX_BUF = 0;
uint8_t RNF24L01_STA = 0; //RNF24L01_STA×´Ì¬ 0-Î´Æô¶¯ 1-·¢ËÍ³É¹¦ 2-·¢ËÍÊ§°Ü
// uint8_t PRE_AMP_MUL;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint8_t LAST_RNF24L01_STA = 0;
  // PRE_AMP_MUL = AMP_MUL;
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
  MX_DAC_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_FSMC_Init();
  MX_TIM3_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  DAC_VAL_Change();
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
  HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 1750);
  delay_init(168);
  LCD_Init();
	tp_dev.init();
	NRF24L01_Init();

	PRE_DAC_VAL = DAC_VAL;
  POINT_COLOR = RED;
  //é„å‰§ãšæ£°æˆ å·¼æ·‡â„ƒä¼?
  //     (28,40)                     (68,40)                                      (192,40)    (202,44)
  //       (32,48) - (44,49)                    DAC_FRE    ()                          (196,48)    +     (208,49)
  //                   (48,56)                                 (172,56)                     (203,52)   (212,56)
  LCD_ShowString(68, 40, 104, 16, 16, (uint8_t *)"DAC_FRE:  KHz");
  LCD_Fill(28,40,48,56,GRAY);
  LCD_Fill(32,48,44,49,GREEN);
  LCD_Fill(192,40,212,56,GRAY);
  LCD_Fill(196,48,208,49,RED);
  LCD_Fill(202,44,203,52,RED);
  // LCD_ShowxNum(132,40,12,2,16,0);

  //é„å‰§ãšæˆæ’³åš­éªžå‘­å®³æ·‡â„ƒä¼…
  LCD_ShowString(64, 90, 112, 16, 16, (uint8_t *)"DAC_VAL:    mV");
  LCD_Fill(28,90,48,106,GRAY);
  LCD_Fill(32,98,44,99,GREEN);
  LCD_Fill(192,90,212,106,GRAY);
  LCD_Fill(196,98,208,99,RED);
  LCD_Fill(202,94,203,102,RED);
  // LCD_ShowxNum(128,90,1122,4,16,0);

  LCD_Fill(40,140,200,170,GREEN);
  LCD_ShowString(72, 147, 168, 163, 16, (uint8_t *)"Start Detect");

	//ÏÔÊ¾24L01×´Ì¬ ²¢ÇÐ»»³É·¢ËÍÄ£Ê½
	LCD_ShowString(20, 220, 150, 16, 16,(uint8_t *)"NRF24L01: ");
  while(!(RNF24L01_STA = !NRF24L01_Check()))
  LCD_ShowString(100,220,60,16,16,(uint8_t *)"Error");
	NRF24L01_TX_Mode();

  // HAL_UART_Receive_IT(&huart1,RxBuf,sizeof(RxBuf));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    //É¨Ãè´¥ÃþÆÁ
    tp_dev.scan(0);
    if (tp_dev.sta & TP_PRES_DOWN)
    {			
      presStatus = 1;
      if (pressTime++ == 0 || (pressTime > 200 && pressTime%20 == 0))
      {
        //éŽ¸å?ç¬…DAC_FREé”Ÿï¿½? '-'
        if(TP_CHECK(28,40,48,56)){
          DAC_FRE = DAC_FRE==1?1:DAC_FRE-1;
        }
        //éŽ¸å?ç¬…DAC_FREé”Ÿï¿½? '+'
        if(TP_CHECK(192,40,212,56)) {
          DAC_FRE = DAC_FRE==40?40:DAC_FRE+1;
        }
        //éŽ¸å?ç¬…DAC_VALé”Ÿï¿½? '-'
        if(TP_CHECK(28,90,48,106)) {
          PRE_DAC_VAL = PRE_DAC_VAL==3?3:PRE_DAC_VAL-1;
        }
        //éŽ¸å?ç¬…DAC_VALé”Ÿï¿½? '+'
        if(TP_CHECK(192,90,212,106)) {
          PRE_DAC_VAL = PRE_DAC_VAL==100?100:PRE_DAC_VAL+1;
        }
        if(TP_CHECK(40,140,200,170)) {
          if(DAC_STA) {
            LCD_Fill(40,140,200,170,GREEN);
            LCD_ShowString(72, 147, 168, 163, 16, (uint8_t *)"Start Detect");
          }else {
            POINT_COLOR = GREEN;
            LCD_Fill(40,140,200,170,RED);
            LCD_ShowString(76, 147, 164, 163, 16, (uint8_t *)"Stop Detect");
            POINT_COLOR = RED;
          }
					DAC_STA = !DAC_STA;
        }
      }
    }
    else {
      presStatus = 0;
			pressTime = 0;
			if(PRE_DAC_VAL != DAC_VAL) {
				DAC_VAL = PRE_DAC_VAL;
				DAC_VAL_Change();
			}
    }


    LCD_ShowxNum(132,40,DAC_FRE,2,16,0);
    LCD_ShowxNum(128,90,PRE_DAC_VAL,4,16,0);
    
    if(RNF24L01_STA != LAST_RNF24L01_STA){
			LCD_Fill(100,220,180,240,WHITE);
			LAST_RNF24L01_STA = RNF24L01_STA;
		}
    if(RNF24L01_STA==TX_OK) {
	    LCD_ShowString(100,220,80,16,16,(uint8_t *)"Sending...");
    }
    else
      LCD_ShowString(100,220,80,16,16,(uint8_t *)"Send Error");
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
u8 TP_CHECK(u16 x0, u16 y0, u16 x1, u16 y1)
{
	return (tp_dev.x[0] > x0 && tp_dev.y[0] > y0 && tp_dev.x[0] < x1 && tp_dev.y[0] < y1);
}
void DAC_VAL_Change(void) {
  uint16_t i;
	uint16_t * Wavetable_p = Wavetable;
  for(i=0;i<5000;i++) {
		*(Wavetable_p++) = ((int32_t)Sin[i]-2028)*DAC_VAL/100+2028;
  }
}
// void AMP_MUL_Change(void) {
//   uint16_t i;
//   for(i=0;i<10;i++) {
//     Wavetable[i] = ((int32_t)Sin[i]-2028)*DAC_VAL*AMP_MUL/1000+2028;
//   }
// }
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
