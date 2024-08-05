// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: thermistor_handler.c
//
// Description: Handles getting this temperature and transitioning
//                between temperature states.
//
// Revision History:
// Date       - Name         -  Ver -  Remarks
// 07/31/2024 - Austin Green -  1.0 -  Initial Document
// 08/05/2024 - Austin Green -  1.1 -  Refactor to not use floats
//
// Notes: Depends on the board support package bsp
//        logger is used to log temperature transitions
//
// ***************************************************************************

/* Private includes ----------------------------------------------------------*/
#include "thermistor_handler.h"
#include "bsp.h"
#include "logger.h"

const int32_t  ThermistorTomCelcius = (1);
const int32_t  mCelciusToThermistor = (1);

const int32_t   HeatingThreshold1   = (100000);
const int32_t   HeatingThreshold2   = (120000);
const int32_t   CoolingThreshold1   = (80000) ;
const int32_t   CoolingThreshold2   = (100000);

static TemperatureRange_e temperature_threshold = TempCool;

static void LogTempChange(TemperatureRange_e temp1, TemperatureRange_e temp2)
{
    switch(temp1)
    {
        case TempCool:
            LogString("Temp Cool", 0);
            break;

        case TempWarm:
            LogString("Temp Warm", 0);
            break;

        case TempHot:
            LogString("Temp Hot", 0);
            break;

        default:
            break;

    }

    LogString("->", 0);

    switch(temp2)
    {
    case TempCool:
        LogString("Cool", 0);
        break;

    case TempWarm:
        LogString("Warm", 0);
        break;

    case TempHot:
        LogString("Hot", 0);
        break;

    default:
        break;

    }

    LogString("\n", 0);

}

int32_t GetTemperature( void )
{
    int32_t temperature = GetThermistorValue() * ThermistorTomCelcius;
    return (temperature);
}

TemperatureRange_e GetTemperatureRange( void )
{
    int32_t temperature = GetTemperature();

    switch(temperature_threshold)
    {
        case TempCool:
            // no cooling check needed
            if (temperature >= HeatingThreshold2)
            {
                temperature_threshold = TempHot;
                LogTempChange(TempCool, temperature_threshold);
            }
            else if (temperature >= HeatingThreshold1)
            {
                temperature_threshold = TempWarm;
                LogTempChange(TempCool, temperature_threshold);
            }
            break;

        case TempWarm:
            // check if cooled down or heated up
            if (temperature <= CoolingThreshold1)
            {
                temperature_threshold = TempCool;
                LogTempChange(TempWarm, temperature_threshold);
            }
            else if (temperature >= HeatingThreshold2)
            {
                temperature_threshold = TempHot;
                LogTempChange(TempWarm, temperature_threshold);
            }
            break;

        case TempHot:
            // check if cooled down
            if (temperature <= CoolingThreshold1)
            {
                temperature_threshold = TempCool;
                LogTempChange(TempHot, temperature_threshold);
            }
            else if (temperature <= CoolingThreshold2)
            {
                temperature_threshold = TempWarm;
                LogTempChange(TempHot, temperature_threshold);
            }
            break;

        default:
            break;
    }

    return temperature_threshold;

}