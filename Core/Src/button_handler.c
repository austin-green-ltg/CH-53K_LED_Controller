/* Private includes ----------------------------------------------------------*/
#include "button_handler.h"
#include "bsp.h"
#include "my_printf.h"

/* Private define ------------------------------------------------------------*/

GPIO_PinState isTogglePressed ( void )
{
  GPIO_PinState pressed = readTogglePin();
  return pressed;
}
GPIO_PinState isDimPressed    ( void )
{
  GPIO_PinState pressed = readDimPin();
  return pressed;
}
GPIO_PinState isBrightPressed ( void )
{
  GPIO_PinState pressed = readBrightPin();
  return pressed;
}