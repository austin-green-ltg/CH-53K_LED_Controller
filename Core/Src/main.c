/* USER CODE BEGIN Header */
/****
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
#include "bsp.h"
#include "pwm_handler.h"
#include "delay_handler.h"
#include "button_handler.h"
#include "current_handler.h"
#include "voltage_handler.h"
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

const float     LowStepTimeMs   = (LOWER_STEP_TIME_MS  - AVG_STEP_DIFF_MS)  ; // 122.5
const float     HighStepTimeMs  = (UPPER_STEP_TIME_MS  + AVG_STEP_DIFF_MS)  ; // 172.5
const uint8_t   ToggleDelayMs   = (250)                                     ;
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
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    /* System interrupt init*/
    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

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
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        MX_USART2_UART_Init();
    #endif /* ENABLE_UART_DEBUGGING */
    MX_TIM3_Init();
    /* USER CODE BEGIN 2 */

    InitWhitePwm();
    InitIrPwm();

    GPIO_PinState prevDimPressed    = BUTTON_UNPRESSED;
    GPIO_PinState prevBrightPressed = BUTTON_UNPRESSED;

    uint8_t isWhite = 1; // 0 = IR, 1 = White

    SetWhitePwm();
    TurnOffIrPwm();

    StartDelayCounter();
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        LL_TIM_EnableCounter(TIM3);
        TIM3->CNT = 0;
    #endif /* ENABLE_UART_DEBUGGING */

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

        if (GetCurrentRange() == CurrentError)
        {
            // TODO: Do something
        }

        if (GetVoltageRange() == VoltageErrorLow || GetVoltageRange() == VoltageErrorHigh)
        {
            // TODO: Do something
        }

        const int8_t CurrBrightness    = isWhite ? GetWhiteBrightness() : GetIRBrightness();
        const uint16_t BrightnessDelay = (uint16_t)((LowStepTimeMs + CurrBrightness) * HOLD_BRIGHTNESS_JUMP);

        uint8_t brightnessDelayHit = DelayHit(BrightnessDelay);

        GPIO_PinState togglePressed = IsTogglePressed();
        GPIO_PinState dimPressed    = IsDimPressed();
        GPIO_PinState brightPressed = IsBrightPressed();

        if ((togglePressed == BUTTON_PRESSED) && DelayHit((uint32_t)ToggleDelayMs))
        {
            // toggle white/IR
            isWhite = !isWhite;
            if (isWhite)
            {
                // set white and disable IR
                SetWhitePwm();
                TurnOffIrPwm();
            }
            else
            {
                // set IR and disable white
                TurnOffWhitePwm();
                SetIrPwm();
            }

            RestartDelayCounter();
        }
        else if ((dimPressed == BUTTON_PRESSED) && brightnessDelayHit)
        {
            if(isWhite) DecreaseWhiteBrightness(prevDimPressed);
            else        DecreaseIRBrightness(prevDimPressed);

            RestartDelayCounter();
        }
        else if ((brightPressed == BUTTON_PRESSED) && brightnessDelayHit)
        {
            if(isWhite) IncreaseWhiteBrightness(prevBrightPressed);
            else        IncreaseIRBrightness(prevBrightPressed);

            RestartDelayCounter();
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
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
    while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
    {
    }
    LL_RCC_HSI_Enable();

    /* Wait till HSI is ready */
    while(LL_RCC_HSI_IsReady() != 1)
    {

    }
    LL_RCC_HSI_SetCalibTrimming(16);
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

    /* Wait till System clock is ready */
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
    {

    }
    LL_Init1msTick(8000000);
    LL_SetSystemCoreClock(8000000);
    LL_RCC_SetTIMClockSource(LL_RCC_TIM1_CLKSOURCE_PCLK2);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
