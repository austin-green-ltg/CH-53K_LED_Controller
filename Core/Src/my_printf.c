#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
#include "my_printf.h"
#include "bsp.h"
int fputc(int c, FILE *stream)
{
  (void)stream;
  sendUARTChar(c);
  return c;
}
#endif /* ENABLE_UART_DEBUGGING */
