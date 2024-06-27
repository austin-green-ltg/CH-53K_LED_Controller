
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_HANDLER_H
#define __DELAY_HANDLER_H

#include <stdint.h>
#include "stm32f3xx_hal.h"

void    startDelayCounter(void);     // start the counter
void    restartDelayCounter(void);   // restart the counter
uint8_t delayHit(uint32_t delay_ms); // is delay in ms hit

#endif /* __DELAY_HANDLER_H */
