#include "unity_fixture.h"      /* UNITY */
#include "button_handler.h"     /* CUT */
#include "stm32l412xx-bsp.h"    /* CUT */

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

TEST(Button_Handler, DimPin)
{
    dim_pin = 0;
    TEST_ASSERT_EQUAL_INT(0, IsDimPressed());
    dim_pin = 1;
    TEST_ASSERT_EQUAL_INT(1, IsDimPressed());
}

TEST(Button_Handler, BrightPin)
{
    bright_pin = 0;
    TEST_ASSERT_EQUAL_INT(0, IsBrightPressed());
    bright_pin = 1;
    TEST_ASSERT_EQUAL_INT(1, IsBrightPressed());
}

/* end button_handler tests */

TEST_GROUP_RUNNER(Button_Handler)
{
    RUN_TEST_CASE(Button_Handler, DimPin);
    RUN_TEST_CASE(Button_Handler, BrightPin);
}