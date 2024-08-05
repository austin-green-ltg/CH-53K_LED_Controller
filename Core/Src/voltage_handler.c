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
#include "bsp.h"
#include "logger.h"

const uint16_t  RawTomVolts = (1);
const uint16_t  mVoltsToRaw = (1);

const uint16_t VoltageErrorLowThreshold_mV  = 24000u;
const uint16_t VoltageLowThreshold_mV       = 26000u;
const uint16_t VoltageHighThreshold_mV      = 30000u;
const uint16_t VoltageErrorHighThreshold_mV = 32000u;

static VoltageRange_e voltage_threshold = VoltageNormal;

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

    LogString(" mV\n", 0);

}

uint16_t GetVoltage( void )
{
    uint16_t voltage = GetVoltageValue() * RawTomVolts;
    return (voltage);
}

VoltageRange_e GetVoltageRange( void )
{
    uint16_t voltage = GetVoltage();
    VoltageRange_e prev_threshold = voltage_threshold;

    if (voltage <= VoltageErrorLowThreshold_mV)
    {
        voltage_threshold = VoltageErrorLow;
    }
    else if (voltage <= VoltageLowThreshold_mV)
    {
        voltage_threshold = VoltageLow;
    }
    else if (voltage >= VoltageErrorHighThreshold_mV)
    {
        voltage_threshold = VoltageErrorHigh;
    }
    else if (voltage >= VoltageHighThreshold_mV)
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