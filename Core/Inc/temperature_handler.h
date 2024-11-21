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
 * @file temperature_handler.h
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
 * Notes:
 *
 *****************************************************************************/

#ifndef INC_temperature_handlerh
#define INC_temperature_handlerh

#include <stdint.h>

/** Temperature Range Enum */
typedef enum
{
    TempCool = 0, /**< Normal Operating Temperature                       */
    TempWarm = 1, /**< Temperature is elevated, decrease brightness       */
    TempHot = 2 /**< Temperature is hot, lower brightness significantly */
} TemperatureRange_e;

/**
  * @brief Get temperature from thermistor
  *
  * @param[out] temperature level in dC
  */
int32_t GetTemperature ( void );

/**
  * @brief Get range that the temperature falls into. There is an increased
  *         threshold to fall back into the previous state.
  *         Possible ranges are
  *           Cool    - Normal Operating Temperature
  *           Warm    - Temperature is elevated, decrease brightness
  *           Hot     - Temperature is hot, lower brightness significantly
  * @param[out] Current temperature range
  */
TemperatureRange_e GetTemperatureRange ( void );

/**
  * @brief Logs the Temperature. Always takes up 16 bits
  */
void LogTemperature ( void );

#endif /* INC_temperature_handlerh */
