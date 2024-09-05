// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: temperature_handler.h
//
// Description: Handles getting this temperature and transitioning
//                between temperature states.
//
// Revision History:
// Date       - Name         -  Ver -  Remarks
// 07/31/2024 - Austin Green -  1.0 -  Initial Document
// 08/05/2024 - Austin Green -  1.1 -  Refactor to not use floats
//
// Notes:
//
// ***************************************************************************

#ifndef INC_temperature_handlerh
    #define INC_temperature_handlerh

    #include <stdint.h>

    /* Temperature Range Enum */
    typedef enum
    {
        TempCool    = 0,
        TempWarm    = 1,
        TempHot     = 2
    } TemperatureRange_e;

    /* Get Temperature */
    int32_t GetTemperature( void ); // in dC
    TemperatureRange_e GetTemperatureRange( void );

#endif /* INC_temperature_handlerh */
