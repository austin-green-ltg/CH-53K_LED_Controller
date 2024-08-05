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
#include "bsp.h"
#include "logger.h"

const uint16_t  RawTomAmps = (1);
const uint16_t  mAmpsToRaw = (1);

const uint16_t CurrentHighThreshold_mA  = 3500u;
const uint16_t CurrentErrorThreshold_mA = 4000u;

static CurrentRange_e current_threshold = Normal;

static void LogCurrentChange(CurrentRange_e range, uint16_t currentValue)
{
    switch(range)
    {
        case Normal:
            LogString("Normal Current ", 0);
            break;

        case High:
            LogString("High Current ", 0);
            break;

        case Error:
            LogString("Error Current ", 0);
            break;

        default:
            break;

    }

    LogNumber(currentValue, 0);

    LogString(" mA\n", 0);

}

uint16_t GetCurrent_mA( void )
{
    uint16_t current = GetCurrentValue() * RawTomAmps;
    return (current);
}

CurrentRange_e GetCurrentRange( void )
{
    uint16_t current = GetCurrent_mA();

    if (current_threshold == Normal)
    {
        if (current >= CurrentErrorThreshold_mA)
        {
            current_threshold = Error;
            LogCurrentChange(Error, current);
        }
        else if (current >= CurrentHighThreshold_mA)
        {
            current_threshold = High;
            LogCurrentChange(High, current);
        }
    }
    else if (current_threshold == High)
    {
        if (current < CurrentHighThreshold_mA)
        {
            current_threshold = Normal;
            LogCurrentChange(Normal, current);
        }
        else if (current >= CurrentErrorThreshold_mA)
        {
            current_threshold = Error;
            LogCurrentChange(Error, current);
        }
    }
    else if (current_threshold == Error)
    {
        if (current < CurrentHighThreshold_mA)
        {
            current_threshold = Normal;
            LogCurrentChange(Normal, current);
        }
        else if (current < CurrentErrorThreshold_mA)
        {
            current_threshold = High;
            LogCurrentChange(High, current);
        }
    }

    return current_threshold;

}