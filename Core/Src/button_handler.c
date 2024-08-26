// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: button_handler.c
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

/* Private includes ----------------------------------------------------------*/
#include "button_handler.h"
#include "stm32l412xx-bsp.h"

/* Private define ------------------------------------------------------------*/

// Check if dim button is pressed
GPIO_PinState IsDimPressed    ( void )
{
    return ReadDimPin();
}

// Check if bright button is pressed
GPIO_PinState IsBrightPressed ( void )
{
    return ReadBrightPin();
}