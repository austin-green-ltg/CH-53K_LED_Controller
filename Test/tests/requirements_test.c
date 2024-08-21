#include "unity_fixture.h"      /* UNITY */
#include "button_handler.h"     /* CUT */
#include "delay_handler.h"      /* CUT */
#include "pwm_handler.h"        /* CUT */
#include "thermistor_handler.h" /* CUT */
#include "stm32l412xx-bsp.h"    /* CUT */

#define PRESS (0)
#define HOLD  (1)

extern const uint8_t    MinBrightness  ;
extern const uint8_t    MaxBrightness  ;
extern const uint8_t    HalfBrightness ;
extern const float      MinPw    ;
extern const float      MaxPw    ;

TEST_GROUP(Requirements);

TEST_SETUP(Requirements)
{
    /* executed before *every* non-skipped test */
    InitPwm();
}


TEST_TEAR_DOWN(Requirements)
{
    /* executed after *every* non-skipped and non-failing test */
}


/* start requirements tests */
/* TEST CASE 1 */
// SrchLt.01225 When the Controllable Searchlight is initially powered on in the Visible mode, it shall illuminate to the 50% brightness level.
// SrchLt.01226 When the Controllable Searchlight is initially powered on in the IR mode, it shall illuminate to the 50% intensity level.
TEST(Requirements, InitialValues)
{
    TEST_ASSERT(GetBrightness() == HalfBrightness);
    // There are 49 steps from min brightness, find brightness that is halfway in discrete steps
    const uint8_t pwm_init = (uint8_t)((MaxPw - MinPw) / ((float)BRIGHTNESS_STEPS - 1.0f) * HalfBrightness + MinPw + 0.5f);
    TEST_ASSERT_EQUAL_UINT8(pwm_init, GetPwm());
}

/* TEST CASE 3 */
// SrchLt.01229 The Controllable Searchlight shall be configured as step-dimmable.
// SrchLt.01230 The digital dimming control of the Controllable Searchlight shall be a minimum 50 dimming steps from Full-Off to Full-Bright.
TEST(Requirements, DimmingSteps)
{
    TEST_ASSERT(BRIGHTNESS_STEPS == 50);
}

/* TEST CASE 4 */
// SrchLt.01232 The Controllable Searchlight shall dim in a linear fashion for as long as the SRCH DIM switch is depressed or until the minimum Searchlight brightness level has been reached.
// SrchLt.01257 The Controllable Searchlight shall increase in brightness in a linear fashion for as long as the SRCH BRT switch is depressed or until the Maximum Searchlight brightness level has been reached.
TEST(Requirements, LinearDimming)
{
    // Set to initial brightness
    SetBrightness(MinBrightness);

    // Get brightness diff
    uint8_t brightness1 = GetPwm();
    IncreaseBrightness(PRESS);
    uint8_t brightness2 = GetPwm();
    const uint8_t brightnessStep = brightness2 - brightness1;

    for (uint8_t i = 2; i < BRIGHTNESS_STEPS; i++)
    {
        brightness1 = brightness2;
        IncreaseBrightness(PRESS);
        brightness2 = GetPwm();
        // Verify step is the same +-1 to deal with float conversion
        TEST_ASSERT(((brightness2 - brightness1) >= (brightnessStep - 1)) &&
                    ((brightness2 - brightness1) <= (brightnessStep + 1)));
    }
}

/* TEST CASE 5 */
// SrchLt.01233 When the LED lamps are turned off, the searchlight shall remember that last dimming setting in both the Visible and IR modes and return to that previous dimming level the next time the Searchlight is commanded ON.
// SrchLt.01240 The Controllable Searchlight BRT and Dim signals shall have two states, PRESS & HOLD.
TEST(Requirements, RememberLedState)
{
    // Sweep up through all brightness levels with press
    SetBrightness(MinBrightness);

    while(GetBrightness() < MaxBrightness)
    {
        IncreaseBrightness(PRESS);
    }

    // Sweep up through all brightness levels with hold
    SetBrightness(MinBrightness);

    while(GetBrightness() < MaxBrightness)
    {
        IncreaseBrightness(HOLD);
    }

    // Sweep up through all brightness levels with alternating press and hold
    SetBrightness(MinBrightness);

    while(GetBrightness() < MaxBrightness)
    {
        IncreaseBrightness(GetBrightness() % 2);
    }

    // Sweep down through all brightness levels with press
    SetBrightness(MaxBrightness);

    while(GetBrightness() > MinBrightness)
    {
        DecreaseBrightness(PRESS);
    }

    // Sweep down through all brightness levels with hold
    SetBrightness(MaxBrightness);

    while(GetBrightness() > MinBrightness)
    {
        DecreaseBrightness(HOLD);
    }

    // Sweep up through all brightness levels with alternating press and hold
    SetBrightness(MaxBrightness);

    while(GetBrightness() > MinBrightness)
    {
        DecreaseBrightness(GetBrightness() % 2);
    }
}

// SrchLt.01234 The Controllable Searchlight shall brighten when the SRCH BRT switch on the collective grip is depressed.
// No practical test method

// SrchLt.01235 The Controllable Searchlight shall dim when the SRCH DIM switch on the collective grip is depressed.
// No practical test method

/* TEST CASE 6 */
// SrchLt.01241 The PRESS State shall occur when either of the debounced DIM or BRT Discrete Inputs are +≥22.0-32.0 VDC (at the input connector) for less than 500 ms. (Input Debounce Time < Input Press Time <= 500 ms.)
TEST(Requirements, PressCheck)
{
    // Ensure greatest delay time is less than 500ms
    for (uint8_t i = MinBrightness; i < BRIGHTNESS_STEPS; i++)
    {
        TEST_ASSERT(BrightnessDelay(i) < 500);
    }
}

/* TEST CASE 7 */
// SrchLt.01242 The HOLD State shall occur when either of the debounced DIM or BRT discrete inputs are at +≥22.0-32.0 VDC (at the input connector) for more than 500 ms. (Input Press Time > 500 ms.)
TEST(Requirements, HoldCheck)
{
    // Ensure two consecutive delay times are greater than 500ms
    uint16_t delay1 = BrightnessDelay(MinBrightness);

    for (uint8_t i = 1; i < BRIGHTNESS_STEPS; i++)
    {
    uint16_t delay2 = BrightnessDelay(MinBrightness + i);
    TEST_ASSERT((delay2 + delay1) > 500);
    delay1 = delay2;
}
}

/* TEST CASE 8 */
// SrchLt.01247 In the Visible Mode, when BRT = HOLD, the Visible Luminance and IR Radiance of the Controllable Searchlight shall increase continuously from the stored dimming value until it reaches Maximum brightness.
// SrchLt.01248 In the IR Mode, when BRT = HOLD, the IR Radiance of the Controllable Searchlight shall increase continuously from the stored dimming value until it reaches Maximum brightness.
#define BRIGHTNESS_ITERATIONS (BRIGHTNESS_STEPS * 2) // Step through more than brightness steps to ensure we don't go over MaxBrightness
TEST(Requirements, MaxBrightnessSweep)
{
    int8_t expectedBrightness;

    // press button case
    expectedBrightness = MinBrightness;
    SetBrightness(MinBrightness);
    for (int i = 0; i < BRIGHTNESS_ITERATIONS; i++)
    {
        expectedBrightness += 1;
        if (expectedBrightness > MaxBrightness) expectedBrightness = MaxBrightness;

        IncreaseBrightness(PRESS);
        TEST_ASSERT_EQUAL_INT8(expectedBrightness, GetBrightness());
    }
    // check to see we hit max brightness
    TEST_ASSERT_EQUAL_INT8(MaxBrightness, GetBrightness());

    // hold button case
    expectedBrightness = MinBrightness;
    SetBrightness(MinBrightness);
    for (int i = 0; i < BRIGHTNESS_ITERATIONS; i++)
    {
        expectedBrightness += HOLD_BRIGHTNESS_JUMP;
        if (expectedBrightness > MaxBrightness) expectedBrightness = MaxBrightness;

        IncreaseBrightness(HOLD);
        TEST_ASSERT_EQUAL_INT8(expectedBrightness, GetBrightness());
    }
    // check to see we hit max brightness
    TEST_ASSERT_EQUAL_INT8(MaxBrightness, GetBrightness());
}

/* TEST CASE 10 */
// SrchLt.01249 In the Visible Mode, when DIM = HOLD, the Visible Luminance and IR Radiance of the Controllable Searchlight shall decrease continuously from the stored dimming value until it reaches Minimum brightness.
// SrchLt.01250 In the IR Mode, when DIM = HOLD, the IR Radiance of the Controllable Searchlight shall decrease continuously from the stored dimming value until it reaches Minimum brightness.
TEST(Requirements, MinBrightnessSweep)
{
    int8_t expectedBrightness;

    // press button case
    expectedBrightness = MaxBrightness;
    SetBrightness(MaxBrightness);
    for (int i = 0; i < BRIGHTNESS_ITERATIONS; i++)
    {
        expectedBrightness -= 1;
        if (expectedBrightness < MinBrightness) expectedBrightness = MinBrightness;

        DecreaseBrightness(PRESS);
        TEST_ASSERT_EQUAL_INT8(expectedBrightness, GetBrightness());
    }
    // check to see we hit max brightness
    TEST_ASSERT_EQUAL_INT8(MinBrightness, GetBrightness());

    // hold button case
    expectedBrightness = MaxBrightness;
    SetBrightness(MaxBrightness);
    for (int i = 0; i < BRIGHTNESS_ITERATIONS; i++)
    {
        expectedBrightness -= HOLD_BRIGHTNESS_JUMP;
        if (expectedBrightness < MinBrightness) expectedBrightness = MinBrightness;

        DecreaseBrightness(HOLD);
        TEST_ASSERT_EQUAL_INT8(expectedBrightness, GetBrightness());
    }
    // check to see we hit max brightness
    TEST_ASSERT_EQUAL_INT8(MinBrightness, GetBrightness());
}

/* TEST CASE 12 */
// SrchLt.01251 In the Visible Mode, the Controllable Searchlight dimmer shall brighten from the initial 50% to Full Bright in 4.5 +/-0.5 seconds.
// SrchLt.01252 In the IR Mode, the Controllable Searchlight dimmer shall brighten from the initial 50% to Full Bright in 4.5 +/-0.5 seconds.
TEST(Requirements, FiftyToOneHundredTheoreticalSweep)
{
    const uint32_t fifty_to_onehundred_time_ms = 4500; // 4.5 seconds
    const uint32_t fifty_to_onehundred_ff_ms = 500;    // +- 0.5 seconds

    // formula is step + 3*step*(num_steps-1)
    // initial step from pressing the button, then sweep
    int8_t start_step;
    int8_t end_step;
    // 50% -> 100%
    // 24 -> 49, 25 steps
    start_step = HalfBrightness;
    end_step = MaxBrightness + 1;

    uint32_t sweep_fifty_to_onehundred_time_ms = BrightnessDelay(start_step);

    for (int8_t i = start_step + 1 ; i < end_step; i+=3)
    {
        sweep_fifty_to_onehundred_time_ms += BrightnessDelay(i);
    }

    TEST_ASSERT(sweep_fifty_to_onehundred_time_ms <= fifty_to_onehundred_time_ms + fifty_to_onehundred_ff_ms);
    TEST_ASSERT(sweep_fifty_to_onehundred_time_ms >= fifty_to_onehundred_time_ms - fifty_to_onehundred_ff_ms);
}

/* TEST CASE 13 */
// SrchLt.01253 In the Visible Mode, the Controllable Searchlight dimmer shall dim from 50% to OFF in 3.25 +/-0.5 seconds.
// SrchLt.01254 In the IR Mode, the Controllable Searchlight dimmer shall dim from 50% to OFF in 3.25 +/-0.5 seconds.
TEST(Requirements, FiftyToZeroTheoreticalSweep)
{
    const uint32_t fifty_to_zero_time_ms = 3250; // 3.25 seconds
    const uint32_t fifty_to_zero_ff_ms = 500;    // +- 0.5 seconds

    // formula is step + 3*step*(num_steps-1)
    // initial step from pressing the button, then sweep
    int8_t start_step;
    int8_t end_step;

    // 50% -> 0%
    // 24 -> 0, 24 steps
    start_step = HalfBrightness;
    end_step = MinBrightness - 1;

    uint32_t sweep_fifty_to_zero_time_ms = BrightnessDelay(start_step);

    for (int8_t i = start_step - 1 ; i > end_step; i-=3)
    {
        sweep_fifty_to_zero_time_ms += BrightnessDelay(i);
    }

    TEST_ASSERT(sweep_fifty_to_zero_time_ms <= fifty_to_zero_time_ms + fifty_to_zero_ff_ms);
    TEST_ASSERT(sweep_fifty_to_zero_time_ms >= fifty_to_zero_time_ms - fifty_to_zero_ff_ms);
}

/* TEST CASE 14 */
// SrchLt.01255 In the Visible Mode, the Controllable Searchlight shall dim from FULL Bright to Minimum in 6 +/-1.5 seconds.
TEST(Requirements, OneHundredToZeroTheoreticalSweep)
{
    const uint32_t onehundred_to_zero_time_ms = 6000; // 6 seconds
    const uint32_t onehundred_to_zero_ff_ms = 1500;    // +- 1.5 seconds

    // formula is step + 3*step*(num_steps-1)
    // initial step from pressing the button, then sweep
    int8_t start_step;
    int8_t end_step;

    // 100% -> 0%
    // 49 -> 0, 49 steps
    start_step = MaxBrightness;
    end_step = MinBrightness - 1;

    uint32_t sweep_onehundred_to_zero_time_ms = BrightnessDelay(start_step);

    for (int8_t i = start_step - 1 ; i > end_step; i-=3)
    {
        sweep_onehundred_to_zero_time_ms += BrightnessDelay(i);
    }

    TEST_ASSERT(sweep_onehundred_to_zero_time_ms <= onehundred_to_zero_time_ms + onehundred_to_zero_ff_ms);
    TEST_ASSERT(sweep_onehundred_to_zero_time_ms >= onehundred_to_zero_time_ms - onehundred_to_zero_ff_ms);
}

/* TEST CASE 15 */
// SrchLt.01256 In the IR Mode, the Controllable Searchlight shall brighten from Minimum to Full Bright in 6 +/-1.5 seconds.
TEST(Requirements, ZeroToOneHundredTheoreticalSweep)
{
    const uint32_t zero_to_onehundred_time_ms = 6000; // 6 seconds
    const uint32_t zero_to_onehundred_ff_ms = 1500;    // +- 1.5 seconds

    // formula is step + 3*step*(num_steps-1)
    // initial step from pressing the button, then sweep
    int8_t start_step;
    int8_t end_step;

    // 0% -> 100%
    // 0 -> 49, 49 steps
    start_step = MinBrightness;
    end_step = MaxBrightness + 1;

    uint32_t sweep_zero_to_onehundred_time_ms = BrightnessDelay(start_step);

    for (int8_t i = start_step + 1 ; i < end_step; i+=3)
    {
        sweep_zero_to_onehundred_time_ms += BrightnessDelay(i);
    }

    TEST_ASSERT(sweep_zero_to_onehundred_time_ms <= zero_to_onehundred_time_ms + zero_to_onehundred_ff_ms);
    TEST_ASSERT(sweep_zero_to_onehundred_time_ms >= zero_to_onehundred_time_ms - zero_to_onehundred_ff_ms);
}
/* end requirements tests */


TEST_GROUP_RUNNER(Requirements)
{
    RUN_TEST_CASE(Requirements, InitialValues);
    RUN_TEST_CASE(Requirements, DimmingSteps);
    RUN_TEST_CASE(Requirements, LinearDimming);
    RUN_TEST_CASE(Requirements, RememberLedState);
    RUN_TEST_CASE(Requirements, PressCheck);
    RUN_TEST_CASE(Requirements, HoldCheck);
    RUN_TEST_CASE(Requirements, MaxBrightnessSweep);
    RUN_TEST_CASE(Requirements, MinBrightnessSweep);
    RUN_TEST_CASE(Requirements, FiftyToOneHundredTheoreticalSweep);
    RUN_TEST_CASE(Requirements, FiftyToZeroTheoreticalSweep);
    RUN_TEST_CASE(Requirements, OneHundredToZeroTheoreticalSweep);
    RUN_TEST_CASE(Requirements, ZeroToOneHundredTheoreticalSweep);
}