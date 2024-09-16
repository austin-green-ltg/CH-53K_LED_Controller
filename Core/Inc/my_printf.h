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
 * @file my_printf.h
 *
 * @brief Prints characters to a terminal for debugging purposes
 *
 * Revision History:
 * Date       - Name         -  Ver -  Remarks
 * 07/31/2024 - Austin Green -  1.0 -  Initial Document
 * 09/10/2024 - Austin Green -  2.0 -  Doxyfile documentation
 *
 * Notes:
 *
 *****************************************************************************/

// Software tracing with printf()
#ifndef INC_my_printfh
#define INC_my_printfh

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
    #include <stdio.h>
#endif /* ENABLE_UART_DEBUGGING */

#endif /* INC_my_printfh */
