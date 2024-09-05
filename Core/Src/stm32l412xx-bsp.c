// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: bsp.c
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

#include "stm32l412xx-bsp.h"

/* Private variables ---------------------------------------------------------*/

GPIO_PinState ReadDimPin( void )
{
    return (LL_GPIO_IsInputPinSet(DIM_GPIO_Port, DIM_Pin));
}

GPIO_PinState ReadBrightPin( void )
{
    return (LL_GPIO_IsInputPinSet(BRIGHT_GPIO_Port, BRIGHT_Pin));
}

void EnablePWM1( void )
{
    LL_TIM_EnableAllOutputs(TIM1);

    LL_TIM_EnableCounter(TIM1);
}

void DisablePWM1( void )
{
    LL_TIM_DisableAllOutputs(TIM1);

    LL_TIM_DisableCounter(TIM1);
}

void StartPWM11( void )
{
    LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
}

void StopPWM11( void )
{
    LL_TIM_CC_DisableChannel(TIM1, LL_TIM_CHANNEL_CH1);
}

void SetPW11( uint32_t pulse_width )
{
    LL_TIM_OC_SetCompareCH1(TIM1, pulse_width);
}

void StartTIM2( void )
{
    LL_TIM_EnableCounter(TIM2);

    return;
}

void RestartTIM2( void )
{
    LL_TIM_SetCounter(TIM2, 0);

    return;
}

uint32_t GetTIM2Cnt( void )
{
    return LL_TIM_GetCounter(TIM2);
}

// Returns raw thermistor ADC value
int16_t GetThermistorValue( void )
{
    return LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_1);
}

// Returns raw current ADC value
int16_t GetCurrentValue( void )
{
    return LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_2);
}

// Returns raw voltage ADC value
int16_t GetVoltageValue( void )
{
    return LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_3);
}

void WriteMem( const uint32_t address, const char* const string, const uint32_t bytes )
{
    // TODO
    // look at yokohama code for writing fram
    (void) address;
    (void) string;
    (void) bytes;
    for (uint32_t i = 0; i < bytes; i++)
    {
        LL_SPI_TransmitData8(SPI1, string[i]);
    }
}
void ReadMem( const uint32_t address, char* string, const uint32_t bytes )
{
    // TODO
    // look at yokohama code for reading fram
    (void) address;
    (void) string;
    (void) bytes;
    for (uint32_t i = 0; i < bytes; i++)
    {
        string[i] = LL_SPI_ReceiveData8(SPI1);
    }
}

void sendUARTChar(char c)
{
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        while(!LL_USART_IsActiveFlag_TXE(USART2)){};
        LL_USART_TransmitData8(USART2, (uint8_t)(c & 0xFFU));
        while(LL_USART_IsActiveFlag_TC(USART2)){}
        return;
    #endif /* ENABLE_UART_DEBUGGING */
}

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