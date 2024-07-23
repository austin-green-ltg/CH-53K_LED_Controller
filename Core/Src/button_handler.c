/* Private includes ----------------------------------------------------------*/
#include "button_handler.h"
#include "bsp.h"
#include "my_printf.h"

/* Private define ------------------------------------------------------------*/

GPIO_PinState isTogglePressed ( void )
{
  return readTogglePin();
}
GPIO_PinState isDimPressed    ( void )
{
  return readDimPin();
}
GPIO_PinState isBrightPressed ( void )
{
  return readBrightPin();
}