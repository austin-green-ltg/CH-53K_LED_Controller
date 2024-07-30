
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_thermistor_handlerh
#define INC_thermistor_handlerh

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

#endif /* INC_thermistor_handlerh */
