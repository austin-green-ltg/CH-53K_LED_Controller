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
#include <stdio.h>
#include <string.h>

#include "temperature_handler.h"
#include "stm32l412xx-bsp.h"
#include "logger.h"

/** Heating Threshold 1 (heating up from Cool to Warm) in dC */
const int16_t HeatingThreshold1_dC = ( 1000 );
/** Heating Threshold 2 (heating up from Warm to Hot) in dC */
const int16_t HeatingThreshold2_dC = ( 1200 );
/** Cooling Threshold 1 (cooling down from Warm to Cool) in dC */
const int16_t CoolingThreshold1_dC = ( 800 ) ;
/** Cooling Threshold 2 (cooling down from Hot to Warm) in dC */
const int16_t CoolingThreshold2_dC = ( 1000 );

/**
  * Temperature Thermistor to dC Conversion Constant
  * Multiply by below then subtract 500 to go from thermistor to dC value
  * Make sure you add 0.5 to round int
  */
const float thermistor_to_dC = RAW_TO_MV;

/**
  * Temperature dC to Thermistor Conversion Constant
  * Add 500 and then multiply by below to go from dC to thermistor value
  * Make sure you add 0.5 to round int
  */
const float dC_to_thermistor = MV_TO_RAW;

/** default temperature state is TempCool */
static TemperatureRange_e temperature_threshold = TempCool;

static uint32_t numTemperatureLogs = 0;

/**
  * @brief Logs temperature change to storage
  *
  * @param[in] temp1 Original range the temperature was in, see TemperatureRange_e
  * @param[in] temp2 New range the temperature is in, see TemperatureRange_e
  */
static void LogTempChange ( TemperatureRange_e temp1, TemperatureRange_e temp2 )
{
    char str [ 20 ];
    memset ( str, '\0', 20 );

    switch ( temp1 )
    {
        case TempCool:
            strncpy ( str, "Temp Cool", sizeof ( str ) - 1 );
            break;

        case TempWarm:
            strncpy ( str, "Temp Warm", sizeof ( str ) - 1 );
            break;

        case TempHot:
            strncpy ( str, "Temp Hot", sizeof ( str ) - 1 );
            break;

        default:
            break;

    }

    str [ 19 ] = '\0';
    strncat ( str, "->", sizeof ( str ) - 1 );

    switch ( temp2 )
    {
        case TempCool:
            strncat ( str, "Cool", sizeof ( str ) - 1 );
            break;

        case TempWarm:
            strncat ( str, "Warm", sizeof ( str ) - 1 );
            break;

        case TempHot:
            strncat ( str, "Hot", sizeof ( str ) - 1 );
            break;

        default:
            break;

    }

    strncat ( str, "\n", sizeof ( str ) - 1 );
    LogString ( str, 0 );

}

/**
  * @brief Logs temperature to storage
  */
void LogTemperature ( void )
{
    char str [ TEMPERATURE_LOG_SIZE ];
    int16_t temperature = GetTemperature();

    numToCharArray ( str, temperature );
    str [ 2 ] = '\0';

    if ( numTemperatureLogs * TEMPERATURE_LOG_SIZE >= TEMPERATURE_LOG_SPACE )
    {
        numTemperatureLogs = 0;
    }

    WriteLog ( STARTING_TEMPERATURE_ADDRESS + numTemperatureLogs *
               TEMPERATURE_LOG_SIZE, str,
               TEMPERATURE_LOG_SIZE );

    numTemperatureLogs++;
}

/**
  * @brief Get temperature from thermistor
  *         Conversion is (thermistor * 3300 / 0xFFF ~ 0.806) = millivolts
  *         Millivolts / 10 - 50 = Temperature C
  *         We multiply everything by 10 to get dC
  *         To reverse (dC + 500) * 4095 / 3300 ~ 1.241
  *
  * @param[out] temperature level in dC
  */
int16_t GetTemperature ( void )
{
    int16_t temperature = ( int16_t ) ( ( float ) GetThermistorValue() *
                                        thermistor_to_dC + 0.5f ) + (-500 );
    return ( temperature );
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
TemperatureRange_e GetTemperatureRange ( void )
{
    int32_t temperature = GetTemperature();

    switch ( temperature_threshold )
    {
        case TempCool:

            // no cooling check needed
            if ( temperature >= HeatingThreshold2_dC )
            {
                temperature_threshold = TempHot;
                LogTempChange ( TempCool, temperature_threshold );
            }
            else if ( temperature >= HeatingThreshold1_dC )
            {
                temperature_threshold = TempWarm;
                LogTempChange ( TempCool, temperature_threshold );
            }

            break;

        case TempWarm:

            // check if cooled down or heated up
            if ( temperature <= CoolingThreshold1_dC )
            {
                temperature_threshold = TempCool;
                LogTempChange ( TempWarm, temperature_threshold );
            }
            else if ( temperature >= HeatingThreshold2_dC )
            {
                temperature_threshold = TempHot;
                LogTempChange ( TempWarm, temperature_threshold );
            }

            break;

        case TempHot:

            // check if cooled down
            if ( temperature <= CoolingThreshold1_dC )
            {
                temperature_threshold = TempCool;
                LogTempChange ( TempHot, temperature_threshold );
            }
            else if ( temperature <= CoolingThreshold2_dC )
            {
                temperature_threshold = TempWarm;
                LogTempChange ( TempHot, temperature_threshold );
            }

            break;

        default:
            break;
    }

    return temperature_threshold;

}
