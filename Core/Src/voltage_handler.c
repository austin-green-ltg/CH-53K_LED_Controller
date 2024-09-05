// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: voltage_handler.c
//
// Description: Handles getting voltage and reporting values.
//
// Revision History:
// Date       - Name         -  Ver -  Remarks
// 08/05/2024 - Austin Green -  1.0 -  Initial Document
//
// Notes: Depends on the board support package bsp
//        logger is used to log voltage transitions
//
// ***************************************************************************

/* Private includes ----------------------------------------------------------*/
#include "voltage_handler.h"
#include "stm32l412xx-bsp.h"
#include "logger.h"

const uint16_t  RawTodColts = (1);
const uint16_t  dColtsToRaw = (1);

// Voltage Thresholds (dV)
const uint16_t VoltageErrorLowThreshold_dC  = 240u;
const uint16_t VoltageLowThreshold_dC       = 260u;
const uint16_t VoltageHighThreshold_dC      = 300u;
const uint16_t VoltageErrorHighThreshold_dC = 320u;

// default voltage state is normal
static VoltageRange_e voltage_threshold = VoltageNormal;

// Logs voltage change to storage
static void LogVoltageChange(VoltageRange_e range, uint16_t voltageValue)
{
    switch(range)
    {
        case VoltageErrorLow:
            LogString("Error Low Voltage ", 0);
            break;

        case VoltageLow:
            LogString("Low Voltage ", 0);
            break;

        case VoltageNormal:
            LogString("Normal Voltage ", 0);
            break;

        case VoltageHigh:
            LogString("High Voltage ", 0);
            break;

        case VoltageErrorHigh:
            LogString("Error High Voltage ", 0);
            break;

        default:
            break;

    }

    LogNumber(voltageValue, 0);

    LogString(" dV\n", 0);

}

// Get voltage from voltmeter
uint16_t GetVoltage( void )
{
    uint16_t voltage = GetVoltageValue() * RawTodColts;
    return (voltage);
}

/****
    * Get range that the voltage falls into
    * Possible ranges are
    *   Normal      - Normal Operating Voltage
    *   Low         - Voltage low, but ok
    *   High        - Voltage high, but ok
    *   ErrorLow    - Voltage too low
    *   ErrorHigh   - Voltage too high
    */
VoltageRange_e GetVoltageRange( void )
{
    uint16_t voltage = GetVoltage();
    VoltageRange_e prev_threshold = voltage_threshold;

    if (voltage <= VoltageErrorLowThreshold_dC)
    {
        voltage_threshold = VoltageErrorLow;
    }
    else if (voltage <= VoltageLowThreshold_dC)
    {
        voltage_threshold = VoltageLow;
    }
    else if (voltage >= VoltageErrorHighThreshold_dC)
    {
        voltage_threshold = VoltageErrorHigh;
    }
    else if (voltage >= VoltageHighThreshold_dC)
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