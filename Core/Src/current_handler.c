// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: current_handler.c
//
// Description: Handles getting current and reporting values.
//
// Revision History:
// Date       - Name         -  Ver -  Remarks
// 08/05/2024 - Austin Green -  1.0 -  Initial Document
//
// Notes: Depends on the board support package bsp
//        logger is used to log current transitions
//
// ***************************************************************************

/* Private includes ----------------------------------------------------------*/
#include "current_handler.h"
#include "stm32l412xx-bsp.h"
#include "logger.h"

const uint16_t  RawTomAmps = (1);
const uint16_t  mAmpsToRaw = (1);

// Current Thresholds (mA)
const uint16_t CurrentHighThreshold_mA  = 3500u;
const uint16_t CurrentErrorThreshold_mA = 4000u;

// Default Current is Normal
static CurrentRange_e current_threshold = CurrentNormal;

// Logs current change to storage
static void LogCurrentChange(CurrentRange_e range, uint16_t currentValue)
{
    switch(range)
    {
        case CurrentNormal:
            LogString("Normal Current ", 0);
            break;

        case CurrentHigh:
            LogString("High Current ", 0);
            break;

        case CurrentError:
            LogString("Error Current ", 0);
            break;

        default:
            break;

    }

    LogNumber(currentValue, 0);

    LogString(" mA\n", 0);

}

// Get voltage from ampmeter
uint16_t GetCurrent( void )
{
    uint16_t current = GetCurrentValue() * RawTomAmps;
    return (current);
}

/****
    * Get range that the current falls into
    * Possible ranges are
    *   Normal  - Normal Operating Current
    *   High    - Current high, but ok
    *   Error   - Current too high
    */
CurrentRange_e GetCurrentRange( void )
{
    uint16_t current = GetCurrent();
    CurrentRange_e prev_threshold = current_threshold;

    if (current >= CurrentErrorThreshold_mA)
    {
        current_threshold = CurrentError;
    }
    else if (current >= CurrentHighThreshold_mA)
    {
        current_threshold = CurrentHigh;
    }
    else
    {
        current_threshold = CurrentNormal;
    }

    if (prev_threshold != current_threshold) LogCurrentChange(current_threshold, current);

    return current_threshold;
}