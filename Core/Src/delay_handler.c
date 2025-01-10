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
 * @file delay_handler.c
 *
 * @brief Handles system counters and delays
 *
 * Revision History:
 * Date       - Name         -  Ver -  Remarks
 * 07/31/2024 - Austin Green -  1.0 -  Initial Document
 * 09/10/2024 - Austin Green -  2.0 -  Doxyfile documentation
 *
 * Notes: Depends on the board support package bsp
 *
 *****************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include "delay_handler.h"
#include "stm32l412xx-bsp.h"

/**
  * Timer clock used to check the delay
  * values in stm32l412xx-bsp.h
  */
const float Tim2ClkKhz = ( CLK_FREQ_HZ / ( float ) TIM2_CLK_DEV /
                           ( float ) AHB_CLK_PRESCALER / ( float ) APB1_CLK_PRESCALER /
                           ( float ) TIM2_CLK_PRESCALER / 1000.0f ); // 10 / ms

const float Tim6ClkKhz = ( CLK_FREQ_HZ / ( float ) TIM6_CLK_DEV /
                           ( float ) AHB_CLK_PRESCALER / ( float ) APB1_CLK_PRESCALER /
                           ( float ) TIM6_CLK_PRESCALER / 1000.0f ); // ~1.22 / ms

const float Tim16ClkKhz = ( CLK_FREQ_HZ / ( float ) TIM16_CLK_DEV /
                            ( float ) AHB_CLK_PRESCALER / ( float ) APB1_CLK_PRESCALER /
                            ( float ) TIM16_CLK_PRESCALER / 1000.0f ); // ~1.22 / ms

/**
  * @brief Starts the delay counter, only needs to be called once on init
  */
void StartDelayCounter ( void )
{
    StartTIM2();
    RestartDelayCounter();
    return;
}

/**
  * @brief Starts the log delay counter, only needs to be called once on init
  */
void StartLogDelayCounter ( void )
{
    StartTIM6();
    RestartLogDelayCounter();
    return;
}

/**
  * @brief Starts the live log delay counter, only needs to be called once on init
  */
void StartLiveLogDelayCounter ( void )
{
    StartTIM16();
    RestartLiveLogDelayCounter();
    return;
}

/**
  * @brief Restart the delay counter
  */
void RestartDelayCounter ( void )
{
    RestartTIM2();
    return;
}

/**
  * @brief Restart the log delay counter
  */
void RestartLogDelayCounter ( void )
{
    RestartTIM6();
    return;
}

/**
  * @brief Restart the live log delay counter
  */
void RestartLiveLogDelayCounter ( void )
{
    RestartTIM16();
    return;
}

/**
  * @brief Checks if the delay (ms) was hit based on timer value
  * @param[in] delay_ms Time in ms to check if timer has hit
  * @param[out] Returns 1 if delay has been hit
  */
uint8_t DelayHit ( uint16_t delay_ms )
{
    uint8_t isDelayHit = ( GetTIM2Cnt() >= ( uint32_t ) ( delay_ms * Tim2ClkKhz +
                           0.5f ) );
    return isDelayHit;
}

/**
  * @brief Checks if the log delay (ms) was hit based on timer value
  * @param[in] delay_ms Time in ms to check if log timer has hit
  * @param[out] Returns 1 if log delay has been hit
  */
uint8_t LogDelayHit ( uint16_t delay_ms )
{
    uint8_t isDelayHit = ( GetTIM6Cnt() >= ( uint32_t ) ( delay_ms * Tim6ClkKhz +
                           0.5f ) );
    return isDelayHit;
}

/**
  * @brief Checks if the live log delay (ms) was hit based on timer value
  * @param[in] delay_ms Time in ms to check if live log timer has hit
  * @param[out] Returns 1 if live log delay has been hit
  */
uint8_t LiveLogDelayHit ( uint16_t delay_ms )
{
    uint8_t isDelayHit = ( GetTIM16Cnt() >= ( uint32_t ) ( delay_ms * Tim16ClkKhz +
                           0.5f ) );
    return isDelayHit;
}

/**
  * @brief Returns a delay value for a brightness level
  * @param[in] brightness Current brightness level
  * @param[out] Returns delay to satisfy specs at current brightness level
  */
uint16_t BrightnessDelay ( int8_t brightness )
{
    /* formula to satisfy requirements */
    return ( brightness * 5 + 250 );
}