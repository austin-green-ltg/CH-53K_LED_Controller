#include "unity_fixture.h"

static void RunAllTests(void)
{
  RUN_TEST_GROUP(Requirements);
  RUN_TEST_GROUP(Button_Handler);
  RUN_TEST_GROUP(Delay_Handler);
  RUN_TEST_GROUP(PWM_Handler);
  RUN_TEST_GROUP(Thermistor_Handler);
}

int main(int argc, const char * argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}