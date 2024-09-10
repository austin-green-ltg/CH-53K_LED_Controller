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
 * @file button_handler.c
 *
 * @brief Returns the button state of the three board buttons
 *
 * Revision History:
 * Date       - Name         -  Ver -  Remarks
 * 07/31/2024 - Austin Green -  1.0 -  Initial Document
 * 09/10/2024 - Austin Green -  2.0 -  Doxyfile documentation
 *
 * Notes: Depends on the board support package bsp
 *
 *****************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include "button_handler.h"
#include "stm32l412xx-bsp.h"

/* Private define ------------------------------------------------------------*/

/**
  * @brief Return state of dim button
  * @param[out] Dim Pin State, pressed or not
  */
GPIO_PinState IsDimPressed    ( void )
{
    return ReadDimPin();
}

/**
  * @brief Return state of brighten button
  * @param[out] Bright Pin State, pressed or not
  */
GPIO_PinState IsBrightPressed ( void )
{
    return ReadBrightPin();
}