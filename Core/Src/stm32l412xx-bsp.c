/*****************************************************************************
 *
 *  @attention
 *  Copyright (c) 2022, 2023 Luminator, An LTG Company
 *  All rights reserved.
 *  Any use without the prior written consent of Luminator,
 *  An LTG Company is strictly prohibited.
 *
 *****************************************************************************
 *****************************************************************************
 *
 * @file bsp.c
 *
 * @brief Board Support Package for STM32L412xx
 *
 * Revision History:
 * Date       - Name         -  Ver -  Remarks
 * 07/31/2024 - Austin Green -  1.0 -  Initial Document
 * 09/10/2024 - Austin Green -  2.0 -  Doxyfile documentation
 *
 * Notes: This uses the Low Level ST API to access the board pins
 *
 *****************************************************************************/

#include "stm32l412xx-bsp.h"

/**
  * @brief Reads on/off pin value
  * @param[out] On/Off pin state
  */
GPIO_PinState ReadOnOffPin ( void )
{
    return ( LL_GPIO_IsInputPinSet ( LED_ON_OFF_GPIO_Port,
                                     LED_ON_OFF_Pin ) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET );
}

/**
  * @brief Reads toggle pin value
  * @param[out] Toggle pin state
  */
GPIO_PinState ReadTogglePin ( void )
{
    return ( LL_GPIO_IsInputPinSet ( VIS_IR_GPIO_Port,
                                     VIS_IR_Pin ) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET );
}

/**
  * @brief Reads dim pin value
  * @param[out] Dim pin state
  */
GPIO_PinState ReadDimPin ( void )
{
    return ( LL_GPIO_IsInputPinSet ( DIM_GPIO_Port,
                                     DIM_Pin ) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET );
}

/**
  * @brief Reads bright pin value
  * @param[out] Bright pin state
  */
GPIO_PinState ReadBrightPin ( void )
{
    return ( LL_GPIO_IsInputPinSet ( BRIGHT_GPIO_Port,
                                     BRIGHT_Pin ) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET );
}

/**
  * @brief Enables Timer 1
  */
void EnablePWM1 ( void )
{
    LL_TIM_EnableAllOutputs ( TIM1 );

    LL_TIM_EnableCounter ( TIM1 );
}

/**
  * @brief Disables Timer 1
  */
void DisablePWM1 ( void )
{
    LL_TIM_DisableAllOutputs ( TIM1 );

    LL_TIM_DisableCounter ( TIM1 );
}

/**
  * @brief Starts PWM Timer 1 Channel 1 output
  */
void StartPWM11 ( void )
{
    LL_TIM_CC_EnableChannel ( TIM1, LL_TIM_CHANNEL_CH1 );
}

/**
  * @brief Starts PWM Timer 1 Channel 2 output
  */
void StartPWM12 ( void )
{
    LL_TIM_CC_EnableChannel ( TIM1, LL_TIM_CHANNEL_CH2 );
}

/**
  * @brief Stops PWM Timer 1 Channel 1 output
  */
void StopPWM11 ( void )
{
    LL_TIM_CC_DisableChannel ( TIM1, LL_TIM_CHANNEL_CH1 );
}

/**
  * @brief Stops PWM Timer 1 Channel 2 output
  */
void StopPWM12 ( void )
{
    LL_TIM_CC_DisableChannel ( TIM1, LL_TIM_CHANNEL_CH2 );
}

/**
  * @brief Sets PWM Timer 1 Channel 1 value
  * @param[in] pulse_width Value out of 255 to set pulse width to
  */
void SetPW11 ( uint32_t pulse_width )
{
    LL_TIM_OC_SetCompareCH1 ( TIM1, pulse_width );
}

/**
  * @brief Sets PWM Timer 1 Channel 2 value
  * @param[in] pulse_width Value out of 255 to set pulse width to
  */
void SetPW12 ( uint32_t pulse_width )
{
    LL_TIM_OC_SetCompareCH2 ( TIM1, pulse_width );
}

/**
  * @brief Starts Timer 2 counter
  */
void StartTIM2 ( void )
{
    LL_TIM_EnableCounter ( TIM2 );

    return;
}

/**
  * @brief Resets Timer 2 counter to zero
  */
void RestartTIM2 ( void )
{
    LL_TIM_SetCounter ( TIM2, 0 );

    return;
}

/**
  * @brief Returns value in the Timer 2 counter
  * @param[out] Value of Timer 2 counter
  */
uint32_t GetTIM2Cnt ( void )
{
    return LL_TIM_GetCounter ( TIM2 );
}

/**
  * @brief Starts Timer 6 counter
  */
void StartTIM6 ( void )
{
    LL_TIM_EnableCounter ( TIM6 );

    return;
}

/**
  * @brief Resets Timer 6 counter to zero
  */
void RestartTIM6 ( void )
{
    LL_TIM_SetCounter ( TIM6, 0 );

    return;
}

/**
  * @brief Returns value in the Timer 6 counter
  * @param[out] Value of Timer 6 counter
  */
uint32_t GetTIM6Cnt ( void )
{
    return LL_TIM_GetCounter ( TIM6 );
}

/**
  * @brief Returns raw ADC value 0x0 to 0xFFF from thermistor.
  *         0x0 = 0V, 0xFFF = 3.3V
  * @param[out] Thermistor raw ADC value
  */
int16_t GetThermistorValue ( void )
{
    return LL_ADC_INJ_ReadConversionData12 ( ADC1, LL_ADC_INJ_RANK_1 );
}

/**
  * @brief Returns raw ADC value from ammeter
  * @param[out] Ammeter raw ADC value
  */
int16_t GetCurrentValue ( void )
{
    return LL_ADC_INJ_ReadConversionData12 ( ADC1, LL_ADC_INJ_RANK_2 );
}

/**
  * @brief Returns raw ADC value from voltmeter
  * @param[out] Voltmeter raw ADC value
  */
int16_t GetVoltageValue ( void )
{
    return LL_ADC_INJ_ReadConversionData12 ( ADC1, LL_ADC_INJ_RANK_3 );
}

/**
  * @brief Enables SPI write protect line (active high)
  */
void enableWriteProtect ( void )
{
    LL_GPIO_SetOutputPin ( SPI_WP_GPIO_Port, SPI_WP_Pin );
}

/**
  * @brief Disables SPI write protect line (active high)
  */
void disableWriteProtect ( void )
{
    LL_GPIO_ResetOutputPin ( SPI_WP_GPIO_Port, SPI_WP_Pin );
}

/**
  * @brief Enables SPI chip select line (active low)
  */
void enableChipSelect ( void )
{
    LL_GPIO_SetOutputPin ( SPI_NSS_GPIO_Port, SPI_NSS_Pin );
}

/**
  * @brief Disables SPI chip select line (active low)
  */
void disableChipSelect ( void )
{
    LL_GPIO_ResetOutputPin ( SPI_NSS_GPIO_Port, SPI_NSS_Pin );
}

static void nop ( uint32_t num )
{
    volatile uint32_t i;

    for ( i = 0; i < num; i++ )
    {
    }
}

/**
  * @brief Sends data via SPI lines
  * @param[in] txData Pointer to data to send
  * @param[in] bytes  Number of bytes to send
  */
void transferData ( const unsigned char* const txData, const uint32_t bytes )
{
    /* Set the transaction information */
    const uint8_t* pTxBuffPtr = ( const uint8_t* ) txData;
    uint32_t TxXferCount = bytes;

    /* Check if the SPI is already enabled */
    if (!LL_SPI_IsEnabled ( SPI1 ) )
    {
        /* Enable SPI peripheral */
        LL_SPI_Enable ( SPI1 );
    }

    /* Transmit data in 8 Bit mode */
    while ( TxXferCount > 0U )
    {
        /* Wait until TXE flag is set to send data */
        if ( LL_SPI_IsActiveFlag_TXE ( SPI1 ) )
        {
            LL_SPI_TransmitData8 ( SPI1, ( uint8_t ) (* ( pTxBuffPtr ) ) );
            pTxBuffPtr++;
            TxXferCount--;
        }
    }

    /* Control if the TX fifo is empty */
    while ( LL_SPI_GetTxFIFOLevel ( SPI1 ) != LL_SPI_TX_FIFO_EMPTY )
    {
        nop ( 100 );
    }

    /* Control the BSY flag */
    while ( LL_SPI_IsActiveFlag_BSY ( SPI1 ) )
    {
        nop ( 100 );
    }

    /* Control if the RX fifo is empty */
    volatile uint8_t tmpreg8 = 0;

    while ( LL_SPI_GetRxFIFOLevel ( SPI1 ) != LL_SPI_RX_FIFO_EMPTY )
    {
        nop ( 100 );
        tmpreg8 = LL_SPI_ReceiveData8 ( SPI1 );
        ( void ) tmpreg8;
    }

    LL_SPI_ClearFlag_OVR ( SPI1 );

    // while ( xferBusy == 1 )
    // {
    // nop ( 100 );
    // }
    //
    // xferBusy = 1;
    //
    // if (!LL_SPI_IsEnabled ( SPI1 ) )
    // {
    // LL_SPI_Enable ( SPI1 );
    // }
    // uint32_t xferCnt = 0;
    // while ( xferCnt < bytes )
    // {
    // /* Wait until TXE flag is set to send data */
    // if ( LL_SPI_IsActiveFlag_TXE ( SPI1 ) )
    // {
    // LL_SPI_TransmitData8 ( SPI1, txData [ xferCnt ] );
    // xferCnt++;
    // }
    // }

    ////    while ( LL_SPI_GetTxFIFOLevel ( SPI1 ) != LL_SPI_TX_FIFO_EMPTY )
    ////    {
    ////        nop ( 100 );
    ////    }
    ////
    ////     while ( !LL_SPI_IsActiveFlag_BSY ( SPI1 ) )
    ////     {
    ////        nop(100);
    ////     }
    //
    // while ( LL_SPI_GetTxFIFOLevel ( SPI1 ) != LL_SPI_TX_FIFO_EMPTY )
    // {
    // nop(100);
    // }

    // LL_SPI_ClearFlag_OVR ( SPI1 );
    //
    // xferBusy = 0;
}

/**
  * @brief Gets data from SPI lines
  * @param[in] rxData Pointer to data buffer
  * @param[in] bytes  Number of bytes to receive
  */
void receiveData ( unsigned char* rxData, const uint32_t bytes )
{
    // use transmitReceiveData with dummy transmit data
    transmitReceiveData ( rxData, rxData, bytes );
}

/****
    * @brief Sends and gets data from SPI lines
    * @param[in] txData Pointer to data to send
    * @param[in] rxData Pointer to data buffer
    * @param[in] bytes  Number of bytes to send and receive
    */
void transmitReceiveData ( const unsigned char* const txData,
                           unsigned char* rxData, const uint32_t bytes )
{
    /* Variable used to alternate Rx and Tx during transfer */
    uint32_t txallowed = 1U;

    /* Set the transaction information */
    uint8_t* pRxBuffPtr = ( uint8_t* ) rxData;
    uint32_t RxXferCount = bytes;
    const uint8_t* pTxBuffPtr = ( const uint8_t* ) txData;
    uint32_t TxXferCount = bytes;

    LL_SPI_SetRxFIFOThreshold ( SPI1, LL_SPI_RX_FIFO_TH_QUARTER );

    /* Check if the SPI is already enabled */
    if (!LL_SPI_IsEnabled ( SPI1 ) )
    {
        /* Enable SPI peripheral */
        LL_SPI_Enable ( SPI1 );
    }

    /* Transmit and Receive data in 8 Bit mode */
    {
        if ( TxXferCount == 0x01U )
        {
            LL_SPI_TransmitData8 ( SPI1, ( uint8_t ) (* ( pTxBuffPtr ) ) );
            pTxBuffPtr++;
            TxXferCount--;

        }

        while ( ( TxXferCount > 0U ) || ( RxXferCount > 0U ) )
        {
            /* Check TXE flag */
            if ( ( LL_SPI_IsActiveFlag_TXE ( SPI1 ) ) &&
                    ( TxXferCount > 0U ) && ( txallowed == 1U ) )
            {

                LL_SPI_TransmitData8 ( SPI1, ( uint8_t ) (* ( pTxBuffPtr ) ) );
                pTxBuffPtr++;
                TxXferCount--;

                /* Next Data is a reception (Rx). Tx not allowed */
                txallowed = 0U;

            }

            /* Wait until RXNE flag is reset */
            if ( ( LL_SPI_IsActiveFlag_RXNE ( SPI1 ) ) &&
                    ( RxXferCount > 0U ) )
            {
                (* ( uint8_t* ) pRxBuffPtr ) = LL_SPI_ReceiveData8 ( SPI1 );
                pRxBuffPtr++;
                RxXferCount--;

                /* Next Data is a Transmission (Tx). Tx is allowed */
                txallowed = 1U;
            }
        }
    }

    /* Control if the TX fifo is empty */
    while ( LL_SPI_GetTxFIFOLevel ( SPI1 ) != LL_SPI_TX_FIFO_EMPTY )
    {
        nop ( 100 );
    }

    /* Control the BSY flag */
    while ( LL_SPI_IsActiveFlag_BSY ( SPI1 ) )
    {
        nop ( 100 );
    }

    /* Control if the RX fifo is empty */
    volatile uint8_t tmpreg8 = 0;

    while ( LL_SPI_GetRxFIFOLevel ( SPI1 ) != LL_SPI_RX_FIFO_EMPTY )
    {
        nop ( 100 );
        tmpreg8 = LL_SPI_ReceiveData8 ( SPI1 );
        ( void ) tmpreg8;
    }
}

/**
  * @brief Sends character via UART line
  * @param[in] c Character to send via UART
  */
void sendUARTChar ( char c )
{
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */

    while (!LL_USART_IsActiveFlag_TXE ( USART2 ) ) {};

    LL_USART_TransmitData8 ( USART2, ( uint8_t ) ( c & 0xFFU ) );

    while ( LL_USART_IsActiveFlag_TC ( USART2 ) ) {}

    return;
#endif /* ENABLE_UART_DEBUGGING */
}
