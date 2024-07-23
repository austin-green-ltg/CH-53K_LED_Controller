/*============================================================================
  * ET: embedded test; very simple test example
============================================================================*/
// #include "pwm_handler.h"
#include "unity_fixture.h"
#include "thermistor_handler.h"  /* CUT */
#include <stdio.h>

#define HEATING_WARM_THERM (100000)
#define HEATING_HOT_THERM  (120000)
#define COOLING_COOL_THERM (80000)
#define COOLING_WARM_THERM (100000)

extern int32_t thermistor_value;

TEST_GROUP(Thermistor_Handler);

TEST_SETUP(Thermistor_Handler)
{
  /* executed before *every* non-skipped test */
}

TEST_TEAR_DOWN(Thermistor_Handler)
{
  /* executed after *every* non-skipped and non-failing test */
  thermistor_value = 0; // return to default value
  get_temperature_range(); // set thermistor range
}

/* TESTS */
/*
  * Check get values
  * Check get range
*/

/* start thermistor_handler tests */
TEST(Thermistor_Handler, GetDefaultTemperature)
{
  TEST_ASSERT(get_temperature() == 0); // Default value
}
TEST(Thermistor_Handler, GetDefaultTemperatureRange)
{
  TEST_ASSERT(get_temperature_range() == Cool);
}
TEST(Thermistor_Handler, GetTemperature)
{
  thermistor_value = 70000;
  TEST_ASSERT(get_temperature() == 70);

  thermistor_value = HEATING_WARM_THERM;
  TEST_ASSERT(get_temperature() == 100);

  thermistor_value = HEATING_HOT_THERM;
  TEST_ASSERT(get_temperature() == 120);

  thermistor_value = 150000;
  TEST_ASSERT(get_temperature() == 150);

  thermistor_value = -70000;
  TEST_ASSERT(get_temperature() == -70);

  thermistor_value = -100000;
  TEST_ASSERT(get_temperature() == -100);

  thermistor_value = -120000;
  TEST_ASSERT(get_temperature() == -120);

  thermistor_value = -150000;
  TEST_ASSERT(get_temperature() == -150);
}
TEST(Thermistor_Handler, TemperatureRangeNormalStepping)
{
  thermistor_value = HEATING_WARM_THERM;
  TEST_ASSERT(get_temperature_range() == Warm);

  thermistor_value = HEATING_HOT_THERM;
  TEST_ASSERT(get_temperature_range() == Hot);

  thermistor_value = COOLING_WARM_THERM;
  TEST_ASSERT(get_temperature_range() == Warm);

  thermistor_value = COOLING_COOL_THERM;
  TEST_ASSERT(get_temperature_range() == Cool);
}
TEST(Thermistor_Handler, TemperatureRangeCoolTesting)
{
  thermistor_value = HEATING_WARM_THERM;
  TEST_ASSERT(get_temperature_range() == Warm);

  thermistor_value = COOLING_COOL_THERM;
  TEST_ASSERT(get_temperature_range() == Cool);

  thermistor_value = HEATING_HOT_THERM;
  TEST_ASSERT(get_temperature_range() == Hot);
}
TEST(Thermistor_Handler, TemperatureRangeWarmTesting)
{
  thermistor_value = HEATING_WARM_THERM;
  TEST_ASSERT(get_temperature_range() == Warm);

  thermistor_value = COOLING_COOL_THERM;
  TEST_ASSERT(get_temperature_range() == Cool);

  thermistor_value = HEATING_WARM_THERM;
  TEST_ASSERT(get_temperature_range() == Warm);

  thermistor_value = HEATING_HOT_THERM;
  TEST_ASSERT(get_temperature_range() == Hot);

  thermistor_value = COOLING_WARM_THERM;
  TEST_ASSERT(get_temperature_range() == Warm);
}
TEST(Thermistor_Handler, TemperatureRangeHotTesting)
{
  thermistor_value = HEATING_HOT_THERM;
  TEST_ASSERT(get_temperature_range() == Hot);

  thermistor_value = COOLING_COOL_THERM;
  TEST_ASSERT(get_temperature_range() == Cool);

  thermistor_value = HEATING_HOT_THERM;
  TEST_ASSERT(get_temperature_range() == Hot);

  thermistor_value = COOLING_WARM_THERM;
  TEST_ASSERT(get_temperature_range() == Warm);
}
TEST(Thermistor_Handler, TemperatureRangeCoolBoundaryTesting)
{
  thermistor_value = (HEATING_WARM_THERM - 1000);
  TEST_ASSERT(get_temperature_range() == Cool);

  thermistor_value = HEATING_WARM_THERM;
  TEST_ASSERT(get_temperature_range() == Warm);

  thermistor_value = COOLING_COOL_THERM;
  TEST_ASSERT(get_temperature_range() == Cool);

  thermistor_value = (HEATING_HOT_THERM - 1000);
  TEST_ASSERT(get_temperature_range() == Warm);

  thermistor_value = COOLING_COOL_THERM;
  TEST_ASSERT(get_temperature_range() == Cool);

  thermistor_value = HEATING_HOT_THERM;
  TEST_ASSERT(get_temperature_range() == Hot);
}
TEST(Thermistor_Handler, TemperatureRangeWarmBoundaryTesting)
{
  thermistor_value = HEATING_WARM_THERM;
  TEST_ASSERT(get_temperature_range() == Warm);

  thermistor_value = (COOLING_COOL_THERM + 1000);
  TEST_ASSERT(get_temperature_range() == Warm);

  thermistor_value = COOLING_COOL_THERM;
  TEST_ASSERT(get_temperature_range() == Cool);

  thermistor_value = HEATING_WARM_THERM;
  TEST_ASSERT(get_temperature_range() == Warm);

  thermistor_value = (HEATING_HOT_THERM - 1000);
  TEST_ASSERT(get_temperature_range() == Warm);

  thermistor_value = HEATING_HOT_THERM;
  TEST_ASSERT(get_temperature_range() == Hot);
}
TEST(Thermistor_Handler, TemperatureRangeHotBoundaryTesting)
{
  thermistor_value = HEATING_HOT_THERM;
  TEST_ASSERT(get_temperature_range() == Hot);

  thermistor_value = (COOLING_WARM_THERM + 1000);
  TEST_ASSERT(get_temperature_range() == Hot);

  thermistor_value = COOLING_WARM_THERM;
  TEST_ASSERT(get_temperature_range() == Warm);

  thermistor_value = HEATING_HOT_THERM;
  TEST_ASSERT(get_temperature_range() == Hot);

  thermistor_value = (COOLING_COOL_THERM + 1000);
  TEST_ASSERT(get_temperature_range() == Warm);

  thermistor_value = HEATING_HOT_THERM;
  TEST_ASSERT(get_temperature_range() == Hot);

  thermistor_value = COOLING_COOL_THERM;
  TEST_ASSERT(get_temperature_range() == Cool);
}
/* end thermistor_handler tests */


TEST_GROUP_RUNNER(Thermistor_Handler)
{
  RUN_TEST_CASE(Thermistor_Handler, GetDefaultTemperature);
  RUN_TEST_CASE(Thermistor_Handler, GetDefaultTemperatureRange);
  RUN_TEST_CASE(Thermistor_Handler, GetTemperature);
  RUN_TEST_CASE(Thermistor_Handler, TemperatureRangeNormalStepping);
  RUN_TEST_CASE(Thermistor_Handler, TemperatureRangeCoolTesting);
  RUN_TEST_CASE(Thermistor_Handler, TemperatureRangeWarmTesting);
  RUN_TEST_CASE(Thermistor_Handler, TemperatureRangeHotTesting);
  RUN_TEST_CASE(Thermistor_Handler, TemperatureRangeCoolBoundaryTesting);
  RUN_TEST_CASE(Thermistor_Handler, TemperatureRangeWarmBoundaryTesting);
  RUN_TEST_CASE(Thermistor_Handler, TemperatureRangeHotBoundaryTesting);
}