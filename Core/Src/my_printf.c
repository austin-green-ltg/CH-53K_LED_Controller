// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: my_printf.c
//
// Description: Prints characters to a terminal for debugging purposes
//
// Revision History:
// Date       - Name         -  Ver -  Remarks
// 07/31/2024 - Austin Green -  1.0 -  Initial Document
//
// Notes: Depends on the board support package bsp.
//        ENABLE_UART_DEBUGGING must be defined to use UART output
//
// ***************************************************************************

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
    #include "my_printf.h"
    #include "stm32l412xx-bsp.h"

    int fputc(int c, FILE *stream)
    {
        (void)stream;
        sendUARTChar(c);
        return c;
    }
#endif /* ENABLE_UART_DEBUGGING */
