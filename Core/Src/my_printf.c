#ifdef ENABLE_UART_DEBUGGING
#include "my_printf.h"
#include "bsp.h"
int fputc(int c, FILE *stream)
{
  (void)stream;
  sendUARTChar(c);
  return c;
}
#endif