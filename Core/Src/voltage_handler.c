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
 * @file voltage_handler.c
 *
 * @brief Handles getting voltage and reporting values.
 *
 * Revision History:
 * Date       - Name         -  Ver -  Remarks
 * 08/05/2024 - Austin Green -  1.0 -  Initial Document
 * 09/10/2024 - Austin Green -  2.0 -  Doxyfile documentation
 *
 * Notes: Depends on the board support package bsp
 *        logger is used to log voltage transitions
 *
 *****************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include <stdio.h>

#include "voltage_handler.h"
#include "stm32l412xx-bsp.h"
#include "logger.h"

/** Raw value out of voltmeter to deciVolts (V*0.1) */
const uint16_t  RawTodVolts = (1);
/** DeciCelcius (C*0.1) to raw value out of voltmeter */
const uint16_t  dVoltsToRaw = (1);

/** Low Voltage Error Level in dV */
const uint16_t VoltageErrorLowThreshold_dV  = 240u;
/** Low Voltage Level in dV */
const uint16_t VoltageLowThreshold_dV       = 260u;
/** High Voltage Level in dV */
const uint16_t VoltageHighThreshold_dV      = 300u;
/** High Voltage Error Level in dV */
const uint16_t VoltageErrorHighThreshold_dV = 320u;

/** default voltage state is VoltageNormal */
static VoltageRange_e voltage_threshold = VoltageNormal;

/**
  * @brief Logs voltage change to storage
  *
  * @param[in] range Range the voltage is in, see VoltageRange_e
  * @param[in] voltageValue Voltage in dV
  */
static void LogVoltageChange(VoltageRange_e range, uint16_t voltageValue)
{
    char str[30];

    switch(range)
    {
        case VoltageErrorLow:
            sprintf(str, "Error Low Voltage %d dV\n", voltageValue);
            break;

        case VoltageLow:
            sprintf(str, "Low Voltage %d dV\n", voltageValue);
            break;

        case VoltageNormal:
            sprintf(str, "Normal Voltage %d dV\n", voltageValue);
            break;

        case VoltageHigh:
            sprintf(str, "High Voltage %d dV\n", voltageValue);
            break;

        case VoltageErrorHigh:
            sprintf(str, "Error High Voltage %d dV\n", voltageValue);
            break;

        default:
            break;

    }

    LogString(str, 0);
}

/**
  * @brief Get voltage from voltmeter
  *
  * @param[out] voltage level in dV
  */
uint16_t GetVoltage( void )
{
    uint16_t voltage = GetVoltageValue() * RawTodVolts;
    return (voltage);
}

/**
  * @brief Get range that the voltage falls into
  *         Possible ranges are
  *         Normal      - Normal Operating Voltage
  *         Low         - Voltage low, but ok
  *         High        - Voltage high, but ok
  *         ErrorLow    - Voltage too low
  *         ErrorHigh   - Voltage too high
  * @param[out] Current voltage range
  */
VoltageRange_e GetVoltageRange( void )
{
    uint16_t voltage = GetVoltage();
    VoltageRange_e prev_threshold = voltage_threshold;

    if (voltage <= VoltageErrorLowThreshold_dV)
    {
        voltage_threshold = VoltageErrorLow;
    }
    else if (voltage <= VoltageLowThreshold_dV)
    {
        voltage_threshold = VoltageLow;
    }
    else if (voltage >= VoltageErrorHighThreshold_dV)
    {
        voltage_threshold = VoltageErrorHigh;
    }
    else if (voltage >= VoltageHighThreshold_dV)
    {
        voltage_threshold = VoltageHigh;
    }
    else
    {
        voltage_threshold = VoltageNormal;
    }

    if (prev_threshold != voltage_threshold) LogVoltageChange(voltage_threshold, voltage);

    return voltage_threshold;

}
