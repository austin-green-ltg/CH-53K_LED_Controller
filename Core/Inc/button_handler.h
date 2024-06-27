
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BUTTON_HANDLER_H
#define __BUTTON_HANDLER_H

#include <stdint.h>
#include "stm32f3xx_hal.h"

GPIO_PinState isTogglePressed ( void );
GPIO_PinState isDimPressed    ( void );
GPIO_PinState isBrightPressed ( void );

#endif /* __BUTTON_HANDLER_H */
