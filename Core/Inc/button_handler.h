// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: button_handler.h
//
// Description: Returns the button state of the three board buttons
//
// Revision History:
// Date       - Name         -  Ver -  Remarks
// 07/31/2024 - Austin Green -  1.0 -  Initial Document
//
// Notes: Depends on the board support package bsp
//
// ***************************************************************************

#ifndef INC_button_handlerh
    #define INC_button_handlerh

    #include <stdint.h>

    #include "bsp.h"

    /* Return state of buttons */
    GPIO_PinState IsTogglePressed ( void );
    GPIO_PinState IsDimPressed    ( void );
    GPIO_PinState IsBrightPressed ( void );

#endif /* INC_button_handlerh */
