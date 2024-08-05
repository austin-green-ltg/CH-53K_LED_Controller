// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: thermistor_handler.h
//
// Description: Handles getting this temperature and transitioning
//                between temperature states.
//
// Revision History:
// Date       - Name         -  Ver -  Remarks
// 07/31/2024 - Austin Green -  1.0 -  Initial Document
//
// Notes:
//
// ***************************************************************************

#ifndef INC_thermistor_handlerh
    #define INC_thermistor_handlerh

    #include <stdint.h>

    /* Temperature Range Enum */
    typedef enum
    {
        TempCool    = 0,
        TempWarm    = 1,
        TempHot     = 2
    } TemperatureRange_e;

    /* Get Temperature */
    int16_t GetTemperature( void );
    TemperatureRange_e GetTemperatureRange( void );

#endif /* INC_thermistor_handlerh */
