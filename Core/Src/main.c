/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "time.h"
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// TODO change to actual values
#define CLK_FREQ_HZ (8000000)
#define TIM2_CLK_DEV (1)
#define TIM2_CLK_PRESCALER (8000)
#define TIM2_CLK_KHZ (CLK_FREQ_HZ / TIM2_CLK_DEV / TIM2_CLK_PRESCALER / 1000) // 1 / ms

#define BRIGHTNESS_STEPS (50)
#define MAX_BRIGHTNESS   (BRIGHTNESS_STEPS - 1)
#define MIN_BRIGHTNESS   (0)
#define HALF_BRIGHTNESS  ((uint8_t)(BRIGHTNESS_STEPS + 0.5f) / 2)

#define HOLD_BRIGHTNESS_JUMP (3)

#define TOGGLE_DELAY_MS     (250)
#define LOWER_SWEEP_TIME_MS (3375)                                          // time from 50%->0%
#define UPPER_SWEEP_TIME_MS (4000)                                          // time from 50%->100%
#define TOTAL_SWEEP_TIME_MS (7375)                                          // time from 0%->100%
#define AVG_SWEEP_TIME_MS   (TOTAL_SWEEP_TIME_MS / 2)                       // 3687.5
#define LOWER_STEP_TIME_MS  (LOWER_SWEEP_TIME_MS / (BRIGHTNESS_STEPS / 2))  // 135, time lower per step
#define UPPER_STEP_TIME_MS  (UPPER_SWEEP_TIME_MS / (BRIGHTNESS_STEPS / 2))  // 160, time upper per step
#define AVG_STEP_TIME_MS    ((UPPER_STEP_TIME_MS + LOWER_STEP_TIME_MS) / 2) // 147.5
#define AVG_STEP_DIFF_MS    (AVG_STEP_TIME_MS    - LOWER_STEP_TIME_MS)      // 12.5, distance between lower step time and average step time
#define LOW_STEP_TIME_MS    (LOWER_STEP_TIME_MS  - AVG_STEP_DIFF_MS)        // 122.5
#define HIGH_STEP_TIME_MS   (UPPER_STEP_TIME_MS  + AVG_STEP_DIFF_MS)        // 172.5

#define LED_PWM_OFF  (0)
#define PW_PERIOD    (255)            // Period of PWM timer
#define MIN_WHITE_PW (PW_PERIOD / 10) // relative pulse width
#define MAX_WHITE_PW (PW_PERIOD)      // relative pulse width
#define MIN_IR_PW    (PW_PERIOD / 10) // relative pulse width
#define MAX_IR_PW    (PW_PERIOD)      // relative pulse width

#define BUTTON_PRESSED (GPIO_PIN_SET)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

// todo make these const
static uint8_t whitePWM [ BRIGHTNESS_STEPS ]; // pulse width out of MAX_WHITE_PW
static uint8_t irPWM    [ BRIGHTNESS_STEPS ]; // pulse width out of MAX_IR_PW

// static const uint8_t whitePWM = {26, 30, 35, 40, 44, 49, 54, 58, 63, 68,
                                 // 72, 77, 82, 86, 91, 96, 100, 105, 110, 114,
                                 // 119, 124, 129, 133, 138, 143, 147, 152, 157, 161,
                                 // 166, 171, 175, 180, 185, 189, 194, 199, 203, 208,
                                 // 213, 218, 222, 227, 232, 236, 241, 246, 250, 255};
// static const uint8_t irPWM    = {26, 30, 35, 40, 44, 49, 54, 58, 63, 68,
                                 // 72, 77, 82, 86, 91, 96, 100, 105, 110, 114,
                                 // 119, 124, 129, 133, 138, 143, 147, 152, 157, 161,
                                 // 166, 171, 175, 180, 185, 189, 194, 199, 203, 208,
                                 // 213, 218, 222, 227, 232, 236, 241, 246, 250, 255};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

void   initWhitePWM(void); // Init whitePWM var
void   initIRPWM(void);    // Init irPWM var
int8_t decreaseBrightness(uint8_t isWhite, int8_t brightness, GPIO_PinState prevState); // decrease selected LED brightness
int8_t increaseBrightness(uint8_t isWhite, int8_t brightness, GPIO_PinState prevState); // increase selected LED brightness
GPIO_PinState inline isTogglePressed ( void );
GPIO_PinState inline isDimPressed    ( void );
GPIO_PinState inline isBrightPressed ( void );
void          inline restartDelayCounter(void);         // restart the counter
uint8_t       inline delayHit(uint32_t delay_ms);   // is delay in ms hit
void          inline setWhitePWM(uint8_t pulse_width); // set White PWM
void          inline setIRPWM(uint8_t pulse_width);    // set IR PWM

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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  printf("Begin\n");

  initWhitePWM();
  initIRPWM();

  GPIO_PinState prevDimPressed    = GPIO_PIN_RESET;
  GPIO_PinState prevBrightPressed = GPIO_PIN_RESET;

  uint8_t isWhite = 1; // 0 = IR, 1 = White
  int8_t whiteBrightness = HALF_BRIGHTNESS; // should never be negative but don't want underflow
  int8_t irBrightness    = HALF_BRIGHTNESS; // should never be negative but don't want underflow

  setWhitePWM(whitePWM[(uint8_t)whiteBrightness]);
  setIRPWM(LED_PWM_OFF);
  HAL_GPIO_WritePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin, GPIO_PIN_SET);

  HAL_TIM_Base_Start(&htim2);
  restartDelayCounter();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    const int8_t currBrightness    = isWhite ? whiteBrightness : irBrightness;
    const uint16_t brightnessDelay = ((LOW_STEP_TIME_MS + currBrightness) * HOLD_BRIGHTNESS_JUMP);

    const uint8_t brightnessDelayHit = delayHit(brightnessDelay);

    const GPIO_PinState togglePressed = isTogglePressed();
    const GPIO_PinState dimPressed    = isDimPressed();
    const GPIO_PinState brightPressed = isBrightPressed();

    if ((togglePressed == BUTTON_PRESSED) && delayHit(TOGGLE_DELAY_MS))
    {
      // toggle white/IR
      isWhite = !isWhite;
      HAL_GPIO_TogglePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin);
      if (isWhite)
      {
        // set white and disable IR
        setWhitePWM(irPWM[(uint8_t)whiteBrightness]);
        setIRPWM(LED_PWM_OFF);
      }
      else
      {
        // set IR and disable white
        setWhitePWM(LED_PWM_OFF);
        setIRPWM(irPWM[(uint8_t)irBrightness]);
      }

      restartDelayCounter();
    }
    else if ((dimPressed == BUTTON_PRESSED) && brightnessDelayHit)
    {
      if(isWhite) whiteBrightness = decreaseBrightness(isWhite, whiteBrightness, prevDimPressed);
    }
    else if ((brightPressed == BUTTON_PRESSED) && brightnessDelayHit)
    {
      if(isWhite) whiteBrightness = increaseBrightness(isWhite, whiteBrightness, prevBrightPressed);
      else        irBrightness    = increaseBrightness(isWhite, irBrightness, prevBrightPressed);
    }

    // save previous button state
    prevDimPressed    = dimPressed;
    prevBrightPressed = brightPressed;
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_TIM1;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 255;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 8000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BRIGHT_Pin DIM_Pin SWITCH_LED_Pin */
  GPIO_InitStruct.Pin = BRIGHT_Pin|DIM_Pin|SWITCH_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_STATUS_Pin */
  GPIO_InitStruct.Pin = LED_STATUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_STATUS_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// Init whitePWM var
// i = [MIN_WHITE_PW, MAX_WHITE_PW]
void initWhitePWM(void)
{
  const float range      = MAX_WHITE_PW - MIN_WHITE_PW;
  const float incPerStep = range / (BRIGHTNESS_STEPS - 1);
  for (uint8_t i = 0; i < BRIGHTNESS_STEPS; i++)
  {
    whitePWM[i] = (uint8_t)(i * incPerStep + 0.5) + MIN_WHITE_PW;
  }
  printf("Init %s PWM\n", "White");
  return;
}

// Init irPWM var
// i = [MIN_IR_PW, MAX_IR_PW]
void initIRPWM(void)
{
  const float range      = MAX_IR_PW - MIN_IR_PW;
  const float incPerStep = range / (BRIGHTNESS_STEPS - 1);
  for (uint8_t i = 0; i < BRIGHTNESS_STEPS; i++)
  {
    irPWM[i] = (uint8_t)(i * incPerStep + 0.5) + MIN_IR_PW;
  }
  printf("Init %s PWM\n", "IR");
  return;
}

int8_t decreaseBrightness(uint8_t isWhite, int8_t brightness, GPIO_PinState prevState)
{
  // decrease brightness
  if (prevState == BUTTON_PRESSED) brightness -= HOLD_BRIGHTNESS_JUMP;
  else                             brightness--;

  // prevent from going below MIN_BRIGHTNESS
  if (brightness < MIN_BRIGHTNESS) brightness = MIN_BRIGHTNESS;

  if (isWhite) printf("%s decreased to %d / %d\n", "White", brightness, MAX_BRIGHTNESS);
  else         printf("%s decreased to %d / %d\n", "IR", brightness, MAX_BRIGHTNESS);

  // set brightness
  if (isWhite) setWhitePWM(whitePWM[(uint8_t)brightness]);
  else         setIRPWM(irPWM[(uint8_t)brightness]);

  restartDelayCounter();

  return (brightness);
}

int8_t increaseBrightness(uint8_t isWhite, int8_t brightness, GPIO_PinState prevState)
{
  // increase brightness
  if (prevState == BUTTON_PRESSED) brightness += HOLD_BRIGHTNESS_JUMP;
  else                             brightness++;

  // prevent from going above MAX_BRIGHTNESS
  if (brightness > MAX_BRIGHTNESS) brightness = MAX_BRIGHTNESS;

  if (isWhite) printf("%s increased to %d / %d\n", "White", brightness, MAX_BRIGHTNESS);
  else         printf("%s increased to %d / %d\n", "IR", brightness, MAX_BRIGHTNESS);

  // set brightness
  if (isWhite) setWhitePWM(whitePWM[(uint8_t)brightness]);
  else         setIRPWM(irPWM[(uint8_t)brightness]);

  restartDelayCounter();

  return (brightness);
}

GPIO_PinState inline isTogglePressed ( void )
{
  GPIO_PinState pressed = (HAL_GPIO_ReadPin(SWITCH_LED_GPIO_Port, SWITCH_LED_Pin));
  return pressed;
}
GPIO_PinState inline isDimPressed    ( void )
{
  GPIO_PinState pressed = (HAL_GPIO_ReadPin(DIM_GPIO_Port, DIM_Pin));
  return pressed;
}
GPIO_PinState inline isBrightPressed ( void )
{
  GPIO_PinState pressed = (HAL_GPIO_ReadPin(BRIGHT_GPIO_Port, BRIGHT_Pin));
  return pressed;
}

// set White PWM
void inline setWhitePWM(uint8_t pulse_width)
{
  // set white pwm to pulse_width
  if (pulse_width == LED_PWM_OFF)
  {
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    printf("Turn off %s\n", "White");
  }
  else
  {
    TIM1->CCR1 = pulse_width;
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    printf("Turn on %s to %u / %u \n", "White", pulse_width, PW_PERIOD);
  }
}

// set IR PWM
void inline setIRPWM(uint8_t pulse_width)
{
  // set IR pwm to pulse_width
  if (pulse_width == LED_PWM_OFF)
  {
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
    printf("Turn off %s\n", "IR");
  }
  else
  {
    TIM1->CCR2 = pulse_width;
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    printf("Turn on %s to %u / %u\n", "IR", pulse_width, PW_PERIOD);
  }
  return;
}

void inline restartDelayCounter(void)
{
  TIM2->CNT = 0;
  return;
}

uint8_t inline delayHit(uint32_t delay_ms)
{
  uint8_t isDelayHit = (TIM2->CNT >= (delay_ms * TIM2_CLK_KHZ));
  return isDelayHit;
}

int fputc(int c, FILE *stream)
{
  (void)stream;
  HAL_UART_Transmit(&huart2, (uint8_t *) &c, sizeof(uint8_t), 0xFFFF);
  return c;
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
