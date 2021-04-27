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
#include "tim.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
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
u8 TP_CHECK(u16 x0, u16 y0, u16 x1, u16 y1);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t AMP_MUL = 1;
int32_t ADC_CAP = 0; 
uint16_t pressTime = 0;
uint8_t presStatus = 0; //标记按键是否按下，防止连
uint16_t offset[100] = {

2048,
2077,
2107,
2136,
2166,
2195,
2225,
2254,
2284,
2313,
2343,
2372,
2402,
2431,
2461,
2490,
2520,
2549,
2579,
2608,
2638,
2667,
2697,
2726,
2756,
2786,
2815,
2845,
2874,
2904,
2933,
2963,
2992,
3022,
3051,
3081,
3110,
3140,
3169,
3199,
3228,
3258,
3287,
3317,
3346,
3376,
3405,
3435,
3464,
3494,
3524,
3553,
3583,
3612,
3642,
3671,
3701,
3730,
3760,
3789,
3819,
3848,
3878,
3907,
3937,
3966,
3996,
4025,
4055,
4084,
4114,
4143,
4173,
4202,
4232,
4262,
4291,
4321,
4350,
4380,
4409,
4439,
4468,
4498,
4527,
4557,
4586,
4616,
4645,
4675,
4704,
4734,
4763,
4793,
4822,
4852,
4881,
4911,
4940,
4970
};
// uint8_t PRE_AMP_MUL;
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
  MX_DMA_Init();
  MX_DAC_Init();
  MX_FSMC_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_CAP,1);
  HAL_DAC_Start(&hdac,DAC1_CHANNEL_2);
  delay_init(168);
  LCD_Init();
	tp_dev.init();

  POINT_COLOR = RED;
  //     (28,40)                     (68,40)                                      (192,40)    (202,44)
  //       (32,48) - (44,49)                    DAC_FRE    ()                          (196,48)    +     (208,49)
  //                   (48,56)                                 (172,56)                     (203,52)   (212,56)

  LCD_ShowString(72, 40, 96, 16, 16, (uint8_t *)"AMP_MUL:X   ");
  LCD_Fill(28,40,48,56,GRAY);
  LCD_Fill(32,48,44,49,GREEN);
  LCD_Fill(192,40,212,56,GRAY);
  LCD_Fill(196,48,208,49,RED);
  LCD_Fill(202,44,203,52,RED);
  // LCD_ShowxNum(148,140,123,3,16,0);
  LCD_ShowxNum(148,40,AMP_MUL,3,16,0);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		
    /* USER CODE BEGIN 3 */

    //ćéŽćŤćé¨ĺ
    tp_dev.scan(0);
		//DAC->DHR12R1 = ((int32_t) ADC_CAP - 2028)* AMP_MUL / 10 + 2028;
		DAC->DHR12R2 = ((ADC_CAP - 1912)* AMP_MUL / 10) + 2048;
		//DAC->DHR12R2 = ((ADC_CAP - 1920)* AMP_MUL / 10) + 2048;
		// DAC->DHR12R2 = ADC_CAP;
		
    if (tp_dev.sta & TP_PRES_DOWN)
    {
      if (pressTime == 0 || ((pressTime > 1000) && (pressTime%50 == 0)))
      {
				pressTime++;
        presStatus = 1; 

        if(TP_CHECK(28,40,48,56)) {
          AMP_MUL = AMP_MUL==1?1:AMP_MUL-1;
          LCD_ShowxNum(148,40,AMP_MUL,3,16,0);
        }
        if(TP_CHECK(192,40,212,56)) {
          AMP_MUL = AMP_MUL==300?300:AMP_MUL+1;
          LCD_ShowxNum(148,40,AMP_MUL,3,16,0);
        }
      }
    }
    else {
      presStatus = 0;
      pressTime = 0;
    }

    //日常刷新
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
