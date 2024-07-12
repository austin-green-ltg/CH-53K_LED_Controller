/* Private includes ----------------------------------------------------------*/
#include "thermistor_handler.h"

#define THERMISTOR_TO_CELCIUS (1.0f / 1000.0f)
#define CELCIUS_TO_THERMISTOR (1.0f / THERMISTOR_TO_CELCIUS)
#define HEATING_THRESHOLD_1   (100)
#define HEATING_THRESHOLD_2   (120)
#define COOLING_THRESHOLD_1   (HEATING_THRESHOLD_1 - 20)
#define COOLING_THRESHOLD_2   (HEATING_THRESHOLD_2 - 20)

static TemperatureRange_e temperature_threshold = Cool;

int16_t get_temperature( void )
{
  float temperature = getThermistorValue() * THERMISTOR_TO_CELCIUS;
  temperature += (temperature > 0 ? 0.5f : -0.5f);
  return (int16_t)(temperature);
}

TemperatureRange_e get_temperature_range( void )
{
  int16_t temperature = get_temperature();

  if (temperature_threshold == Cool)
  {
    // no cooling check needed
    if (temperature >= HEATING_THRESHOLD_2)       temperature_threshold = Hot;
    else if (temperature >= HEATING_THRESHOLD_1)  temperature_threshold = Warm;
  }
  else if (temperature_threshold == Warm)
  {
    // check if cooled down or heated up
    if (temperature <= COOLING_THRESHOLD_1)       temperature_threshold = Cool;
    else if (temperature >= HEATING_THRESHOLD_2)  temperature_threshold = Hot;
  }
  else if (temperature_threshold == Hot)
  {
    // check if cooled down
    if (temperature <= COOLING_THRESHOLD_1)       temperature_threshold = Cool;
    else if (temperature <= COOLING_THRESHOLD_2)  temperature_threshold = Warm;
  }

  return temperature_threshold;

}