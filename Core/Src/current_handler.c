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
#include <stdio.h>

#include "current_handler.h"
#include "stm32l412xx-bsp.h"
#include "logger.h"

const uint16_t  RawTodAmps = (1);
const uint16_t  dAmpsToRaw = (1);

// Current Thresholds (dA)
const uint16_t CurrentHighThreshold_dA  = 35u;
const uint16_t CurrentErrorThreshold_dA = 40u;

// Default Current is Normal
static CurrentRange_e current_threshold = CurrentNormal;

// Logs current change to storage
static void LogCurrentChange(CurrentRange_e range, uint16_t currentValue)
{
    char str[25];

    switch(range)
    {
        case CurrentNormal:
            sprintf(str, "Normal Current %d dA\n", currentValue);
            break;

        case CurrentHigh:
            sprintf(str, "High Current %d dA\n", currentValue);
            break;

        case CurrentError:
            sprintf(str, "Error Current %d dA\n", currentValue);
            break;

        default:
            break;

    }

    LogString(str, 0);
}

// Get voltage from ampmeter
uint16_t GetCurrent( void )
{
    uint16_t current = GetCurrentValue() * RawTodAmps;
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

    if (current >= CurrentErrorThreshold_dA)
    {
        current_threshold = CurrentError;
    }
    else if (current >= CurrentHighThreshold_dA)
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
