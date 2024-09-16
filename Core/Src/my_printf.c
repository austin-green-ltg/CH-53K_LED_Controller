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
 * @file my_printf.c
 *
 * @brief Prints characters to a terminal for debugging purposes
 *
 * Revision History:
 * Date       - Name         -  Ver -  Remarks
 * 07/31/2024 - Austin Green -  1.0 -  Initial Document
 * 09/10/2024 - Austin Green -  2.0 -  Doxyfile documentation
 *
 * Notes: Depends on the board support package bsp.
 *        ENABLE_UART_DEBUGGING must be defined to use UART output
 *
 *****************************************************************************/

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
#include "my_printf.h"
#include "stm32l412xx-bsp.h"

/**
  * @brief sends character via uart
  * @param[in] c Character to printf
  * @param[in] stream Pointer to file stream (unused)
  * @param[out] Character printed
  */
int fputc ( int c, FILE *stream )
{
  ( void ) stream;
  sendUARTChar ( c );
  return c;
}

#endif /* ENABLE_UART_DEBUGGING */
