#include <stdlib.h>

#include "unity_fixture.h"      /* UNITY */
#include "delay_handler.h"      /* CUT */
#include "bsp.h"                /* CUT */

#define PRESS (0)
#define HOLD  (1)

TEST_GROUP(Delay_Handler);

extern TimerStruct timer;

TEST_SETUP(Delay_Handler)
{
    /* executed before *every* non-skipped test */
    timer.is_running = 0;
    timer.time = 0;
}

TEST_TEAR_DOWN(Delay_Handler)
{
    /* executed after *every* non-skipped and non-failing test */
}

/* start delay_handler tests */
TEST(Delay_Handler, StartDelayCounter)
{
    StartDelayCounter();
    TEST_ASSERT_EQUAL_INT(1, timer.is_running);
    TEST_ASSERT_EQUAL_INT(0, timer.time);
}

TEST(Delay_Handler, RestartDelayCounter)
{
    StartDelayCounter();
    timer.time = 100;
    TEST_ASSERT_EQUAL_INT(100, timer.time);
    RestartDelayCounter();
    TEST_ASSERT_EQUAL_INT(0, timer.time);
}

TEST(Delay_Handler, delayNotHit)
{
    TEST_ASSERT_FALSE(DelayHit(100));
    timer.time = 99;
    TEST_ASSERT_FALSE(DelayHit(100));
}

TEST(Delay_Handler, delayExactHit)
{
    timer.time = 100;
    TEST_ASSERT_TRUE(DelayHit(100));
}

TEST(Delay_Handler, delayPassedHit)
{
    timer.time = 101;
    TEST_ASSERT_TRUE(DelayHit(100));
}

#define ITERATIONS (100)
TEST(Delay_Handler, BrightnessDelay)
{
    // Test random brightness values and verify they fit constraints
    // constraints calculated from requirements
    // SrchLt.01251 In the Visible Mode, the Controllable Searchlight dimmer shall brighten from the initial 50% to Full Bright in 4.5 +/-0.5 seconds.
    // SrchLt.01252 In the IR Mode, the Controllable Searchlight dimmer shall brighten from the initial 50% to Full Bright in 4.5 +/-0.5 seconds.
    // SrchLt.01253 In the Visible Mode, the Controllable Searchlight dimmer shall dim from 50% to OFF in 3.25 +/-0.5 seconds.
    // SrchLt.01254 In the IR Mode, the Controllable Searchlight dimmer shall dim from 50% to OFF in 3.25 +/-0.5 seconds.
    // SrchLt.01255 In the Visible Mode, the Controllable Searchlight shall dim from FULL Bright to Minimum in 6 +/-1.5 seconds.
    // SrchLt.01256 In the IR Mode, the Controllable Searchlight shall brighten from Minimum to Full Bright in 6 +/-1.5 seconds.
    for (int i = 0; i < ITERATIONS; i++)
    {
        int8_t brightness = rand();
        TEST_ASSERT_EQUAL_INT16((brightness * 5) + 250, BrightnessDelay(brightness));
    }
}
/* end delay_handler tests */

TEST_GROUP_RUNNER(Delay_Handler)
{
    RUN_TEST_CASE(Delay_Handler, StartDelayCounter);
    RUN_TEST_CASE(Delay_Handler, RestartDelayCounter);
    RUN_TEST_CASE(Delay_Handler, delayNotHit);
    RUN_TEST_CASE(Delay_Handler, delayExactHit);
    RUN_TEST_CASE(Delay_Handler, delayPassedHit);
    RUN_TEST_CASE(Delay_Handler, BrightnessDelay);
}