#include <stdlib.h>
#include <limits.h>

#include <unity_fixture.h>      /* UNITY */
#include "delay_handler.h"      /* CUT */
#include "stm32l412xx-bsp.h"    /* CUT */

#define PRESS (0)
#define HOLD  (1)

TEST_GROUP ( Delay_Handler );

extern TimerStruct timer;
extern TimerStruct logTimer;
extern TimerStruct liveLogTimer;

TEST_SETUP ( Delay_Handler )
{
    /* executed before *every* non-skipped test */
}

TEST_TEAR_DOWN ( Delay_Handler )
{
    /* executed after *every* non-skipped and non-failing test */
    timer.is_running = 0;
    timer.time = 0;
    logTimer.is_running = 0;
    logTimer.time = 0;
    liveLogTimer.is_running = 0;
    liveLogTimer.time = 0;
}

/* start delay_handler tests */
TEST ( Delay_Handler, StartDelayCounter )
{
    StartDelayCounter();
    TEST_ASSERT_EQUAL_INT ( 1, timer.is_running );
    TEST_ASSERT_EQUAL_INT ( 0, timer.time );
}

TEST ( Delay_Handler, StartLogDelayCounter )
{
    StartLogDelayCounter();
    TEST_ASSERT_EQUAL_INT ( 1, logTimer.is_running );
    TEST_ASSERT_EQUAL_INT ( 0, logTimer.time );
}

TEST ( Delay_Handler, StartLiveLogDelayCounter )
{
    StartLiveLogDelayCounter();
    TEST_ASSERT_EQUAL_INT ( 1, liveLogTimer.is_running );
    TEST_ASSERT_EQUAL_INT ( 0, liveLogTimer.time );
}

TEST ( Delay_Handler, RestartDelayCounter )
{
    const uint16_t time = ( rand() % UINT16_MAX - 1 ) + 1; // not 0, not max
    StartDelayCounter();

    timer.time = time;
    TEST_ASSERT_EQUAL_INT ( time, timer.time );
    RestartDelayCounter();
    TEST_ASSERT_EQUAL_INT ( 0, timer.time );
}

TEST ( Delay_Handler, RestartLogDelayCounter )
{
    const uint16_t time = ( rand() % UINT16_MAX - 1 ) + 1; // not 0, not max
    StartLogDelayCounter();

    logTimer.time = time;
    TEST_ASSERT_EQUAL_INT ( time, logTimer.time );
    RestartLogDelayCounter();
    TEST_ASSERT_EQUAL_INT ( 0, logTimer.time );
}

TEST ( Delay_Handler, RestartLiveLogDelayCounter )
{
    const uint16_t time = ( rand() % UINT16_MAX - 1 ) + 1; // not 0, not max
    StartLiveLogDelayCounter();

    liveLogTimer.time = time;
    TEST_ASSERT_EQUAL_INT ( time, liveLogTimer.time );
    RestartLiveLogDelayCounter();
    TEST_ASSERT_EQUAL_INT ( 0, liveLogTimer.time );
}

TEST ( Delay_Handler, delayNotHit )
{
    const uint16_t time = ( rand() % UINT16_MAX - 1 ) + 1; // not 0, not max

    TEST_ASSERT_FALSE ( DelayHit ( time ) );
    timer.time = time - 1;
    TEST_ASSERT_FALSE ( DelayHit ( time ) );
}

TEST ( Delay_Handler, logDelayNotHit )
{
    const uint16_t time = ( rand() % UINT16_MAX - 1 ) + 1; // not 0, not max

    TEST_ASSERT_FALSE ( LogDelayHit ( time ) );
    logTimer.time = time - 1;
    TEST_ASSERT_FALSE ( LogDelayHit ( time ) );
}

TEST ( Delay_Handler, liveLogDelayNotHit )
{
    const uint16_t time = ( rand() % UINT16_MAX - 1 ) + 1; // not 0, not max

    TEST_ASSERT_FALSE ( LiveLogDelayHit ( time ) );
    liveLogTimer.time = time - 1;
    TEST_ASSERT_FALSE ( LiveLogDelayHit ( time ) );
}

TEST ( Delay_Handler, delayExactHit )
{
    const uint16_t time = ( rand() % UINT16_MAX - 1 ) + 1; // not 0, not max

    timer.time = time;
    TEST_ASSERT_TRUE ( DelayHit ( time ) );
}

TEST ( Delay_Handler, logDelayExactHit )
{
    const uint16_t time = ( rand() % UINT16_MAX - 1 ) + 1; // not 0, not max

    logTimer.time = time;

    TEST_ASSERT_TRUE ( LogDelayHit ( time ) );
}

TEST ( Delay_Handler, liveLogDelayExactHit )
{
    const uint16_t time = ( rand() % UINT16_MAX - 1 ) + 1; // not 0, not max

    liveLogTimer.time = time;
    TEST_ASSERT_TRUE ( LiveLogDelayHit ( time ) );
}

TEST ( Delay_Handler, delayPassedHit )
{
    const uint16_t time = ( rand() % UINT16_MAX - 1 ) + 1; // not 0, not max

    timer.time = time + 1;
    TEST_ASSERT_TRUE ( DelayHit ( time ) );
}

TEST ( Delay_Handler, logDelayPassedHit )
{
    const uint16_t time = ( rand() % UINT16_MAX - 1 ) + 1; // not 0, not max

    logTimer.time = time + 1;
    TEST_ASSERT_TRUE ( LogDelayHit ( time ) );
}

TEST ( Delay_Handler, liveLogDelayPassedHit )
{
    const uint16_t time = ( rand() % UINT16_MAX - 1 ) + 1; // not 0, not max

    liveLogTimer.time = time + 1;
    TEST_ASSERT_TRUE ( LiveLogDelayHit ( time ) );
}

#define ITERATIONS (100)
TEST ( Delay_Handler, BrightnessDelay )
{
    // Test random brightness values and verify they fit constraints
    // constraints calculated from requirements
    // SrchLt.01251 In the Visible Mode, the Controllable Searchlight dimmer shall brighten from the initial 50% to Full Bright in 4.5 +/-0.5 seconds.
    // SrchLt.01252 In the IR Mode, the Controllable Searchlight dimmer shall brighten from the initial 50% to Full Bright in 4.5 +/-0.5 seconds.
    // SrchLt.01253 In the Visible Mode, the Controllable Searchlight dimmer shall dim from 50% to OFF in 3.25 +/-0.5 seconds.
    // SrchLt.01254 In the IR Mode, the Controllable Searchlight dimmer shall dim from 50% to OFF in 3.25 +/-0.5 seconds.
    // SrchLt.01255 In the Visible Mode, the Controllable Searchlight shall dim from FULL Bright to Minimum in 6 +/-1.5 seconds.
    // SrchLt.01256 In the IR Mode, the Controllable Searchlight shall brighten from Minimum to Full Bright in 6 +/-1.5 seconds.
    for ( int i = 0; i < ITERATIONS; i++ )
    {
        int8_t brightness = ( int8_t ) rand();
        TEST_ASSERT_EQUAL_INT16 ( ( brightness * 5 ) + 250,
                                  BrightnessDelay ( brightness ) );
    }
}

/* end delay_handler tests */

TEST_GROUP_RUNNER ( Delay_Handler )
{
    RUN_TEST_CASE ( Delay_Handler, StartDelayCounter );
    RUN_TEST_CASE ( Delay_Handler, StartLogDelayCounter );
    RUN_TEST_CASE ( Delay_Handler, StartLiveLogDelayCounter );
    RUN_TEST_CASE ( Delay_Handler, RestartDelayCounter );
    RUN_TEST_CASE ( Delay_Handler, RestartLogDelayCounter );
    RUN_TEST_CASE ( Delay_Handler, RestartLiveLogDelayCounter );
    RUN_TEST_CASE ( Delay_Handler, delayNotHit );
    RUN_TEST_CASE ( Delay_Handler, logDelayNotHit );
    RUN_TEST_CASE ( Delay_Handler, liveLogDelayNotHit );
    RUN_TEST_CASE ( Delay_Handler, delayExactHit );
    RUN_TEST_CASE ( Delay_Handler, logDelayExactHit );
    RUN_TEST_CASE ( Delay_Handler, liveLogDelayExactHit );
    RUN_TEST_CASE ( Delay_Handler, delayPassedHit );
    RUN_TEST_CASE ( Delay_Handler, logDelayPassedHit );
    RUN_TEST_CASE ( Delay_Handler, liveLogDelayPassedHit );
    RUN_TEST_CASE ( Delay_Handler, BrightnessDelay );
}