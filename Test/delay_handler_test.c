/*============================================================================
  * ET: embedded test; very simple test example
============================================================================*/
// #include "pwm_handler.h"
#include "unity_fixture.h"
#include "delay_handler.h"  /* CUT */
#include <stdio.h>

#define PRESS (0)
#define HOLD  (1)

TEST_GROUP(Delay_Handler);

TEST_SETUP(Delay_Handler)
{
  /* executed before *every* non-skipped test */
}

TEST_TEAR_DOWN(Delay_Handler)
{
  /* executed after *every* non-skipped and non-failing test */
}

/* start delay_handler tests */
// Currently No Additional Tests
TEST(Delay_Handler, Test)
{
  TEST_ASSERT(1);
}
/* end delay_handler tests */

TEST_GROUP_RUNNER(Delay_Handler)
{
  RUN_TEST_CASE(Delay_Handler, Test);
}