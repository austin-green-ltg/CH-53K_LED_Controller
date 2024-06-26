// Software tracing with printf()
#ifndef MY_PRINTF_H
#define MY_PRINTF_H

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
#include <stdio.h>

#define MY_PRINTF(format_, ...) printf(format_, ##__VA_ARGS__)

#else /* tracing disabled */

#define MY_PRINTF(format_, ...) (0)

#endif /* ENABLE_UART_DEBUGGING */

#endif /* MY_PRINTF_H */