#include "unity_fixture.h"      /* UNITY */
#include "button_handler.h"     /* CUT */
#include "delay_handler.h"      /* CUT */
#include "pwm_handler.h"        /* CUT */
#include "thermistor_handler.h" /* CUT */
#include "bsp.h"                /* CUT */

#define PRESS (0)
#define HOLD  (1)

TEST_GROUP(Requirements);


TEST_SETUP(Requirements)
{
    /* executed before *every* non-skipped test */
    InitWhitePWM();
    InitIRPWM();
}


TEST_TEAR_DOWN(Requirements)
{
    /* executed after *every* non-skipped and non-failing test */
}


/* start requirements tests */
/* TEST CASE 1 */
// SrchLt.01225 When the Controllable Searchlight is initially powered on in the Visible mode, it shall illuminate to the 50% brightness level.
TEST(Requirements, WhiteInitialValues)
{
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS);
    // There are 49 steps from min brightness, find brightness that is halfway in discrete steps
    const uint8_t white_pwm_init = (uint8_t)((MAX_WHITE_PW - MIN_WHITE_PW) / (BRIGHTNESS_STEPS - 1) * HALF_BRIGHTNESS + MIN_WHITE_PW + 0.5f);
    TEST_ASSERT(GetWhitePWM() == white_pwm_init);
}

/* TEST CASE 2 */
// SrchLt.01226 When the Controllable Searchlight is initially powered on in the IR mode, it shall illuminate to the 50% intensity level.
TEST(Requirements, IRInitialValues)
{
    TEST_ASSERT(GetIRBrightness() == HALF_BRIGHTNESS);
    // There are 49 steps from min brightness, find brightness that is halfway in discrete steps
    const uint8_t ir_pwm_init = (uint8_t)((MAX_IR_PW - MIN_IR_PW) / (BRIGHTNESS_STEPS - 1) * HALF_BRIGHTNESS + MIN_IR_PW + 0.5f);
    TEST_ASSERT(GetIRPWM() == ir_pwm_init);
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
    SetWhiteBrightness(MIN_BRIGHTNESS);
    SetIRBrightness(MIN_BRIGHTNESS);

    // Get white brightness diff
    uint8_t whiteBrightness1 = GetWhitePWM();
    IncreaseWhiteBrightness(PRESS);
    uint8_t whiteBrightness2 = GetWhitePWM();
    const uint8_t whiteBrightnessStep = whiteBrightness2 - whiteBrightness1;

    for (uint8_t i = 2; i < BRIGHTNESS_STEPS; i++)
    {
        whiteBrightness1 = whiteBrightness2;
        IncreaseWhiteBrightness(PRESS);
        whiteBrightness2 = GetWhitePWM();
        // Verify step is the same +-1 to deal with float conversion
        TEST_ASSERT(((whiteBrightness2 - whiteBrightness1) >= (whiteBrightnessStep - 1)) &&
                    ((whiteBrightness2 - whiteBrightness1) <= (whiteBrightnessStep + 1)));
    }

    // Get IR brightness diff
    uint8_t irBrightness1 = GetIRPWM();
    IncreaseIRBrightness(PRESS);
    uint8_t irBrightness2 = GetIRPWM();
    const uint8_t irBrightnessStep = irBrightness2 - irBrightness1;

    for (uint8_t i = 2; i < BRIGHTNESS_STEPS; i++)
    {
        irBrightness1 = irBrightness2;
        IncreaseIRBrightness(PRESS);
        irBrightness2 = GetIRPWM();
        // Verify step is the same +-1 to deal with float conversion
        TEST_ASSERT(((irBrightness2 - irBrightness1) >= (irBrightnessStep - 1)) &&
                    ((irBrightness2 - irBrightness1) <= (irBrightnessStep + 1)));
    }
}

/* TEST CASE 5 */
// SrchLt.01233 When the LED lamps are turned off, the searchlight shall remember that last dimming setting in both the Visible and IR modes and return to that previous dimming level the next time the Searchlight is commanded ON.
// SrchLt.01240 The Controllable Searchlight BRT and Dim signals shall have two states, PRESS & HOLD.
TEST(Requirements, RememberLedState)
{
    uint8_t expectedWhite = 0;
    uint8_t expectedIR = 0;
    // Sweep up through all brightness levels with white press
    SetWhiteBrightness(MIN_BRIGHTNESS);
    expectedIR = GetIRBrightness();

    while(GetWhiteBrightness() < MAX_BRIGHTNESS)
    {
        IncreaseWhiteBrightness(PRESS);
        TEST_ASSERT(expectedIR == GetIRBrightness());
    }

    // Sweep up through all brightness levels with white hold
    SetWhiteBrightness(MIN_BRIGHTNESS);
    expectedIR = GetIRBrightness();

    while(GetWhiteBrightness() < MAX_BRIGHTNESS)
    {
        IncreaseWhiteBrightness(HOLD);
        TEST_ASSERT(expectedIR == GetIRBrightness());
    }

    // Sweep up through all brightness levels with white alternating press and hold
    SetWhiteBrightness(MIN_BRIGHTNESS);
    expectedIR = GetIRBrightness();

    while(GetWhiteBrightness() < MAX_BRIGHTNESS)
    {
        IncreaseWhiteBrightness(GetWhiteBrightness() % 2);
        TEST_ASSERT(expectedIR == GetIRBrightness());
    }

    // Sweep up through all brightness levels with ir press
    SetIRBrightness(MIN_BRIGHTNESS);
    expectedWhite = GetWhiteBrightness();

    while(GetIRBrightness() < MAX_BRIGHTNESS)
    {
        IncreaseIRBrightness(PRESS);
        TEST_ASSERT(expectedWhite == GetWhiteBrightness());
    }

    // Sweep up through all brightness levels with ir hold
    SetIRBrightness(MIN_BRIGHTNESS);
    expectedWhite = GetWhiteBrightness();

    while(GetIRBrightness() < MAX_BRIGHTNESS)
    {
        IncreaseIRBrightness(HOLD);
        TEST_ASSERT(expectedWhite == GetWhiteBrightness());
    }

    // Sweep up through all brightness levels with ir alternating press and hold
    SetIRBrightness(MIN_BRIGHTNESS);
    expectedWhite = GetWhiteBrightness();

    while(GetIRBrightness() < MAX_BRIGHTNESS)
    {
        IncreaseIRBrightness(GetIRBrightness() % 2);
        TEST_ASSERT(expectedWhite == GetWhiteBrightness());
    }

    // Sweep down through all brightness levels with white press
    SetWhiteBrightness(MAX_BRIGHTNESS);
    expectedIR = GetIRBrightness();

    while(GetWhiteBrightness() > MIN_BRIGHTNESS)
    {
        DecreaseWhiteBrightness(PRESS);
        TEST_ASSERT(expectedIR == GetIRBrightness());
    }

    // Sweep down through all brightness levels with white hold
    SetWhiteBrightness(MAX_BRIGHTNESS);
    expectedIR = GetIRBrightness();

    while(GetWhiteBrightness() > MIN_BRIGHTNESS)
    {
        DecreaseWhiteBrightness(HOLD);
        TEST_ASSERT(expectedIR == GetIRBrightness());
    }

    // Sweep up through all brightness levels with white alternating press and hold
    SetWhiteBrightness(MAX_BRIGHTNESS);
    expectedIR = GetIRBrightness();

    while(GetWhiteBrightness() > MIN_BRIGHTNESS)
    {
        DecreaseWhiteBrightness(GetWhiteBrightness() % 2);
        TEST_ASSERT(expectedIR == GetIRBrightness());
    }

    // Sweep down through all brightness levels with ir press
    SetIRBrightness(MAX_BRIGHTNESS);
    expectedWhite = GetWhiteBrightness();

    while(GetIRBrightness() > MIN_BRIGHTNESS)
    {
        DecreaseIRBrightness(PRESS);
        TEST_ASSERT(expectedWhite == GetWhiteBrightness());
    }

    // Sweep down through all brightness levels with ir hold
    SetIRBrightness(MAX_BRIGHTNESS);
    expectedWhite = GetWhiteBrightness();

    while(GetIRBrightness() > MIN_BRIGHTNESS)
    {
        DecreaseIRBrightness(HOLD);
        TEST_ASSERT(expectedWhite == GetWhiteBrightness());
    }

    // Sweep down through all brightness levels with ir alternating press and hold
    SetIRBrightness(MAX_BRIGHTNESS);
    expectedWhite = GetWhiteBrightness();

    while(GetIRBrightness() > MIN_BRIGHTNESS)
    {
        DecreaseIRBrightness(GetIRBrightness() % 2);
        TEST_ASSERT(expectedWhite == GetWhiteBrightness());
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
    for (uint8_t i = MIN_BRIGHTNESS; i < BRIGHTNESS_STEPS; i++)
    {
        TEST_ASSERT(BrightnessDelay(i) < 500);
    }
}

/* TEST CASE 7 */
// SrchLt.01242 The HOLD State shall occur when either of the debounced DIM or BRT discrete inputs are at +≥22.0-32.0 VDC (at the input connector) for more than 500 ms. (Input Press Time > 500 ms.)
TEST(Requirements, HoldCheck)
{
    // Ensure two consecutive delay times are greater than 500ms
    uint16_t delay1 = BrightnessDelay(MIN_BRIGHTNESS);

    for (uint8_t i = 1; i < BRIGHTNESS_STEPS; i++)
    {
    uint16_t delay2 = BrightnessDelay(MIN_BRIGHTNESS + i);
    TEST_ASSERT((delay2 + delay1) > 500);
    delay1 = delay2;
}
}

/* TEST CASE 8 */
// SrchLt.01247 In the Visible Mode, when BRT = HOLD, the Visible Luminance and IR Radiance of the Controllable Searchlight shall increase continuously from the stored dimming value until it reaches Maximum brightness.
#define BRIGHTNESS_ITERATIONS (BRIGHTNESS_STEPS * 2) // Step through more than brightness steps to ensure we don't go over MAX_BRIGHTNESS
TEST(Requirements, MaxWhiteBrightnessSweep)
{
    int8_t expectedBrightness;

    // press button case
    expectedBrightness = MIN_BRIGHTNESS;
    SetWhiteBrightness(MIN_BRIGHTNESS);
    for (int i = 0; i < BRIGHTNESS_ITERATIONS; i++)
    {
        expectedBrightness += 1;
        if (expectedBrightness > MAX_BRIGHTNESS) expectedBrightness = MAX_BRIGHTNESS;

        IncreaseWhiteBrightness(PRESS);
        TEST_ASSERT_EQUAL_INT8(expectedBrightness, GetWhiteBrightness());
    }
    // check to see we hit max brightness
    TEST_ASSERT_EQUAL_INT8(MAX_BRIGHTNESS, GetWhiteBrightness());

    // hold button case
    expectedBrightness = MIN_BRIGHTNESS;
    SetWhiteBrightness(MIN_BRIGHTNESS);
    for (int i = 0; i < BRIGHTNESS_ITERATIONS; i++)
    {
        expectedBrightness += HOLD_BRIGHTNESS_JUMP;
        if (expectedBrightness > MAX_BRIGHTNESS) expectedBrightness = MAX_BRIGHTNESS;

        IncreaseWhiteBrightness(HOLD);
        TEST_ASSERT_EQUAL_INT8(expectedBrightness, GetWhiteBrightness());
    }
    // check to see we hit max brightness
    TEST_ASSERT_EQUAL_INT8(MAX_BRIGHTNESS, GetWhiteBrightness());
}

/* TEST CASE 9 */
// SrchLt.01248 In the IR Mode, when BRT = HOLD, the IR Radiance of the Controllable Searchlight shall increase continuously from the stored dimming value until it reaches Maximum brightness.
TEST(Requirements, MaxIRBrightnessSweep)
{
    int8_t expectedBrightness;

    // press button case
    expectedBrightness = MIN_BRIGHTNESS;
    SetIRBrightness(MIN_BRIGHTNESS);
    for (int i = 0; i < BRIGHTNESS_ITERATIONS; i++)
    {
        expectedBrightness += 1;
        if (expectedBrightness > MAX_BRIGHTNESS) expectedBrightness = MAX_BRIGHTNESS;

        IncreaseIRBrightness(PRESS);
        TEST_ASSERT_EQUAL_INT8(expectedBrightness, GetIRBrightness());
    }
    // check to see we hit max brightness
    TEST_ASSERT_EQUAL_INT8(MAX_BRIGHTNESS, GetIRBrightness());

    // hold button case
    expectedBrightness = MIN_BRIGHTNESS;
    SetIRBrightness(MIN_BRIGHTNESS);
    for (int i = 0; i < BRIGHTNESS_ITERATIONS; i++)
    {
        expectedBrightness += HOLD_BRIGHTNESS_JUMP;
        if (expectedBrightness > MAX_BRIGHTNESS) expectedBrightness = MAX_BRIGHTNESS;

        IncreaseIRBrightness(HOLD);
        TEST_ASSERT_EQUAL_INT8(expectedBrightness, GetIRBrightness());
    }
    // check to see we hit max brightness
    TEST_ASSERT_EQUAL_INT8(MAX_BRIGHTNESS, GetIRBrightness());
}

/* TEST CASE 10 */
// SrchLt.01249 In the Visible Mode, when DIM = HOLD, the Visible Luminance and IR Radiance of the Controllable Searchlight shall decrease continuously from the stored dimming value until it reaches Minimum brightness.
TEST(Requirements, MinWhiteBrightnessSweep)
{
    int8_t expectedBrightness;

    // press button case
    expectedBrightness = MAX_BRIGHTNESS;
    SetWhiteBrightness(MAX_BRIGHTNESS);
    for (int i = 0; i < BRIGHTNESS_ITERATIONS; i++)
    {
        expectedBrightness -= 1;
        if (expectedBrightness < MIN_BRIGHTNESS) expectedBrightness = MIN_BRIGHTNESS;

        DecreaseWhiteBrightness(PRESS);
        TEST_ASSERT_EQUAL_INT8(expectedBrightness, GetWhiteBrightness());
    }
    // check to see we hit max brightness
    TEST_ASSERT_EQUAL_INT8(MIN_BRIGHTNESS, GetWhiteBrightness());

    // hold button case
    expectedBrightness = MAX_BRIGHTNESS;
    SetWhiteBrightness(MAX_BRIGHTNESS);
    for (int i = 0; i < BRIGHTNESS_ITERATIONS; i++)
    {
        expectedBrightness -= HOLD_BRIGHTNESS_JUMP;
        if (expectedBrightness < MIN_BRIGHTNESS) expectedBrightness = MIN_BRIGHTNESS;

        DecreaseWhiteBrightness(HOLD);
        TEST_ASSERT_EQUAL_INT8(expectedBrightness, GetWhiteBrightness());
    }
    // check to see we hit max brightness
    TEST_ASSERT_EQUAL_INT8(MIN_BRIGHTNESS, GetWhiteBrightness());
}

/* TEST CASE 11 */
// SrchLt.01250 In the IR Mode, when DIM = HOLD, the IR Radiance of the Controllable Searchlight shall decrease continuously from the stored dimming value until it reaches Minimum brightness.
TEST(Requirements, MinIRBrightnessSweep)
{
    int8_t expectedBrightness;

    // press button case
    expectedBrightness = MAX_BRIGHTNESS;
    SetIRBrightness(MAX_BRIGHTNESS);
    for (int i = 0; i < BRIGHTNESS_ITERATIONS; i++)
    {
        expectedBrightness -= 1;
        if (expectedBrightness < MIN_BRIGHTNESS) expectedBrightness = MIN_BRIGHTNESS;

        DecreaseIRBrightness(PRESS);
        TEST_ASSERT_EQUAL_INT8(expectedBrightness, GetIRBrightness());
    }
    // check to see we hit max brightness
    TEST_ASSERT_EQUAL_INT8(MIN_BRIGHTNESS, GetIRBrightness());

    // hold button case
    expectedBrightness = MAX_BRIGHTNESS;
    SetIRBrightness(MAX_BRIGHTNESS);
    for (int i = 0; i < BRIGHTNESS_ITERATIONS; i++)
    {
        expectedBrightness -= HOLD_BRIGHTNESS_JUMP;
        if (expectedBrightness < MIN_BRIGHTNESS) expectedBrightness = MIN_BRIGHTNESS;

        DecreaseIRBrightness(HOLD);
        TEST_ASSERT_EQUAL_INT8(expectedBrightness, GetIRBrightness());
    }
    // check to see we hit max brightness
    TEST_ASSERT_EQUAL_INT8(MIN_BRIGHTNESS, GetIRBrightness());
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
    start_step = HALF_BRIGHTNESS;
    end_step = MAX_BRIGHTNESS + 1;

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
    start_step = HALF_BRIGHTNESS;
    end_step = MIN_BRIGHTNESS - 1;

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
    start_step = MAX_BRIGHTNESS;
    end_step = MIN_BRIGHTNESS - 1;

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
    start_step = MIN_BRIGHTNESS;
    end_step = MAX_BRIGHTNESS + 1;

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
    RUN_TEST_CASE(Requirements, WhiteInitialValues);
    RUN_TEST_CASE(Requirements, IRInitialValues);
    RUN_TEST_CASE(Requirements, DimmingSteps);
    RUN_TEST_CASE(Requirements, LinearDimming);
    RUN_TEST_CASE(Requirements, RememberLedState);
    RUN_TEST_CASE(Requirements, PressCheck);
    RUN_TEST_CASE(Requirements, HoldCheck);
    RUN_TEST_CASE(Requirements, MaxWhiteBrightnessSweep);
    RUN_TEST_CASE(Requirements, MaxIRBrightnessSweep);
    RUN_TEST_CASE(Requirements, MinWhiteBrightnessSweep);
    RUN_TEST_CASE(Requirements, MinIRBrightnessSweep);
    RUN_TEST_CASE(Requirements, FiftyToOneHundredTheoreticalSweep);
    RUN_TEST_CASE(Requirements, FiftyToZeroTheoreticalSweep);
    RUN_TEST_CASE(Requirements, OneHundredToZeroTheoreticalSweep);
    RUN_TEST_CASE(Requirements, ZeroToOneHundredTheoreticalSweep);
}