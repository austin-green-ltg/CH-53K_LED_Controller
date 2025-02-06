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
// #include "dma.h"
// #include "spi.h"
// #include "tim.h"
#include "usart.h"
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
#include "com.h"
#include "string.h"
#include "logger.h"

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

#define LOG_DELAY_MS        (600000) // 10 minutes
#define LIVE_LOG_DELAY_MS   (1000)   // 1 second

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config ( void );
/* USER CODE BEGIN PFP */

void LogVitals ( void );
void FindVitalsEOL ( void );
void EraseLogs ( void );
int verifyLogRead ( void );

void LL_ADC_Start_DMA ( uint32_t* pData,
                        uint32_t Length );
void LL_DMA_Start_IT ( uint32_t SrcAddress,
                       uint32_t DstAddress, uint32_t DataLength );

void LL_DMA_SetConfig ( uint32_t SrcAddress,
                        uint32_t DstAddress, uint32_t DataLength );

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
    MX_TIM15_Init();
    MX_TIM16_Init();
    MX_USART2_UART_Init();
    MX_ADC1_Init();
    MX_SPI1_Init();
    MX_USB_DEVICE_Init();
    /* USER CODE BEGIN 2 */

    LL_ADC_Start_DMA ( ( uint32_t* ) value_adc, 3 );
    InitPwm();

    GPIO_PinState prevDimPressed = BUTTON_UNPRESSED;
    GPIO_PinState prevBrightPressed = BUTTON_UNPRESSED;

    StartDelayCounter();
    StartLogDelayCounter();
    StartLiveLogDelayCounter();

    // Init Logs
    FindVitalsEOL();

    // if brightness has changed since last log
    uint8_t isBrightnessChanged = 0;

    // EraseLogs();

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

        // log levels
        if ( LogDelayHit ( LOG_DELAY_MS ) )
        {
            LogVitals();
            RestartLogDelayCounter();
        }

        // log levels
        if ( LiveLogDelayHit ( LIVE_LOG_DELAY_MS ) )
        {
            sendLiveLogs();
            RestartLiveLogDelayCounter();
        }

        uint8_t response [ 2 ] = {0, 0};
        checkLine ( response );

        uint8_t response_type = response [ 0 ];
        uint8_t response_num = response [ 1 ];

        if ( response_type != 0 )
        {
            sendRecordedLogs ( response_type, response_num );
            RestartLiveLogDelayCounter();
        }

        // int8_t isLogCorrect = verifyLogRead();

        // if (isLogCorrect != 0)
        // {
        // DisablePWM1();
        // return isLogCorrect;
        // }

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

void FindVitalsEOL ( void )
{
    findTemperatureLogEOL();
    findCurrentLogEOL();
    findVoltageLogEOL();
}

// Writes Zero to Log Space
void EraseLogs ( void )
{
#define TOTAL_SPACE (PWM_LOG_SPACE + CURRENT_LOG_SPACE + TEMPERATURE_LOG_SPACE + VOLTAGE_LOG_SPACE)
    uint8_t zero_array [ TOTAL_SPACE ];
    memset ( zero_array, 0, TOTAL_SPACE );

    WriteLog ( STARTING_PWM_ADDRESS, ( char const* ) zero_array, TOTAL_SPACE );
}

// Verify that the pwm log is being written correctly
int verifyLogRead ( void )
{
    char stringVis [ PWM_LOG_SIZE ];
    char stringIr [ PWM_LOG_SIZE ];
    char stringPrevInit [ PWM_LOG_SIZE ];

    ReadLog ( STARTING_PWM_ADDRESS, stringVis,
              PWM_LOG_SIZE );

    ReadLog ( STARTING_PWM_ADDRESS + PWM_LOG_SIZE, stringIr,
              PWM_LOG_SIZE );

    ReadLog ( STARTING_PWM_ADDRESS + 2 * PWM_LOG_SIZE, stringPrevInit,
              PWM_LOG_SIZE );

    uint8_t isVisCorrect = ( int8_t ) stringVis [ 0 ] == GetBrightness ( 0 );
    uint8_t isIrCorrect = ( int8_t ) stringIr [ 0 ] == GetBrightness ( 1 );
    uint8_t isPrevInitCorrect = ( int8_t ) stringPrevInit [ 0 ] == 1;

    if (!isPrevInitCorrect )
    {
        return -1;
    }
    else if (!isVisCorrect )
    {
        return -2;
    }
    else if (!isIrCorrect )
    {
        return -3;
    }
    else
    {
        return 0;
    }
}

void LL_ADC_Start_DMA ( uint32_t* pData,
                        uint32_t Length )
{
    /* Perform ADC enable and conversion start if no conversion is on going */
    /* Ensure that multimode regular conversions are not enabled.   */
    /* Otherwise, dedicated API HAL_ADCEx_MultiModeStart_DMA() must be used.  */
    /* Enable the ADC peripheral */
    /* ADC enable and wait for ADC ready (in case of ADC is disabled or         */
    /* enabling phase not yet completed: flag ADC ready not yet set).           */
    /* Timeout implemented to not be stuck if ADC cannot be enabled (possible   */
    /* causes: ADC clock not running, ...).                                     */
    if ( LL_ADC_IsEnabled ( ADC1 ) == 0UL )
    {
        /* Enable the ADC peripheral */
        LL_ADC_Enable ( ADC1 );

        while ( LL_ADC_IsActiveFlag_ADRDY ( ADC1 ) == 0UL )
        {
            /*  If ADEN bit is set less than 4 ADC clock cycles after the ADCAL bit
                has been cleared (after a calibration), ADEN bit is reset by the
                calibration logic.
                The workaround is to continue setting ADEN until ADRDY is becomes 1.
                Additionally, ADC_ENABLE_TIMEOUT is defined to encompass this
                4 ADC clock cycle duration */
            /* Note: Test of ADC enabled required due to hardware constraint to     */
            /*       not enable ADC if already enabled.                             */
            if ( LL_ADC_IsEnabled ( ADC1 ) == 0UL )
            {
                LL_ADC_Enable ( ADC1 );
            }
        }
    }

    /* Start conversion if ADC is effectively enabled */
    /* Set ADC state                                                        */
    /* - Clear state bitfield related to regular group conversion results   */
    /* - Set state bitfield related to regular operation                    */
    LL_ADC_ClearFlag_ADRDY ( ADC1 );
    LL_ADC_ClearFlag_EOC ( ADC1 );
    LL_ADC_ClearFlag_OVR ( ADC1 );
    LL_ADC_ClearFlag_EOSMP ( ADC1 );

    /* Manage ADC and DMA start: ADC overrun interruption, DMA start,     */
    /* ADC start (in case of SW start):                                   */

    /* Clear regular group conversion flag and overrun flag               */
    /* (To ensure of no unknown state from potential previous ADC         */
    /* operations)                                                        */
    LL_ADC_ClearFlag_EOC ( ADC1 );
    LL_ADC_ClearFlag_EOS ( ADC1 );
    LL_ADC_ClearFlag_OVR ( ADC1 );

    /* With DMA, overrun event is always considered as an error even if
       hadc->Init.Overrun is set to ADC_OVR_DATA_OVERWRITTEN. Therefore,
       ADC_IT_OVR is enabled. */
    LL_ADC_EnableIT_OVR ( ADC1 );

    /* Enable ADC DMA mode */
    LL_ADC_REG_SetDMATransfer ( ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED );

    /* Start the DMA channel */
    LL_DMA_Start_IT ( ( uint32_t ) &ADC1->DR, ( uint32_t ) pData,
                      Length );

    /* Enable conversion of regular group.                                  */
    /* If software start has been selected, conversion starts immediately.  */
    /* If external trigger has been selected, conversion will start at next */
    /* trigger event.                                                       */
    /* Start ADC group regular conversion */
    LL_ADC_REG_StartConversion ( ADC1 );
}

void LL_DMA_Start_IT ( uint32_t SrcAddress,
                       uint32_t DstAddress, uint32_t DataLength )
{

    /* Disable the peripheral */
    LL_DMA_DisableChannel ( DMA1, LL_DMA_CHANNEL_1 );

    /* Configure the source, destination address and the data length & clear flags*/
    LL_DMA_SetConfig ( SrcAddress, DstAddress, DataLength );

    /* Enable the transfer complete interrupt */
    /* Enable the transfer Error interrupt */
    /* Enable the Half transfer complete interrupt as well */
    LL_DMA_EnableIT_TC ( DMA1, LL_DMA_CHANNEL_1 );
    LL_DMA_EnableIT_HT ( DMA1, LL_DMA_CHANNEL_1 );
    LL_DMA_EnableIT_TE ( DMA1, LL_DMA_CHANNEL_1 );

    /* Enable the Peripheral */
    LL_DMA_EnableChannel ( DMA1, LL_DMA_CHANNEL_1 );
}

void LL_DMA_SetConfig ( uint32_t SrcAddress,
                        uint32_t DstAddress, uint32_t DataLength )
{
    /* Clear all flags */
    LL_DMA_ClearFlag_GI1 ( DMA1 );
    LL_DMA_ClearFlag_TC1 ( DMA1 );
    LL_DMA_ClearFlag_HT1 ( DMA1 );
    LL_DMA_ClearFlag_TE1 ( DMA1 );

    /* Configure DMA Channel data length */
    // DMA1_Channel1->CNDTR = DataLength;
    LL_DMA_SetDataLength ( DMA1, LL_DMA_CHANNEL_1, DataLength );

    /* Configure DMA Channel source and destination address */
    LL_DMA_ConfigAddresses ( DMA1, LL_DMA_CHANNEL_1, SrcAddress, DstAddress,
                             LL_DMA_DIRECTION_PERIPH_TO_MEMORY );
    // DMA1_Channel1->CPAR = SrcAddress;
    // DMA1_Channel1->CMAR = DstAddress;
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
