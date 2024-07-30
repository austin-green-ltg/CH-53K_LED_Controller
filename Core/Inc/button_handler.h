
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_button_handlerh
#define INC_button_handlerh

#include "bsp.h"
#include <stdint.h>

GPIO_PinState isTogglePressed ( void );
GPIO_PinState isDimPressed    ( void );
GPIO_PinState isBrightPressed ( void );

#endif /* INC_button_handlerh */
