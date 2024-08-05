// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: voltage_handler.h
//
// Description: Handles getting voltage and reporting values.
//
// Revision History:
// Date       - Name         -  Ver -  Remarks
// 08/05/2024 - Austin Green -  1.0 -  Initial Document
//
// Notes:
//
// ***************************************************************************

#ifndef INC_voltage_handlerh
    #define INC_voltage_handlerh

    #include <stdint.h>

    /* Voltage Range Enum */
    typedef enum
    {
        VoltageNormal       = 0,
        VoltageLow          = 1,
        VoltageHigh         = 2,
        VoltageErrorLow     = 3,
        VoltageErrorHigh    = 4
    } VoltageRange_e;

    /* Get Voltage */
    uint16_t GetVoltage( void ); // in mV
    VoltageRange_e GetVoltageRange( void );

#endif /* INC_voltage_handlerh */
