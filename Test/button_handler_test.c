/*============================================================================
  * ET: embedded test; very simple test example
============================================================================*/
// #include "pwm_handler.h"
#include "unity_fixture.h"
#include "button_handler.h"  /* CUT */
#include <stdio.h>

#define PRESS (0)
#define HOLD  (1)

TEST_GROUP(Button_Handler);

TEST_SETUP(Button_Handler)
{
  /* executed before *every* non-skipped test */
}

TEST_TEAR_DOWN(Button_Handler)
{
  /* executed after *every* non-skipped and non-failing test */
}

/* start button_handler tests */
// Currently No Additional Tests
TEST(Button_Handler, Test)
{
  TEST_ASSERT(1);
}
/* end button_handler tests */

TEST_GROUP_RUNNER(Button_Handler)
{
  RUN_TEST_CASE(Button_Handler, Test);
}