/*============================================================================
  * ET: embedded test; very simple test example
============================================================================*/
// #include "pwm_handler.h"
#include "unity_fixture.h"
#include "button_handler.h"  /* CUT */
#include "delay_handler.h"  /* CUT */
#include "pwm_handler.h"    /* CUT */
#include "thermistor_handler.h"    /* CUT */
#include <stdio.h>

#define PRESS (0)
#define HOLD  (1)

TEST_GROUP(Requirements);

TEST_SETUP(Requirements)
{
  /* executed before *every* non-skipped test */
  initWhitePWM();
  initIRPWM();
}

TEST_TEAR_DOWN(Requirements)
{
  /* executed after *every* non-skipped and non-failing test */
}

/* start requirements tests */
// Init at 50% brightness
// Verify Initial Values
TEST(Requirements, InitialValues)
{
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS);
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS);
  // There are 49 steps from min brightness, find brightness that is halfway in discrete steps
  const uint8_t white_pwm_init = (MAX_WHITE_PW - MIN_WHITE_PW) / (BRIGHTNESS_STEPS - 1) * HALF_BRIGHTNESS + MIN_WHITE_PW + 0.5f;
  const uint8_t ir_pwm_init =    (MAX_IR_PW - MIN_IR_PW)       / (BRIGHTNESS_STEPS - 1) * HALF_BRIGHTNESS + MIN_IR_PW + 0.5f;
  TEST_ASSERT(getWhitePWM() == white_pwm_init);
  TEST_ASSERT(getIRPWM()    == ir_pwm_init);
}

// 50 Dimming steps
TEST(Requirements, DimmingSteps)
{
  TEST_ASSERT(BRIGHTNESS_STEPS == 50);
}

// Linear Dimming
TEST(Requirements, LinearDimming)
{
  // Set to initial brightness
  setWhiteBrightness(MIN_BRIGHTNESS);
  setIRBrightness(MIN_BRIGHTNESS);

  // Get white brightness diff
  uint8_t whiteBrightness1 = getWhitePWM();
  increaseWhiteBrightness(PRESS);
  uint8_t whiteBrightness2 = getWhitePWM();
  const uint8_t whiteBrightnessStep = whiteBrightness2 - whiteBrightness1;

  for (uint8_t i = 2; i < BRIGHTNESS_STEPS; i++)
  {
    whiteBrightness1 = whiteBrightness2;
    increaseWhiteBrightness(PRESS);
    whiteBrightness2 = getWhitePWM();
    // Verify step is the same +-1 to deal with float conversion
    TEST_ASSERT(((whiteBrightness2 - whiteBrightness1) >= (whiteBrightnessStep - 1)) &&
    ((whiteBrightness2 - whiteBrightness1) <= (whiteBrightnessStep + 1)));
  }

  // Get IR brightness diff
  uint8_t irBrightness1 = getIRPWM();
  increaseIRBrightness(PRESS);
  uint8_t irBrightness2 = getIRPWM();
  const uint8_t irBrightnessStep = irBrightness2 - irBrightness1;

  for (uint8_t i = 2; i < BRIGHTNESS_STEPS; i++)
  {
    irBrightness1 = irBrightness2;
    increaseIRBrightness(PRESS);
    irBrightness2 = getIRPWM();
    // Verify step is the same +-1 to deal with float conversion
    TEST_ASSERT(((irBrightness2 - irBrightness1) >= (irBrightnessStep - 1)) &&
    ((irBrightness2 - irBrightness1) <= (irBrightnessStep + 1)));
  }

}

// Remember led states
TEST(Requirements, RememberLedState)
{
  uint8_t expectedWhite = 0;
  uint8_t expectedIR = 0;
  // Sweep up through all brightness levels with white press
  setWhiteBrightness(MIN_BRIGHTNESS);
  expectedIR = getIRBrightness();

  while(getWhiteBrightness() < MAX_BRIGHTNESS)
  {
    increaseWhiteBrightness(PRESS);
    TEST_ASSERT(expectedIR == getIRBrightness());
  }

  // Sweep up through all brightness levels with white hold
  setWhiteBrightness(MIN_BRIGHTNESS);
  expectedIR = getIRBrightness();

  while(getWhiteBrightness() < MAX_BRIGHTNESS)
  {
    increaseWhiteBrightness(HOLD);
    TEST_ASSERT(expectedIR == getIRBrightness());
  }

  // Sweep up through all brightness levels with white alternating press and hold
  setWhiteBrightness(MIN_BRIGHTNESS);
  expectedIR = getIRBrightness();

  while(getWhiteBrightness() < MAX_BRIGHTNESS)
  {
    increaseWhiteBrightness(getWhiteBrightness() % 2);
    TEST_ASSERT(expectedIR == getIRBrightness());
  }

  // Sweep up through all brightness levels with ir press
  setIRBrightness(MIN_BRIGHTNESS);
  expectedWhite = getWhiteBrightness();

  while(getIRBrightness() < MAX_BRIGHTNESS)
  {
    increaseIRBrightness(PRESS);
    TEST_ASSERT(expectedWhite == getWhiteBrightness());
  }

  // Sweep up through all brightness levels with ir hold
  setIRBrightness(MIN_BRIGHTNESS);
  expectedWhite = getWhiteBrightness();

  while(getIRBrightness() < MAX_BRIGHTNESS)
  {
    increaseIRBrightness(HOLD);
    TEST_ASSERT(expectedWhite == getWhiteBrightness());
  }

  // Sweep up through all brightness levels with ir alternating press and hold
  setIRBrightness(MIN_BRIGHTNESS);
  expectedWhite = getWhiteBrightness();

  while(getIRBrightness() < MAX_BRIGHTNESS)
  {
    increaseIRBrightness(getIRBrightness() % 2);
    TEST_ASSERT(expectedWhite == getWhiteBrightness());
  }

  // Sweep down through all brightness levels with white press
  setWhiteBrightness(MAX_BRIGHTNESS);
  expectedIR = getIRBrightness();

  while(getWhiteBrightness() > MIN_BRIGHTNESS)
  {
    decreaseWhiteBrightness(PRESS);
    TEST_ASSERT(expectedIR == getIRBrightness());
  }

  // Sweep down through all brightness levels with white hold
  setWhiteBrightness(MAX_BRIGHTNESS);
  expectedIR = getIRBrightness();

  while(getWhiteBrightness() > MIN_BRIGHTNESS)
  {
    decreaseWhiteBrightness(HOLD);
    TEST_ASSERT(expectedIR == getIRBrightness());
  }

  // Sweep up through all brightness levels with white alternating press and hold
  setWhiteBrightness(MAX_BRIGHTNESS);
  expectedIR = getIRBrightness();

  while(getWhiteBrightness() > MIN_BRIGHTNESS)
  {
    decreaseWhiteBrightness(getWhiteBrightness() % 2);
    TEST_ASSERT(expectedIR == getIRBrightness());
  }

  // Sweep down through all brightness levels with ir press
  setIRBrightness(MAX_BRIGHTNESS);
  expectedWhite = getWhiteBrightness();

  while(getIRBrightness() > MIN_BRIGHTNESS)
  {
    decreaseIRBrightness(PRESS);
    TEST_ASSERT(expectedWhite == getWhiteBrightness());
  }

  // Sweep down through all brightness levels with ir hold
  setIRBrightness(MAX_BRIGHTNESS);
  expectedWhite = getWhiteBrightness();

  while(getIRBrightness() > MIN_BRIGHTNESS)
  {
    decreaseIRBrightness(HOLD);
    TEST_ASSERT(expectedWhite == getWhiteBrightness());
  }

  // Sweep down through all brightness levels with ir alternating press and hold
  setIRBrightness(MAX_BRIGHTNESS);
  expectedWhite = getWhiteBrightness();

  while(getIRBrightness() > MIN_BRIGHTNESS)
  {
    decreaseIRBrightness(getIRBrightness() % 2);
    TEST_ASSERT(expectedWhite == getWhiteBrightness());
  }
}

// Bright and Dim have press and hold states
// Press state occurs when debounced pressed for less than 500ms
// Hold state occurs when debounced pressed for more than 500ms
TEST(Requirements, HoldCheck)
{
  // Ensure greatest delay time is less than 500ms
  for (uint8_t i = MIN_BRIGHTNESS; i < BRIGHTNESS_STEPS; i++)
  {
    TEST_ASSERT(brightnessDelay(i) < 500);
  }

  // Ensure two consecutive delay times are greater than 500ms
  uint16_t delay1 = brightnessDelay(MIN_BRIGHTNESS);

  for (uint8_t i = 1; i < BRIGHTNESS_STEPS; i++)
  {
    uint16_t delay2 = brightnessDelay(MIN_BRIGHTNESS + i);
    TEST_ASSERT((delay2 + delay1) > 500);
    delay1 = delay2;
  }
}

// Sweep
// 50% to full bright in 4.5 +- 0.5 seconds
// 50% to full dim in 3.25 +- 0.5 seconds
// 100% to 0% in 6 +- 1.5 seconds
// 0% to 100% in 6 +- 1.5 seconds// This test ensures that the sweep time is within spec
TEST(Requirements, TheoreticalSweep)
{
  const uint32_t fifty_to_onehundred_time_ms = 4500; // 4.5 seconds
  const uint32_t fifty_to_onehundred_ff_ms = 500;    // +- 0.5 seconds
  const uint32_t fifty_to_zero_time_ms = 3250; // 3.25 seconds
  const uint32_t fifty_to_zero_ff_ms = 500;    // +- 0.5 seconds
  const uint32_t zero_to_onehundred_time_ms = 6000; // 6 seconds
  const uint32_t zero_to_onehundred_ff_ms = 1500;    // +- 1.5 seconds
  const uint32_t onehundred_to_zero_time_ms = 6000; // 6 seconds
  const uint32_t onehundred_to_zero_ff_ms = 1500;    // +- 1.5 seconds

  // formula is step + 3*step*(num_steps-1)
  // initial step from pressing the button, then sweep
  int8_t start_step;
  int8_t end_step;
  // 50% -> 100%
  // 24 -> 49, 25 steps
  {
    start_step = HALF_BRIGHTNESS;
    end_step = MAX_BRIGHTNESS + 1;

    uint32_t sweep_fifty_to_onehundred_time_ms = brightnessDelay(start_step);

    for (int8_t i = start_step + 1 ; i < end_step; i+=3)
    {
      sweep_fifty_to_onehundred_time_ms += brightnessDelay(i);
    }


    TEST_ASSERT(sweep_fifty_to_onehundred_time_ms <= fifty_to_onehundred_time_ms + fifty_to_onehundred_ff_ms);
    TEST_ASSERT(sweep_fifty_to_onehundred_time_ms >= fifty_to_onehundred_time_ms - fifty_to_onehundred_ff_ms);
  }

  // 50% -> 0%
  // 24 -> 0, 24 steps
  {
    start_step = HALF_BRIGHTNESS;
    end_step = MIN_BRIGHTNESS - 1;

    uint32_t sweep_fifty_to_zero_time_ms = brightnessDelay(start_step);

    for (int8_t i = start_step - 1 ; i > end_step; i-=3)
    {
      sweep_fifty_to_zero_time_ms += brightnessDelay(i);
    }

    TEST_ASSERT(sweep_fifty_to_zero_time_ms <= fifty_to_zero_time_ms + fifty_to_zero_ff_ms);
    TEST_ASSERT(sweep_fifty_to_zero_time_ms >= fifty_to_zero_time_ms - fifty_to_zero_ff_ms);
  }

  // 0% -> 100%
  // 0 -> 49, 49 steps
  {
    start_step = MIN_BRIGHTNESS;
    end_step = MAX_BRIGHTNESS + 1;

    uint32_t sweep_zero_to_onehundred_time_ms = brightnessDelay(start_step);

    for (int8_t i = start_step + 1 ; i < end_step; i+=3)
    {
      sweep_zero_to_onehundred_time_ms += brightnessDelay(i);
    }

    TEST_ASSERT(sweep_zero_to_onehundred_time_ms <= zero_to_onehundred_time_ms + zero_to_onehundred_ff_ms);
    TEST_ASSERT(sweep_zero_to_onehundred_time_ms >= zero_to_onehundred_time_ms - zero_to_onehundred_ff_ms);
  }

  // 100% -> 0%
  // 49 -> 0, 49 steps
  {
    start_step = MAX_BRIGHTNESS;
    end_step = MIN_BRIGHTNESS - 1;

    uint32_t sweep_onehundred_to_zero_time_ms = brightnessDelay(start_step);

    for (int8_t i = start_step - 1 ; i > end_step; i-=3)
    {
      sweep_onehundred_to_zero_time_ms += brightnessDelay(i);
    }

    TEST_ASSERT(sweep_onehundred_to_zero_time_ms <= onehundred_to_zero_time_ms + onehundred_to_zero_ff_ms);
    TEST_ASSERT(sweep_onehundred_to_zero_time_ms >= onehundred_to_zero_time_ms - onehundred_to_zero_ff_ms);
  }
}
/* end requirements tests */

TEST_GROUP_RUNNER(Requirements)
{
  RUN_TEST_CASE(Requirements, InitialValues);
  RUN_TEST_CASE(Requirements, DimmingSteps);
  RUN_TEST_CASE(Requirements, LinearDimming);
  RUN_TEST_CASE(Requirements, RememberLedState);
  RUN_TEST_CASE(Requirements, HoldCheck);
  RUN_TEST_CASE(Requirements, TheoreticalSweep);
}