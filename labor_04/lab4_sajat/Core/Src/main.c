/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "OLED.h"
#include <stdio.h>

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

I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

/* Aktualis szint 0 es 100 kozott */
uint8_t level = 50;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);

static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_I2C2_Init(void);

/* USER CODE BEGIN PFP */

void OLED_ShowLevel(uint8_t value);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/**
  * @brief Szint kijelzese az OLED kijelzon
  */
void OLED_ShowLevel(uint8_t value)
{
    char text[10];
    uint16_t bar_width;

    /* Kijelzo torles */
    OLED_Fill(BLACK);

    /* Cim */
    OLED_SetCursor(36, 2);
    OLED_WriteString("SZINT", Font_11x18, WHITE);

    /* Szazalek szovegge alakitasa */
    sprintf(text, "%d %%", (int)value);

    /* Szazalek kiirasa */
    OLED_SetCursor(40, 25);
    OLED_WriteString(text, Font_11x18, WHITE);

    /* Szintjelzo kerete */
    OLED_DrawRectangle(10, 50, 117, 62, WHITE);

    /* 0-100% -> 0-104 pixel */
    bar_width = ((uint16_t)value * 104) / 100;

    /* Sav kitoltese */
    for (uint16_t x = 12; x < (12 + bar_width); x++)
    {
        OLED_DrawLine(
            (uint8_t)x,
            52,
            (uint8_t)x,
            60,
            WHITE
        );
    }

    /* Kijelzo frissitese */
    OLED_UpdateScreen();
}


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

  HAL_Init();


  /* USER CODE BEGIN Init */

  /* USER CODE END Init */


  /* Configure the system clock */
  SystemClock_Config();


  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */


  /* Initialize all configured peripherals */

  MX_GPIO_Init();

  MX_USART3_UART_Init();

  MX_USB_OTG_FS_PCD_Init();

  MX_I2C2_Init();


  /* USER CODE BEGIN 2 */


  /* OLED inicializalasa */
  OLED_Init();

  HAL_Delay(500);

  /* Kezdo ertek megjelenitese */
  OLED_ShowLevel(level);


  /* USER CODE END 2 */


  /* Infinite loop */

  /* USER CODE BEGIN WHILE */

  while (1)
  {

      /*
       * ============================================
       * PE2 - ELSO GOMB
       * SZINT CSOKKENTESE 10%-KAL
       * ============================================
       */

      if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == GPIO_PIN_SET)
      {
          /* Pergesmentesites */
          HAL_Delay(30);

          if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == GPIO_PIN_SET)
          {
              if (level >= 10)
              {
                  level -= 10;
              }
              else
              {
                  level = 0;
              }

              OLED_ShowLevel(level);

              /*
               * Megvarjuk a gomb felengedeset,
               * igy egy nyomas csak egyszer szamit.
               */
              while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == GPIO_PIN_SET)
              {
              }

              HAL_Delay(30);
          }
      }



      /*
       * ============================================
       * PE3 - MASODIK GOMB
       * SZINT NOVELESE 10%-KAL
       * ============================================
       */

      if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_SET)
      {
          HAL_Delay(30);

          if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_SET)
          {
              if (level <= 90)
              {
                  level += 10;
              }
              else
              {
                  level = 100;
              }

              OLED_ShowLevel(level);

              while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_SET)
              {
              }

              HAL_Delay(30);
          }
      }



      /*
       * ============================================
       * PE6 - HARMADIK GOMB
       * RESET 50%-RA
       * ============================================
       */

      if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6) == GPIO_PIN_SET)
      {
          HAL_Delay(30);

          if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6) == GPIO_PIN_SET)
          {
              level = 50;

              OLED_ShowLevel(level);

              while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6) == GPIO_PIN_SET)
              {
              }

              HAL_Delay(30);
          }
      }


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

  __HAL_PWR_VOLTAGESCALING_CONFIG(
      PWR_REGULATOR_VOLTAGE_SCALE1
  );


  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType =
      RCC_OSCILLATORTYPE_HSE;

  RCC_OscInitStruct.HSEState =
      RCC_HSE_BYPASS;

  RCC_OscInitStruct.PLL.PLLState =
      RCC_PLL_ON;

  RCC_OscInitStruct.PLL.PLLSource =
      RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 4;

  RCC_OscInitStruct.PLL.PLLN = 168;

  RCC_OscInitStruct.PLL.PLLP =
      RCC_PLLP_DIV2;

  RCC_OscInitStruct.PLL.PLLQ = 7;

  RCC_OscInitStruct.PLL.PLLR = 2;


  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
      Error_Handler();
  }


  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType =
      RCC_CLOCKTYPE_HCLK |
      RCC_CLOCKTYPE_SYSCLK |
      RCC_CLOCKTYPE_PCLK1 |
      RCC_CLOCKTYPE_PCLK2;


  RCC_ClkInitStruct.SYSCLKSource =
      RCC_SYSCLKSOURCE_PLLCLK;


  RCC_ClkInitStruct.AHBCLKDivider =
      RCC_SYSCLK_DIV1;


  RCC_ClkInitStruct.APB1CLKDivider =
      RCC_HCLK_DIV4;


  RCC_ClkInitStruct.APB2CLKDivider =
      RCC_HCLK_DIV2;


  if (HAL_RCC_ClockConfig(
          &RCC_ClkInitStruct,
          FLASH_LATENCY_5) != HAL_OK)
  {
      Error_Handler();
  }
}


/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */


  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */


  hi2c2.Instance = I2C2;

  hi2c2.Init.ClockSpeed = 100000;

  hi2c2.Init.DutyCycle =
      I2C_DUTYCYCLE_2;

  hi2c2.Init.OwnAddress1 = 0;

  hi2c2.Init.AddressingMode =
      I2C_ADDRESSINGMODE_7BIT;

  hi2c2.Init.DualAddressMode =
      I2C_DUALADDRESS_DISABLE;

  hi2c2.Init.OwnAddress2 = 0;

  hi2c2.Init.GeneralCallMode =
      I2C_GENERALCALL_DISABLE;

  hi2c2.Init.NoStretchMode =
      I2C_NOSTRETCH_DISABLE;


  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
      Error_Handler();
  }


  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */
}


/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */


  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */


  huart3.Instance = USART3;

  huart3.Init.BaudRate = 115200;

  huart3.Init.WordLength =
      UART_WORDLENGTH_8B;

  huart3.Init.StopBits =
      UART_STOPBITS_1;

  huart3.Init.Parity =
      UART_PARITY_NONE;

  huart3.Init.Mode =
      UART_MODE_TX_RX;

  huart3.Init.HwFlowCtl =
      UART_HWCONTROL_NONE;

  huart3.Init.OverSampling =
      UART_OVERSAMPLING_16;


  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
      Error_Handler();
  }


  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */
}


/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */


  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */


  hpcd_USB_OTG_FS.Instance =
      USB_OTG_FS;

  hpcd_USB_OTG_FS.Init.dev_endpoints =
      6;

  hpcd_USB_OTG_FS.Init.speed =
      PCD_SPEED_FULL;

  hpcd_USB_OTG_FS.Init.dma_enable =
      DISABLE;

  hpcd_USB_OTG_FS.Init.phy_itface =
      PCD_PHY_EMBEDDED;

  hpcd_USB_OTG_FS.Init.Sof_enable =
      ENABLE;

  hpcd_USB_OTG_FS.Init.low_power_enable =
      DISABLE;

  hpcd_USB_OTG_FS.Init.lpm_enable =
      DISABLE;

  hpcd_USB_OTG_FS.Init.vbus_sensing_enable =
      ENABLE;

  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 =
      DISABLE;


  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
      Error_Handler();
  }


  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */
}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};


  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */


  /* GPIO Ports Clock Enable */

  __HAL_RCC_GPIOE_CLK_ENABLE();

  __HAL_RCC_GPIOC_CLK_ENABLE();

  __HAL_RCC_GPIOF_CLK_ENABLE();

  __HAL_RCC_GPIOH_CLK_ENABLE();

  __HAL_RCC_GPIOB_CLK_ENABLE();

  __HAL_RCC_GPIOD_CLK_ENABLE();

  __HAL_RCC_GPIOG_CLK_ENABLE();

  __HAL_RCC_GPIOA_CLK_ENABLE();


  /*Configure GPIO pin Output Level */

  HAL_GPIO_WritePin(
      GPIOB,
      LD1_Pin | LD3_Pin | LD2_Pin,
      GPIO_PIN_RESET
  );


  /*Configure GPIO pin Output Level */

  HAL_GPIO_WritePin(
      USB_PowerSwitchOn_GPIO_Port,
      USB_PowerSwitchOn_Pin,
      GPIO_PIN_RESET
  );


  /*
   * PE2, PE3, PE6
   *
   * Jelenlegi CubeMX beallitas:
   *
   * External Interrupt Rising
   * Pull-down
   *
   * HAL_GPIO_ReadPin()-nel ettol meg
   * beolvashatoak a pinek.
   */

  GPIO_InitStruct.Pin =
      GPIO_PIN_2 |
      GPIO_PIN_3 |
      GPIO_PIN_6;

  GPIO_InitStruct.Mode =
      GPIO_MODE_IT_RISING;

  GPIO_InitStruct.Pull =
      GPIO_PULLDOWN;

  HAL_GPIO_Init(
      GPIOE,
      &GPIO_InitStruct
  );


  /* USER BUTTON */

  GPIO_InitStruct.Pin =
      USER_Btn_Pin;

  GPIO_InitStruct.Mode =
      GPIO_MODE_IT_RISING;

  GPIO_InitStruct.Pull =
      GPIO_NOPULL;

  HAL_GPIO_Init(
      USER_Btn_GPIO_Port,
      &GPIO_InitStruct
  );


  /* LEDs */

  GPIO_InitStruct.Pin =
      LD1_Pin |
      LD3_Pin |
      LD2_Pin;

  GPIO_InitStruct.Mode =
      GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Pull =
      GPIO_NOPULL;

  GPIO_InitStruct.Speed =
      GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(
      GPIOB,
      &GPIO_InitStruct
  );


  /* USB Power Switch */

  GPIO_InitStruct.Pin =
      USB_PowerSwitchOn_Pin;

  GPIO_InitStruct.Mode =
      GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Pull =
      GPIO_NOPULL;

  GPIO_InitStruct.Speed =
      GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(
      USB_PowerSwitchOn_GPIO_Port,
      &GPIO_InitStruct
  );


  /* USB OverCurrent */

  GPIO_InitStruct.Pin =
      USB_OverCurrent_Pin;

  GPIO_InitStruct.Mode =
      GPIO_MODE_INPUT;

  GPIO_InitStruct.Pull =
      GPIO_NOPULL;

  HAL_GPIO_Init(
      USB_OverCurrent_GPIO_Port,
      &GPIO_InitStruct
  );


  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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

  __disable_irq();

  while (1)
  {
  }

  /* USER CODE END Error_Handler_Debug */
}


#ifdef USE_FULL_ASSERT

/**
  * @brief Reports the name of the source file and the source line number
  */
void assert_failed(uint8_t *file, uint32_t line)
{

  /* USER CODE BEGIN 6 */

  /* User can add his own implementation */

  /* USER CODE END 6 */
}

#endif /* USE_FULL_ASSERT */
