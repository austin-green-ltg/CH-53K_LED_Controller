// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: stm32l412xx-bsp.h
//
// Description: Board Support Package for STM32L412xx
//
// Revision History:
// Date       - Name         -  Ver -  Remarks
// 07/31/2024 - Austin Green -  1.0 -  Initial Document
//
// Notes: This uses the Low Level ST API to access the board pins
//
// ***************************************************************************

#ifndef INC_bsph
    #define INC_bsph

    #include <stdint.h>

    /* Private defines -----------------------------------------------------------*/
    #ifdef STM32L412xx

        #include "stm32l4xx_ll_adc.h"
        #include "stm32l4xx_ll_crs.h"
        #include "stm32l4xx_ll_rcc.h"
        #include "stm32l4xx_ll_bus.h"
        #include "stm32l4xx_ll_system.h"
        #include "stm32l4xx_ll_exti.h"
        #include "stm32l4xx_ll_cortex.h"
        #include "stm32l4xx_ll_utils.h"
        #include "stm32l4xx_ll_pwr.h"
        #include "stm32l4xx_ll_dma.h"
        #include "stm32l4xx_ll_spi.h"
        #include "stm32l4xx_ll_tim.h"
        #include "stm32l4xx_ll_usart.h"
        #include "stm32l4xx_ll_gpio.h"

        #if defined(USE_FULL_ASSERT)
            #include "stm32_assert.h"
        #endif /* USE_FULL_ASSERT */

        /* Peripherals */
        #include "adc.h"
        #include "gpio.h"
        #include "spi.h"
        #include "tim.h"

        #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
            /* Peripherals enabled for UART */
            #include "usart.h"
        #endif /* ENABLE_UART_DEBUGGING */

        #define THERMISTOR_ADC_Pin LL_GPIO_PIN_0
        #define THERMISTOR_ADC_GPIO_Port GPIOA
        #define BRIGHT_Pin LL_GPIO_PIN_1
        #define BRIGHT_GPIO_Port GPIOA
        #define DIM_Pin LL_GPIO_PIN_3
        #define DIM_GPIO_Port GPIOA
        #define EEPROM_SCK_Pin LL_GPIO_PIN_5
        #define EEPROM_SCK_GPIO_Port GPIOA
        #define EEPROM_MISO_Pin LL_GPIO_PIN_6
        #define EEPROM_MISO_GPIO_Port GPIOA
        #define EEPROM_MOSI_Pin LL_GPIO_PIN_7
        #define EEPROM_MOSI_GPIO_Port GPIOA
        #define VOLTMETER_ADC_Pin LL_GPIO_PIN_0
        #define VOLTMETER_ADC_GPIO_Port GPIOB
        #define AMPMETER_ADC_Pin LL_GPIO_PIN_1
        #define AMPMETER_ADC_GPIO_Port GPIOB
        #define PWM_OUT_Pin LL_GPIO_PIN_8
        #define PWM_OUT_GPIO_Port GPIOA

    #else /* STM32L412xx */

        /* Below is for debugging purposes */
        #define THERMISTOR_ADC_Pin          0
        #define THERMISTOR_ADC_GPIO_Port    0
        #define BRIGHT_Pin                  0
        #define BRIGHT_GPIO_Port            0
        #define DIM_Pin                     0
        #define DIM_GPIO_Port               0
        #define EEPROM_SCK_Pin              0
        #define EEPROM_SCK_GPIO_Port        0
        #define EEPROM_MISO_Pin             0
        #define EEPROM_MISO_GPIO_Port       0
        #define EEPROM_MOSI_Pin             0
        #define EEPROM_MOSI_GPIO_Port       0
        #define VOLTMETER_ADC_Pin           0
        #define VOLTMETER_ADC_GPIO_Port     0
        #define AMPMETER_ADC_Pin            0
        #define AMPMETER_ADC_GPIO_Port      0
        #define PWM_OUT_Pin                 0
        #define PWM_OUT_GPIO_Port           0

        typedef struct
        {
            uint8_t is_running;
            uint32_t pulse_width;
        } PwmStruct;

        typedef struct
        {
            uint8_t is_running;
            uint32_t time;
        } TimerStruct;

    #endif /* STM32L412xx */

    /* Interrupt Handlers */
    #ifndef NVIC_PRIORITYGROUP_0
        #define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                            4 bits for subpriority */
        #define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                            3 bits for subpriority */
        #define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                            2 bits for subpriority */
        #define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                            1 bit  for subpriority */
        #define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                            0 bit  for subpriority */
    #endif

    /* Button Defines */
    typedef uint8_t GPIO_PinState;
    enum { BUTTON_PRESSED = 1, BUTTON_UNPRESSED = 0};

    /* Clock frequency Values */
    #define CLK_FREQ_HZ  (8000000)
    #define TIM2_CLK_DEV  (1)
    #define TIM2_CLK_PRESCALER  (8000)

    /* Returns button state */
    GPIO_PinState ReadDimPin( void );
    GPIO_PinState ReadBrightPin( void );
    /* PWM Outputs */
    void EnablePWM1( void );
    void DisablePWM1( void );
    void StartPWM11( void );
    void StopPWM11( void );
    void SetPW11( uint32_t pulse_width );
    /* Timers */
    void StartTIM2( void );
    void RestartTIM2( void );
    uint32_t GetTIM2Cnt( void );
    /* Thermistor */
    int32_t GetThermistorValue( void );
    /* Current Sensing */
    int32_t GetCurrentValue( void );
    /* Voltage Sensing */
    int32_t GetVoltageValue( void );
    /* Logging */
    void WriteMem( const uint32_t address, const char* const string, const uint32_t bytes );
    void ReadMem(  const uint32_t address, char* string, const uint32_t bytes );
    /* UART Output */
    void sendUARTChar(char c);
    /* Errors */
    void Error_Handler(void);


#endif /* INC_bsph */
