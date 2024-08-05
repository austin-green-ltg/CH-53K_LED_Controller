// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: current_handler.h
//
// Description: Handles getting current and reporting values.
//
// Revision History:
// Date       - Name         -  Ver -  Remarks
// 08/05/2024 - Austin Green -  1.0 -  Initial Document
//
// Notes:
//
// ***************************************************************************

#ifndef INC_current_handlerh
    #define INC_current_handlerh

    #include <stdint.h>

    /* Current Range Enum */
    typedef enum
    {
        Normal  = 0,
        High    = 1,
        Error   = 2
    } CurrentRange_e;

    /* Get Current */
    uint16_t GetCurrent_mA( void );
    CurrentRange_e GetCurrentRange( void );

#endif /* INC_current_handlerh */
