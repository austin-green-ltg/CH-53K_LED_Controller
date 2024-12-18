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
// #include "adc.h"
// #include "spi.h"
// #include "tim.h"
// #include "usart.h"
#include "usb_device.h"
// #include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pwm_handler.h"
#include "delay_handler.h"
#include "button_handler.h"
#include "current_handler.h"
#include "voltage_handler.h"
#include "temperature_handler.h"
#include "my_printf.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* Delay timings for sweeping brightness */
#define LOWER_SWEEP_TIME_MS  (3375)                                             // time from 50%->0%
#define UPPER_SWEEP_TIME_MS  (4000)                                             // time from 50%->100%
#define TOTAL_SWEEP_TIME_MS (7375)                                              // time from 0%->100%
#define LOWER_STEP_TIME_MS   (LOWER_SWEEP_TIME_MS / (BRIGHTNESS_STEPS / 2.0f))  // 135, time lower per step
#define UPPER_STEP_TIME_MS   (UPPER_SWEEP_TIME_MS / (BRIGHTNESS_STEPS / 2.0f))  // 160, time upper per step
#define AVG_STEP_TIME_MS     ((UPPER_STEP_TIME_MS + LOWER_STEP_TIME_MS) / 2.0f) // 147.5
#define AVG_STEP_DIFF_MS     (AVG_STEP_TIME_MS    - LOWER_STEP_TIME_MS)         // 12.5, distance between lower step time and average step time

#define LOG_DELAY_MS    (1000)

const float LowStepTimeMs = ( LOWER_STEP_TIME_MS - AVG_STEP_DIFF_MS )
                            ; // 122.5
const float HighStepTimeMs = ( UPPER_STEP_TIME_MS + AVG_STEP_DIFF_MS )
                             ; // 172.5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config ( void );
/* USER CODE BEGIN PFP */

void LogVitals ( void );

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * Initialize variables and go into bare metal loop.
  * Polls buttons and sensors.
  * @retval int
  */
int main ( void )
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
    MX_TIM6_Init();
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
    /* Peripherals enabled for UART */
    MX_TIM15_Init();
    MX_USART2_UART_Init();
#endif /* ENABLE_UART_DEBUGGING */
    MX_ADC1_Init();
    MX_SPI1_Init();
    MX_USB_DEVICE_Init();
    /* USER CODE BEGIN 2 */

    InitPwm();

    GPIO_PinState prevDimPressed = BUTTON_UNPRESSED;
    GPIO_PinState prevBrightPressed = BUTTON_UNPRESSED;

    SetPwm ( 1 );

    StartDelayCounter();
    StartLogDelayCounter();
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
    /* Peripherals enabled for UART */
    LL_TIM_EnableCounter ( TIM15 );
    TIM15->CNT = 0;
#endif /* ENABLE_UART_DEBUGGING */

    // if brightness has changed since last log
    uint8_t isBrightnessChanged = 0;

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while ( 1 )
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

        GPIO_PinState onOffPressed = IsOnOffPressed(); // 0 = Off, 1 = On

        if ( onOffPressed )
        {
            EnablePWM1();
            GPIO_PinState togglePressed = IsTogglePressed(); // 0 = IR, 1 = Visible
            SetPwm (!togglePressed );

            const int8_t CurrBrightness = GetBrightness (!togglePressed );;
            const uint16_t BrightnessDelay_ms = ( uint16_t ) ( ( LowStepTimeMs +
                                                CurrBrightness ) * HOLD_BRIGHTNESS_JUMP );

            uint8_t brightnessDelayHit = DelayHit ( BrightnessDelay_ms );

            GPIO_PinState dimPressed = IsDimPressed();
            GPIO_PinState brightPressed = IsBrightPressed();

            if ( ( dimPressed == BUTTON_PRESSED ) && brightnessDelayHit )
            {
                DecreaseBrightness ( prevDimPressed, !togglePressed );

                RestartDelayCounter();

                isBrightnessChanged = 1; // mark brightness as changed
            }
            else if ( ( brightPressed == BUTTON_PRESSED ) && brightnessDelayHit )
            {
                IncreaseBrightness ( prevBrightPressed, !togglePressed );

                RestartDelayCounter();

                isBrightnessChanged = 1; // mark brightness as changed
            }

            // Check to see if brightness has changed
            if ( isBrightnessChanged )
            {
                // If so, log
                LogPwm();
                isBrightnessChanged = 0;
            }

            // save previous button state
            prevDimPressed = dimPressed;
            prevBrightPressed = brightPressed;
        }
        else
        {
            DisablePWM1();
            prevDimPressed = BUTTON_UNPRESSED;
            prevBrightPressed = BUTTON_UNPRESSED;
        }

        // log brightness levels
        if ( LogDelayHit ( LOG_DELAY_MS ) )
        {
            LogVitals();
            RestartLogDelayCounter();
        }
    }

    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config ( void )
{
    LL_FLASH_SetLatency ( LL_FLASH_LATENCY_4 );

    while ( LL_FLASH_GetLatency() != LL_FLASH_LATENCY_4 )
    {
    }

    LL_PWR_SetRegulVoltageScaling ( LL_PWR_REGU_VOLTAGE_SCALE1 );

    while ( LL_PWR_IsActiveFlag_VOS() != 0 )
    {
    }

    LL_RCC_HSI_Enable();

    /* Wait till HSI is ready */
    while ( LL_RCC_HSI_IsReady() != 1 )
    {

    }

    LL_RCC_HSI_SetCalibTrimming ( 64 );
    LL_RCC_MSI_Enable();

    /* Wait till MSI is ready */
    while ( LL_RCC_MSI_IsReady() != 1 )
    {

    }

    LL_RCC_MSI_EnableRangeSelection();
    LL_RCC_MSI_SetRange ( LL_RCC_MSIRANGE_11 );
    LL_RCC_MSI_SetCalibTrimming ( 0 );
    LL_RCC_PLL_ConfigDomain_SYS ( LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 10,
                                  LL_RCC_PLLR_DIV_2 );
    LL_RCC_PLL_EnableDomain_SYS();
    LL_RCC_PLL_Enable();

    /* Wait till PLL is ready */
    while ( LL_RCC_PLL_IsReady() != 1 )
    {

    }

    LL_RCC_SetSysClkSource ( LL_RCC_SYS_CLKSOURCE_PLL );

    /* Wait till System clock is ready */
    while ( LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL )
    {

    }

    LL_RCC_SetAHBPrescaler ( LL_RCC_SYSCLK_DIV_1 );
    LL_RCC_SetAPB1Prescaler ( LL_RCC_APB1_DIV_1 );
    LL_RCC_SetAPB2Prescaler ( LL_RCC_APB2_DIV_1 );
    LL_SetSystemCoreClock ( 80000000 );

    /* Update the time base */
    if ( HAL_InitTick ( TICK_INT_PRIORITY ) != HAL_OK )
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

void LogVitals ( void )
{
    LogTemperature();
    LogCurrent();
    LogVoltage();
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler ( void )
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();

    while ( 1 )
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
void assert_failed ( uint8_t* file, uint32_t line )
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}

#endif /* USE_FULL_ASSERT */
