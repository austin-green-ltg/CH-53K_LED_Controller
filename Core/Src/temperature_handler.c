/*****************************************************************************
 *
 *  @attention
 * Copyright © 2007 Luminator Mark IV
 * All rights reserved.
 * Any use without the prior written consent of Luminator Mark IV
 * is strictly prohibited.
 *
 *****************************************************************************
 *****************************************************************************
 *
 * @file temperature_handler.c
 *
 * @brief Handles getting this temperature and transitioning between
 *          temperature states.
 *
 * Revision History:
 * Date       - Name         -  Ver -  Remarks
 * 07/31/2024 - Austin Green -  1.0 -  Initial Document
 * 08/05/2024 - Austin Green -  1.1 -  Refactor to not use floats
 * 09/10/2024 - Austin Green -  2.0 -  Doxyfile documentation
 *
 * Notes: Depends on the board support package bsp
 *        logger is used to log temperature transitions
 *
 *****************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include <string.h>

#include "temperature_handler.h"
#include "stm32l412xx-bsp.h"
#include "logger.h"

/** Raw value out of thermistor to deciCelcius (C*0.1) */
const int32_t  ThermistorTodCelcius = (1);
/** DeciCelcius (V*0.1) to raw value out of thermistor */
const int32_t  dCelciusToThermistor = (1);

/** Heating Threshold 1 (heating up from Cool to Warm) in dC */
const int32_t   HeatingThreshold1_dC = (1000);
/** Heating Threshold 2 (heating up from Warm to Hot) in dC */
const int32_t   HeatingThreshold2_dC = (1200);
/** Cooling Threshold 1 (cooling down from Warm to Cool) in dC */
const int32_t   CoolingThreshold1_dC = (800) ;
/** Cooling Threshold 2 (cooling down from Hot to Warm) in dC */
const int32_t   CoolingThreshold2_dC = (1000);

/** default temperature state is TempCool */
static TemperatureRange_e temperature_threshold = TempCool;

/**
  * @brief Logs temperature change to storage
  *
  * @param[in] temp1 Original range the temperature was in, see TemperatureRange_e
  * @param[in] temp2 New range the temperature is in, see TemperatureRange_e
  */
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

/**
  * @brief Get temperature from thermistor
  *
  * @param[out] temperature level in dC
  */
int32_t GetTemperature( void )
{
    int32_t temperature = GetThermistorValue() * ThermistorTodCelcius;
    return (temperature);
}

/**
  * @brief Get range that the temperature falls into. There is an increased
  *         threshold to fall back into the previous state.
  *         Possible ranges are
  *           Cool    - Normal Operating Temperature
  *           Warm    - Temperature is elevated, decrease brightness
  *           Hot     - Temperature is hot, lower brightness significantly
  * @param[out] Current temperature range
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
