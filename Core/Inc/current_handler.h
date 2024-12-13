/*****************************************************************************
  *
  *  @attention
  *  Copyright (c) 2022, 2023 Luminator, An LTG Company
  *  All rights reserved.
  *  Any use without the prior written consent of Luminator,
  *  An LTG Company is strictly prohibited.
  *
  *****************************************************************************
  *****************************************************************************
  *
  * @file current_handler.h
  *
  * @brief Handles getting current and reporting values.
  *
  * Revision History:
  * Date       - Name         -  Ver -  Remarks
  * 08/05/2024 - Austin Green -  1.0 -  Initial Document
  * 09/10/2024 - Austin Green -  2.0 -  Doxyfile documentation
  *
  * Notes:
  *
*****************************************************************************/

#ifndef INC_current_handlerh
#define INC_current_handlerh

#include <stdint.h>

/** Current Range Enum */
typedef enum
{
    CurrentNormal = 0, /**< Normal Operating Current */
    CurrentHigh = 1, /**< Current high, but ok     */
    CurrentError = 2 /**< Current too high         */
} CurrentRange_e;

/**
  * @brief Logs current to storage
  */
void LogCurrent ( void );

/**
  * @brief Get current from ammeter
  *
  * @param[out] current level in dA
*/
uint16_t GetCurrent ( void );

/**
  * @brief Get range that the current falls into
*         Possible ranges are
*           Normal  - Normal Operating Current
*           High    - Current high, but ok
*           Error   - Current too high
* @param[out] Current current range
*/
CurrentRange_e GetCurrentRange ( void );

#endif /* INC_current_handlerh */
