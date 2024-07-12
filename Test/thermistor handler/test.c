/*============================================================================
* ET: embedded test; very simple test example
============================================================================*/
// #include "pwm_handler.h"
#include "et.h" /* ET: embedded test */
#include "thermistor_handler.h"  /* CUT */
#include <stdio.h>

#define HEATING_WARM_THERM (100000)
#define HEATING_HOT_THERM  (120000)
#define COOLING_COOL_THERM (80000)
#define COOLING_WARM_THERM (100000)

extern int32_t thermistor_value;

void setup(void) {
    /* executed before *every* non-skipped test */
}

void teardown(void) {
    /* executed after *every* non-skipped and non-failing test */
    thermistor_value = 0; // return to default value
    get_temperature_range(); // set thermistor range
}

/* TESTS */
/*
  * Check get values
  * Check get range
*/

/* test group --------------------------------------------------------------*/
TEST_GROUP("Thermistor Handler")
{

    /* start thermistor_handler tests */
    TEST("Get Default Temperature")
    {
      VERIFY(get_temperature() == 0); // Default value
    }
    TEST("Get Default Temperature Range")
    {
      VERIFY(get_temperature_range() == Cool);
    }
    TEST("Get Temperature")
    {
      thermistor_value = 70000;
      VERIFY(get_temperature() == 70);

      thermistor_value = HEATING_WARM_THERM;
      VERIFY(get_temperature() == 100);

      thermistor_value = HEATING_HOT_THERM;
      VERIFY(get_temperature() == 120);

      thermistor_value = 150000;
      VERIFY(get_temperature() == 150);

      thermistor_value = -70000;
      VERIFY(get_temperature() == -70);

      thermistor_value = -100000;
      VERIFY(get_temperature() == -100);

      thermistor_value = -120000;
      VERIFY(get_temperature() == -120);

      thermistor_value = -150000;
      VERIFY(get_temperature() == -150);
    }
    TEST("Temperature Range Normal Stepping")
    {
      thermistor_value = HEATING_WARM_THERM;
      VERIFY(get_temperature_range() == Warm);

      thermistor_value = HEATING_HOT_THERM;
      VERIFY(get_temperature_range() == Hot);

      thermistor_value = COOLING_WARM_THERM;
      VERIFY(get_temperature_range() == Warm);

      thermistor_value = COOLING_COOL_THERM;
      VERIFY(get_temperature_range() == Cool);
    }
    TEST("Temperature Range Cool Testing")
    {
      thermistor_value = HEATING_WARM_THERM;
      VERIFY(get_temperature_range() == Warm);

      thermistor_value = COOLING_COOL_THERM;
      VERIFY(get_temperature_range() == Cool);

      thermistor_value = HEATING_HOT_THERM;
      VERIFY(get_temperature_range() == Hot);
    }
    TEST("Temperature Range Warm Testing")
    {
      thermistor_value = HEATING_WARM_THERM;
      VERIFY(get_temperature_range() == Warm);

      thermistor_value = COOLING_COOL_THERM;
      VERIFY(get_temperature_range() == Cool);

      thermistor_value = HEATING_WARM_THERM;
      VERIFY(get_temperature_range() == Warm);

      thermistor_value = HEATING_HOT_THERM;
      VERIFY(get_temperature_range() == Hot);

      thermistor_value = COOLING_WARM_THERM;
      VERIFY(get_temperature_range() == Warm);
    }
    TEST("Temperature Range Hot Testing")
    {
      thermistor_value = HEATING_HOT_THERM;
      VERIFY(get_temperature_range() == Hot);

      thermistor_value = COOLING_COOL_THERM;
      VERIFY(get_temperature_range() == Cool);

      thermistor_value = HEATING_HOT_THERM;
      VERIFY(get_temperature_range() == Hot);

      thermistor_value = COOLING_WARM_THERM;
      VERIFY(get_temperature_range() == Warm);
    }
    TEST("Temperature Range Cool Boundary Testing")
    {
      thermistor_value = (HEATING_WARM_THERM - 1000);
      VERIFY(get_temperature_range() == Cool);

      thermistor_value = HEATING_WARM_THERM;
      VERIFY(get_temperature_range() == Warm);

      thermistor_value = COOLING_COOL_THERM;
      VERIFY(get_temperature_range() == Cool);

      thermistor_value = (HEATING_HOT_THERM - 1000);
      VERIFY(get_temperature_range() == Warm);

      thermistor_value = COOLING_COOL_THERM;
      VERIFY(get_temperature_range() == Cool);

      thermistor_value = HEATING_HOT_THERM;
      VERIFY(get_temperature_range() == Hot);
    }
    TEST("Temperature Range Warm Boundary Testing")
    {
      thermistor_value = HEATING_WARM_THERM;
      VERIFY(get_temperature_range() == Warm);

      thermistor_value = (COOLING_COOL_THERM + 1000);
      VERIFY(get_temperature_range() == Warm);

      thermistor_value = COOLING_COOL_THERM;
      VERIFY(get_temperature_range() == Cool);

      thermistor_value = HEATING_WARM_THERM;
      VERIFY(get_temperature_range() == Warm);

      thermistor_value = (HEATING_HOT_THERM - 1000);
      VERIFY(get_temperature_range() == Warm);

      thermistor_value = HEATING_HOT_THERM;
      VERIFY(get_temperature_range() == Hot);
    }
    TEST("Temperature Range Hot Boundary Testing")
    {
      thermistor_value = HEATING_HOT_THERM;
      VERIFY(get_temperature_range() == Hot);

      thermistor_value = (COOLING_WARM_THERM + 1000);
      VERIFY(get_temperature_range() == Hot);

      thermistor_value = COOLING_WARM_THERM;
      VERIFY(get_temperature_range() == Warm);

      thermistor_value = HEATING_HOT_THERM;
      VERIFY(get_temperature_range() == Hot);

      thermistor_value = (COOLING_COOL_THERM + 1000);
      VERIFY(get_temperature_range() == Warm);

      thermistor_value = HEATING_HOT_THERM;
      VERIFY(get_temperature_range() == Hot);

      thermistor_value = COOLING_COOL_THERM;
      VERIFY(get_temperature_range() == Cool);
    }
    /* end thermistor_handler tests */

} /* TEST_GROUP() */