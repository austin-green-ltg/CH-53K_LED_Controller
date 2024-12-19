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
#include "dma.h"
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

#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES  (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)

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

extern uint16_t value_adc [ 3 ];

/* Delay between ADC end of calibration and ADC enable.                     */
/* Delay estimation in CPU cycles: Case of ADC enable done                  */
/* immediately after ADC calibration, ADC clock setting slow                */
/* (LL_ADC_CLOCK_ASYNC_DIV32). Use a higher delay if ratio                  */
/* (CPU clock / ADC clock) is above 32.                                     */
#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES  (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)

/* Definitions of data related to this example */
/* Init variable out of expected ADC conversion data range */
#define VAR_CONVERTED_DATA_INIT_VALUE    (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B) + 1)

/* Variables for ADC conversion data */
__IO uint16_t uhADCxConvertedData =
    VAR_CONVERTED_DATA_INIT_VALUE; /* ADC group regular conversion data */

/* Variables for ADC conversion data computation to physical values */
uint16_t uhADCxConvertedData_Voltage_mVolt =
    0; /* Value of voltage calculated from ADC conversion data (unit: mV) */

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
    MX_DMA_Init();
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

    HAL_ADC_Start_DMA (&hadc1, ( uint32_t* ) value_adc, 3 );

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

    /* Activate ADC */
    /* Perform ADC activation procedure to make it ready to convert. */
    // ADC_Activate();

    /* Perform ADC group regular conversion start, poll for conversion        */
    /* completion.                                                            */
    // ConversionStartPoll_ADC_GrpRegular();

    EnablePWM1();
    StartPWM12();

    const uint16_t expected_voltage_mv = 22500;

    uint16_t temp_dc = 0;
    uint16_t temp_out = 0;

    uint16_t out_voltage_mv = 0;
    uint16_t conv_out = 0;

    uint16_t current_ma = 0;
    uint16_t current_out = 0;

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while ( 1 )
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

        // Temperature
        temp_dc = GetTemperature() ;
        // Convert to show actual tempurature in dC(?) on voltmeter
        temp_out = ( int16_t ) ( ( float ) temp_dc / 10.0f + 0.5f ) + 3u;

        // Voltage
        out_voltage_mv = GetVoltage() * 100 ;
        // Convert to show actual voltage in V(?) on voltmeter
        conv_out = ( int16_t ) ( ( float ) out_voltage_mv * 255.0f /
                                 expected_voltage_mv / 10.0f + 0.5f );
        // Current
        current_ma = GetCurrent() * 100 ;
        // Convert to show actual current in A(?) on voltmeter
        current_out = ( int16_t ) ( ( float ) current_ma * 255.0f / expected_voltage_mv
                                    + 0.5f );

        GPIO_PinState onOffPressed = IsOnOffPressed(); // 0 = Off, 1 = On
        GPIO_PinState togglePressed = IsTogglePressed(); // 0 = IR, 1 = Visible

        if ( onOffPressed )
        {

            if ( togglePressed )
            {
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
                SetPW12 ( temp_out );
                StartPWM12();
            }
        }
        else
        {
            if ( togglePressed )
            {
                SetPW12 ( conv_out );
                StartPWM12();
            }
            else
            {
                SetPW12 ( current_out );
                StartPWM12();
            }

            prevDimPressed = BUTTON_UNPRESSED;
            prevBrightPressed = BUTTON_UNPRESSED;
        }

        // log levels
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

/**
  * @brief  Perform ADC activation procedure to make it ready to convert
  *         (ADC instance: ADC1).
  * @param  None
  * @retval None
  */
void ADC_Activate ( void )
{
    __IO uint32_t wait_loop_index = 0U;
    __IO uint32_t backup_setting_adc_dma_transfer = 0U;
#if (USE_TIMEOUT == 1)
    uint32_t Timeout = 0U; /* Variable used for timeout management */
#endif /* USE_TIMEOUT */

    /*## Operation on ADC hierarchical scope: ADC instance #####################*/

    /* Note: Hardware constraint (refer to description of the functions         */
    /*       below):                                                            */
    /*       On this STM32 series, setting of these features is conditioned to  */
    /*       ADC state:                                                         */
    /*       ADC must be disabled.                                              */
    /* Note: In this example, all these checks are not necessary but are        */
    /*       implemented anyway to show the best practice usages                */
    /*       corresponding to reference manual procedure.                       */
    /*       Software can be optimized by removing some of these checks, if     */
    /*       they are not relevant considering previous settings and actions    */
    /*       in user application.                                               */
    if ( LL_ADC_IsEnabled ( ADC1 ) == 0 )
    {
        /* Enable ADC internal voltage regulator */
        LL_ADC_EnableInternalRegulator ( ADC1 );

        /* Delay for ADC internal voltage regulator stabilization.                */
        /* Compute number of CPU cycles to wait for, from delay in us.            */
        /* Note: Variable divided by 2 to compensate partially                    */
        /*       CPU processing cycles (depends on compilation optimization).     */
        /* Note: If system core clock frequency is below 200kHz, wait time        */
        /*       is only a few CPU processing cycles.                             */
        wait_loop_index = ( ( LL_ADC_DELAY_INTERNAL_REGUL_STAB_US *
                              ( SystemCoreClock / ( 100000 * 2 ) ) ) / 10 );

        while ( wait_loop_index != 0 )
        {
            wait_loop_index--;
        }

        /* Disable ADC DMA transfer request during calibration */
        /* Note: Specificity of this STM32 series: Calibration factor is           */
        /*       available in data register and also transferred by DMA.           */
        /*       To not insert ADC calibration factor among ADC conversion data   */
        /*       in DMA destination address, DMA transfer must be disabled during */
        /*       calibration.                                                     */
        backup_setting_adc_dma_transfer = LL_ADC_REG_GetDMATransfer ( ADC1 );
        LL_ADC_REG_SetDMATransfer ( ADC1, LL_ADC_REG_DMA_TRANSFER_NONE );

        /* Run ADC self calibration */
        LL_ADC_StartCalibration ( ADC1, LL_ADC_SINGLE_ENDED );

        /* Poll for ADC effectively calibrated */
#if (USE_TIMEOUT == 1)
        Timeout = ADC_CALIBRATION_TIMEOUT_MS;
#endif /* USE_TIMEOUT */

        while ( LL_ADC_IsCalibrationOnGoing ( ADC1 ) != 0 )
        {
#if (USE_TIMEOUT == 1)

            /* Check Systick counter flag to decrement the time-out value */
            if ( LL_SYSTICK_IsActiveCounterFlag() )
            {
                if ( Timeout-- == 0 )
                {
                    /* Error: Time-out */
                    Error_Handler();
                }
            }

#endif /* USE_TIMEOUT */
        }

        /* Restore ADC DMA transfer request after calibration */
        LL_ADC_REG_SetDMATransfer ( ADC1, backup_setting_adc_dma_transfer );

        /* Delay between ADC end of calibration and ADC enable.                   */
        /* Note: Variable divided by 2 to compensate partially                    */
        /*       CPU processing cycles (depends on compilation optimization).     */
        wait_loop_index = ( ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1 );

        while ( wait_loop_index != 0 )
        {
            wait_loop_index--;
        }

        /* Enable ADC */
        LL_ADC_Enable ( ADC1 );

        /* Poll for ADC ready to convert */
#if (USE_TIMEOUT == 1)
        Timeout = ADC_ENABLE_TIMEOUT_MS;
#endif /* USE_TIMEOUT */

        while ( LL_ADC_IsActiveFlag_ADRDY ( ADC1 ) == 0 )
        {
#if (USE_TIMEOUT == 1)

            /* Check Systick counter flag to decrement the time-out value */
            if ( LL_SYSTICK_IsActiveCounterFlag() )
            {
                if ( Timeout-- == 0 )
                {
                    /* Error: Time-out */
                    Error_Handler();
                }
            }

#endif /* USE_TIMEOUT */
        }

        /* Note: ADC flag ADRDY is not cleared here to be able to check ADC       */
        /*       status afterwards.                                               */
        /*       This flag should be cleared at ADC Deactivation, before a new    */
        /*       ADC activation, using function "LL_ADC_ClearFlag_ADRDY()".       */
    }

    /*## Operation on ADC hierarchical scope: ADC group regular ################*/
    /* Note: No operation on ADC group regular performed here.                  */
    /*       ADC group regular conversions to be performed after this function  */
    /*       using function:                                                    */
    /*       "LL_ADC_REG_StartConversion();"                                    */

    /*## Operation on ADC hierarchical scope: ADC group injected ###############*/
    /* Note: Feature not available on this STM32 series */

}

/**
  * @brief  Perform ADC group regular conversion start, poll for conversion
  *         completion.
  *         (ADC instance: ADC1).
  * @note   This function does not perform ADC group regular conversion stop:
  *         intended to be used with ADC in single mode, trigger SW start
  *         (only 1 ADC conversion done at each trigger, no conversion stop
  *         needed).
  *         In case of continuous mode or conversion trigger set to
  *         external trigger, ADC group regular conversion stop must be added
  *         after polling for conversion data.
  * @param  None
  * @retval None
  */
void ConversionStartPoll_ADC_GrpRegular ( void )
{
#if (USE_TIMEOUT == 1)
    uint32_t Timeout = 0U; /* Variable used for timeout management */
#endif /* USE_TIMEOUT */

    /* Start ADC group regular conversion */
    /* Note: Hardware constraint (refer to description of the function          */
    /*       below):                                                            */
    /*       On this STM32 series, setting of this feature is conditioned to    */
    /*       ADC state:                                                         */
    /*       ADC must be enabled without conversion on going on group regular,  */
    /*       without ADC disable command on going.                              */
    /* Note: In this example, all these checks are not necessary but are        */
    /*       implemented anyway to show the best practice usages                */
    /*       corresponding to reference manual procedure.                       */
    /*       Software can be optimized by removing some of these checks, if     */
    /*       they are not relevant considering previous settings and actions    */
    /*       in user application.                                               */
    if ( ( LL_ADC_IsEnabled ( ADC1 ) == 1 ) &&
            ( LL_ADC_IsDisableOngoing ( ADC1 ) == 0 ) &&
            ( LL_ADC_REG_IsConversionOngoing ( ADC1 ) == 0 ) )
    {
        LL_ADC_REG_StartConversion ( ADC1 );
    }
    else
    {
        /* Error: ADC conversion start could not be performed */
        Error_Handler();
    }

#if (USE_TIMEOUT == 1)
    Timeout = ADC_UNITARY_CONVERSION_TIMEOUT_MS;
#endif /* USE_TIMEOUT */

    while ( LL_ADC_IsActiveFlag_EOC ( ADC1 ) == 0 )
    {
#if (USE_TIMEOUT == 1)

        /* Check Systick counter flag to decrement the time-out value */
        if ( LL_SYSTICK_IsActiveCounterFlag() )
        {
            if ( Timeout-- == 0 )
            {
                Error_Handler();
            }
        }

#endif /* USE_TIMEOUT */
    }

    /* Clear flag ADC group regular end of unitary conversion */
    /* Note: This action is not needed here, because flag ADC group regular   */
    /*       end of unitary conversion is cleared automatically when          */
    /*       software reads conversion data from ADC data register.           */
    /*       Nevertheless, this action is done anyway to show how to clear    */
    /*       this flag, needed if conversion data is not always read          */
    /*       or if group injected end of unitary conversion is used (for      */
    /*       devices with group injected available).                          */
    LL_ADC_ClearFlag_EOC ( ADC1 );

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
