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
/* �?测是否在某区域按�? */
u8 TP_CHECK(u16 x0, u16 y0, u16 x1, u16 y1);
void DAC_VAL_Change(void);
void AMP_MUL_Change(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern uint8_t DAC_STA;
extern uint8_t DAC_FRE;
extern uint8_t DAC_VAL;
extern uint8_t AMP_MUL; 
extern uint32_t TimeOffset;
extern uint16_t Wavetable[];
extern uint16_t Sin[];
uint8_t PRE_DAC_FRE;
uint8_t PRE_DAC_VAL;
uint8_t PRE_AMP_MUL;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint8_t presStatus = 0; //记录触摸屏的按下情况，用于防止连�?
  PRE_DAC_FRE = DAC_FRE;
  PRE_DAC_VAL = DAC_VAL;
  PRE_AMP_MUL = AMP_MUL;
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
  delay_init(168);
  LCD_Init();
	tp_dev.init();

  POINT_COLOR = RED;
  //显示频率信息
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

  //显示输出幅度信息
  LCD_ShowString(64, 90, 112, 16, 16, (uint8_t *)"DAC_VAL:    mV");
  LCD_Fill(28,90,48,106,GRAY);
  LCD_Fill(32,98,44,99,GREEN);
  LCD_Fill(192,90,212,106,GRAY);
  LCD_Fill(196,98,208,99,RED);
  LCD_Fill(202,94,203,102,RED);
  // LCD_ShowxNum(128,90,1122,4,16,0);

  //显示放大倍数信息
  LCD_ShowString(72, 140, 96, 16, 16, (uint8_t *)"AMP_MUL:X   ");
  LCD_Fill(28,140,48,156,GRAY);
  LCD_Fill(32,148,44,149,GREEN);
  LCD_Fill(192,140,212,156,GRAY);
  LCD_Fill(196,148,208,149,RED);
  LCD_Fill(202,144,203,152,RED);
  // LCD_ShowxNum(148,140,123,3,16,0);

  // HAL_UART_Receive_IT(&huart1,RxBuf,sizeof(RxBuf));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    //按键扫描部分
    tp_dev.scan(0);




    if (tp_dev.sta & TP_PRES_DOWN)
    {
      if (presStatus == 0)
      {
        presStatus = 1;

        //按下DAC_FRE�? '-'
        if(TP_CHECK(28,40,48,56)){
          PRE_DAC_FRE = PRE_DAC_FRE==1?1:PRE_DAC_FRE-1;
        }
        //按下DAC_FRE�? '+'
        if(TP_CHECK(192,40,212,56)) {
          PRE_DAC_FRE = PRE_DAC_FRE==40?40:PRE_DAC_FRE+1;
        }
        //按下DAC_VAL�? '-'
        if(TP_CHECK(28,90,48,106)) {
          PRE_DAC_VAL = PRE_DAC_VAL==3?3:PRE_DAC_VAL-1;
        }
        //按下DAC_VAL�? '+'
        if(TP_CHECK(192,90,212,106)) {
          PRE_DAC_VAL = PRE_DAC_VAL==100?100:PRE_DAC_VAL+1;
        }
        //按下AMP_MUL�? '-'
        if(TP_CHECK(28,140,48,156)) {
          PRE_AMP_MUL = PRE_AMP_MUL==1?1:PRE_AMP_MUL-1;
        }
        //按下AMP_MUL�? '+'
        if(TP_CHECK(192,140,212,156)) {
          PRE_AMP_MUL = PRE_AMP_MUL==100?100:PRE_AMP_MUL+1;
        }
      }
    }
    else {
      presStatus = 0;
    }

    //�?查更新部�?
    if(DAC_FRE != PRE_DAC_FRE)
    {
      DAC_FRE = PRE_DAC_FRE;
    }
    if(DAC_VAL != PRE_DAC_VAL)
    {
      DAC_VAL = PRE_DAC_VAL;
      DAC_VAL_Change();
    }
    if(AMP_MUL != PRE_AMP_MUL)
    {
      AMP_MUL = PRE_AMP_MUL;
      AMP_MUL_Change();
    }

    //日常刷新
    LCD_ShowxNum(132,40,DAC_FRE,2,16,0);
    LCD_ShowxNum(128,90,DAC_VAL,4,16,0);
    LCD_ShowxNum(148,140,AMP_MUL,3,16,0);
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
		*(Wavetable_p++) = ((int32_t)Sin[i]-2028)*DAC_VAL*AMP_MUL/1000+2028;
  }
}
void AMP_MUL_Change(void) {
  uint16_t i;
  for(i=0;i<10;i++) {
    Wavetable[i] = ((int32_t)Sin[i]-2028)*DAC_VAL*AMP_MUL/1000+2028;
  }
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
