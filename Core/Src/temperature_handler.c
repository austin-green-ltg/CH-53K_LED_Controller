// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: temperature_handler.c
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
#include <string.h>

#include "temperature_handler.h"
#include "stm32l412xx-bsp.h"
#include "logger.h"

const int32_t  ThermistorTodCelcius = (1);
const int32_t  dCelciusToThermistor = (1);

// Temperature Thresholds (dC)
const int32_t   HeatingThreshold1_dC = (1000);
const int32_t   HeatingThreshold2_dC = (1200);
const int32_t   CoolingThreshold1_dC = (800) ;
const int32_t   CoolingThreshold2_dC = (1000);

// Normal Temperature is Cool
static TemperatureRange_e temperature_threshold = TempCool;

// Logs temperature change to storage
static void LogTempChange(TemperatureRange_e temp1, TemperatureRange_e temp2)
{
    char str[20];
    switch(temp1)
    {
        case TempCool:
            strcpy(str, "Temp Cool");
            break;

        case TempWarm:
            strcpy(str, "Temp Warm");
            break;

        case TempHot:
            strcpy(str, "Temp Hot");
            break;

        default:
            break;

    }

    strcat(str, "->");

    switch(temp2)
    {
        case TempCool:
            strcat(str, "Cool");
            break;

        case TempWarm:
            strcat(str, "Warm");
            break;

        case TempHot:
            strcat(str, "Hot");
            break;

        default:
            break;

    }

    strcat(str, "\n");
    LogString(str, 0);

}

// Get temperature from thermistor
int32_t GetTemperature( void )
{
    int32_t temperature = GetThermistorValue() * ThermistorTodCelcius;
    return (temperature);
}

/****
    * Get range that the temperature falls into.
    * There is an increased threshold to fall back
    * into the previous state.
    * Possible ranges are
    *   Cool    - Normal Operating Temperature
    *   Warm    - Temperature is elevated, decrease brightness
    *   Hot     - Temperature is hot, lower brightness significantly
    */
TemperatureRange_e GetTemperatureRange( void )
{
    int32_t temperature = GetTemperature();

    switch(temperature_threshold)
    {
        case TempCool:
            // no cooling check needed
            if (temperature >= HeatingThreshold2_dC)
            {
                temperature_threshold = TempHot;
                LogTempChange(TempCool, temperature_threshold);
            }
            else if (temperature >= HeatingThreshold1_dC)
            {
                temperature_threshold = TempWarm;
                LogTempChange(TempCool, temperature_threshold);
            }
            break;

        case TempWarm:
            // check if cooled down or heated up
            if (temperature <= CoolingThreshold1_dC)
            {
                temperature_threshold = TempCool;
                LogTempChange(TempWarm, temperature_threshold);
            }
            else if (temperature >= HeatingThreshold2_dC)
            {
                temperature_threshold = TempHot;
                LogTempChange(TempWarm, temperature_threshold);
            }
            break;

        case TempHot:
            // check if cooled down
            if (temperature <= CoolingThreshold1_dC)
            {
                temperature_threshold = TempCool;
                LogTempChange(TempHot, temperature_threshold);
            }
            else if (temperature <= CoolingThreshold2_dC)
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
