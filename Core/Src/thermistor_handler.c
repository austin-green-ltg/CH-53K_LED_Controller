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
//
// Notes: Depends on the board support package bsp
//        logger is used to log temperature transitions
//
// ***************************************************************************

/* Private includes ----------------------------------------------------------*/
#include "thermistor_handler.h"
#include "bsp.h"
#include "logger.h"

const float     ThermistorToCelcius = (1.0f / 1000.0f);
const uint16_t  CelciusToThermistor = (1000.0f)       ;

const uint8_t   HeatingThreshold1   = (100);
const uint8_t   HeatingThreshold2   = (120);
const uint8_t   CoolingThreshold1   = (80) ;
const uint8_t   CoolingThreshold2   = (100);

static TemperatureRange_e temperature_threshold = Cool;

static void LogTempChange(TemperatureRange_e temp1, TemperatureRange_e temp2)
{
    switch(temp1)
    {
        case Cool:
            LogString("Cool", 0);
            break;

        case Warm:
            LogString("Warm", 0);
            break;

        case Hot:
            LogString("Hot", 0);
            break;

        default:
            break;

    }

    LogString("->", 0);

    switch(temp2)
    {
    case Cool:
        LogString("Cool", 0);
        break;

    case Warm:
        LogString("Warm", 0);
        break;

    case Hot:
        LogString("Hot", 0);
        break;

    default:
        break;

    }

    LogString("\n", 0);

}

int16_t GetTemperature( void )
{
    float temperature = GetThermistorValue() * ThermistorToCelcius;
    temperature += (temperature > 0 ? 0.5f : -0.5f);
    return (int16_t)(temperature);
}

TemperatureRange_e GetTemperatureRange( void )
{
    int16_t temperature = GetTemperature();

    if (temperature_threshold == Cool)
    {
        // no cooling check needed
        if (temperature >= HeatingThreshold2)
        {
            temperature_threshold = Hot;
            LogTempChange(Cool, Hot);
        }
        else if (temperature >= HeatingThreshold1)
        {
            temperature_threshold = Warm;
            LogTempChange(Cool, Warm);
        }
    }
    else if (temperature_threshold == Warm)
    {
        // check if cooled down or heated up
        if (temperature <= CoolingThreshold1)
        {
            temperature_threshold = Cool;
            LogTempChange(Warm, Cool);
        }
        else if (temperature >= HeatingThreshold2)
        {
            temperature_threshold = Hot;
            LogTempChange(Warm, Hot);
        }
    }
    else if (temperature_threshold == Hot)
    {
        // check if cooled down
        if (temperature <= CoolingThreshold1)
        {
            temperature_threshold = Cool;
            LogTempChange(Hot, Cool);
        }
        else if (temperature <= CoolingThreshold2)
        {
            temperature_threshold = Warm;
            LogTempChange(Hot, Warm);
        }
    }

    return temperature_threshold;

}