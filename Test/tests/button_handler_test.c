#include "unity_fixture.h"      /* UNITY */
#include "button_handler.h"     /* CUT */

#define PRESS (0)
#define HOLD  (1)

extern GPIO_PinState toggle_pin;
extern GPIO_PinState dim_pin;
extern GPIO_PinState bright_pin;

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
TEST(Button_Handler, TogglePin)
{
  toggle_pin = 0;
  TEST_ASSERT_EQUAL_INT(0, isTogglePressed());
  toggle_pin = 1;
  TEST_ASSERT_EQUAL_INT(1, isTogglePressed());
}

TEST(Button_Handler, DimPin)
{
  dim_pin = 0;
  TEST_ASSERT_EQUAL_INT(0, isDimPressed());
  dim_pin = 1;
  TEST_ASSERT_EQUAL_INT(1, isDimPressed());
}

TEST(Button_Handler, BrightPin)
{
  bright_pin = 0;
  TEST_ASSERT_EQUAL_INT(0, isBrightPressed());
  bright_pin = 1;
  TEST_ASSERT_EQUAL_INT(1, isBrightPressed());
}

/* end button_handler tests */

TEST_GROUP_RUNNER(Button_Handler)
{
  RUN_TEST_CASE(Button_Handler, TogglePin);
  RUN_TEST_CASE(Button_Handler, DimPin);
  RUN_TEST_CASE(Button_Handler, BrightPin);
}