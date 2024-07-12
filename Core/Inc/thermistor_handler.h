
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __THERMISTOR_HANDLER_H
#define __THERMISTOR_HANDLER_H

#include "bsp.h"
#include <stdint.h>

typedef enum
{
  Cool  = 0,
  Warm  = 1,
  Hot   = 2
}TemperatureRange_e;

int16_t get_temperature( void );
TemperatureRange_e get_temperature_range( void );

#endif /* __THERMISTOR_HANDLER_H */
