/*****************************************************************************
 *
 * @attention
 *  Copyright (c) 2022, 2023 Luminator, An LTG Company
 *  All rights reserved.
 *  Any use without the prior written consent of Luminator,
 *  An LTG Company is strictly prohibited.
 *
 *****************************************************************************
 *****************************************************************************
 *
 * @file button_handler.h
 *
 * @brief Returns the button state of the three board buttons
 *
 * Revision History:
 * Date       - Name         -  Ver -  Remarks
 * 07/31/2024 - Austin Green -  1.0 -  Initial Document
 * 09/10/2024 - Austin Green -  2.0 -  Doxyfile documentation
 *
 * Notes: Depends on the board support package bsp for GPIO_PinState
 *
 *****************************************************************************/

#ifndef INC_button_handlerh
#define INC_button_handlerh

#include <stdint.h>

#include "stm32l412xx-bsp.h"

/**
  * @brief Return state of toggle button
  * @param[out] Toogle Pin State, pressed or not
  */
GPIO_PinState IsTogglePressed ( void );

/**
  * @brief Return state of dim button
  * @param[out] Dim Pin State, pressed or not
  */
GPIO_PinState IsDimPressed ( void );

/**
  * @brief Return state of brighten button
  * @param[out] Bright Pin State, pressed or not
  */
GPIO_PinState IsBrightPressed ( void );

#endif /* INC_button_handlerh */
