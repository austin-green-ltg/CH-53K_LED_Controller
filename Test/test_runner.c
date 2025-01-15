#include <unity_fixture.h>      /* UNITY */

static void RunAllTests(void)
{
  RUN_TEST_GROUP(Requirements);
  RUN_TEST_GROUP(Button_Handler);
  RUN_TEST_GROUP(Com);
  RUN_TEST_GROUP(Delay_Handler);
  RUN_TEST_GROUP(PWM_Handler);
  RUN_TEST_GROUP(Temperature_Handler);
  RUN_TEST_GROUP(Fram);
  RUN_TEST_GROUP(Logger);
  RUN_TEST_GROUP(Current_Handler);
  RUN_TEST_GROUP(Voltage_Handler);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}
