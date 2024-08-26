// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: delay_handler.c
//
// Description: Handles system counters and delays
//
// Revision History:
// Date       - Name         -  Ver -  Remarks
// 07/31/2024 - Austin Green -  1.0 -  Initial Document
//
// Notes: Depends on the board support package bsp
//
// ***************************************************************************

/* Private includes ----------------------------------------------------------*/
#include "delay_handler.h"
#include "stm32l412xx-bsp.h"

// Timer clock used to check the delay
/* values in stm32l412xx-bsp.h */
const float Tim2ClkKhz = (CLK_FREQ_HZ / (float)TIM2_CLK_DEV / (float)TIM2_CLK_PRESCALER / 1000.0f); // 1 / ms

// Starts the delay counter, only needs to be called once on init
void StartDelayCounter(void)
{
    StartTIM2();
    RestartDelayCounter();
    return;
}

// Restart the delay counter
void RestartDelayCounter(void)
{
    RestartTIM2();
    return;
}

// Checks if the delay (ms) was hit based on timer value
uint8_t DelayHit(uint32_t delay_ms)
{
    uint8_t isDelayHit = (GetTIM2Cnt() >= (uint32_t)(delay_ms * Tim2ClkKhz + 0.5f));
    return isDelayHit;
}

// Returns a delay value for a brightness level
uint16_t BrightnessDelay(int8_t brightness)
{
    /* formula to satisfy requirements */
    return (brightness * 5 + 250);
}