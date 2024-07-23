/*============================================================================
  * ET: embedded test; very simple test example
============================================================================*/
// #include "pwm_handler.h"
#include "unity_fixture.h"
#include "pwm_handler.h"    /* CUT */
#include "thermistor_handler.h"    /* CUT */
#include "bsp.h"    /* CUT */
#include <stdio.h>

#define PRESS (0)
#define HOLD  (1)

#define HEATING_WARM_THERM (100000)
#define HEATING_HOT_THERM  (120000)
#define COOLING_COOL_THERM (80000)
#define COOLING_WARM_THERM (100000)

extern int32_t thermistor_value;

TEST_GROUP(PWM_Handler);

TEST_SETUP(PWM_Handler)
{
  /* executed before *every* non-skipped test */
  initWhitePWM();
  initIRPWM();
}

TEST_TEAR_DOWN(PWM_Handler)
{
  /* executed after *every* non-skipped and non-failing test */
  thermistor_value = 0; // return to default value
  get_temperature_range(); // set thermistor range
}

/* start pwm_handler tests */
/* NICE CASES */
// verify nice case of decreaseWhiteBrightness
TEST(PWM_Handler, decreaseWhiteBrightnessNiceCase)
{
  extern pwmStruct pwm_white;
  extern pwmStruct pwm_ir;

  decreaseWhiteBrightness(PRESS);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS - 1);
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
  TEST_ASSERT(pwm_ir.running == 0);

  decreaseWhiteBrightness(HOLD);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS - 1 - HOLD_BRIGHTNESS_JUMP);
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
  TEST_ASSERT(pwm_ir.running == 0);

  initWhitePWM();
  initIRPWM();

  decreaseWhiteBrightness(HOLD);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS - HOLD_BRIGHTNESS_JUMP);
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change

  TEST_ASSERT(pwm_ir.running == 0);
  decreaseWhiteBrightness(PRESS);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS - HOLD_BRIGHTNESS_JUMP - 1);
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
  TEST_ASSERT(pwm_ir.running == 0);
}

// verify nice case of decreaseIRBrightness
TEST(PWM_Handler, decreaseIRBrightnessNiceCase)
{
  extern pwmStruct pwm_white;
  extern pwmStruct pwm_ir;

  decreaseIRBrightness(PRESS);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
  TEST_ASSERT(pwm_white.running == 0);
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS - 1);
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());

  decreaseIRBrightness(HOLD);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
  TEST_ASSERT(pwm_white.running == 0);
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS - 1 - HOLD_BRIGHTNESS_JUMP);
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());

  initWhitePWM();
  initIRPWM();

  decreaseIRBrightness(HOLD);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
  TEST_ASSERT(pwm_white.running == 0);
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS - HOLD_BRIGHTNESS_JUMP);
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());

  decreaseIRBrightness(PRESS);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
  TEST_ASSERT(pwm_white.running == 0);
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS - HOLD_BRIGHTNESS_JUMP - 1);
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());
}

// verify nice case of increaseWhiteBrightness
TEST(PWM_Handler, increaseWhiteBrightnessNiceCase)
{
  extern pwmStruct pwm_white;
  extern pwmStruct pwm_ir;

  increaseWhiteBrightness(PRESS);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS + 1);
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
  TEST_ASSERT(pwm_ir.running == 0);

  increaseWhiteBrightness(HOLD);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS + 1 + HOLD_BRIGHTNESS_JUMP);
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
  TEST_ASSERT(pwm_ir.running == 0);

  initWhitePWM();
  initIRPWM();

  increaseWhiteBrightness(HOLD);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS + HOLD_BRIGHTNESS_JUMP);
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
  TEST_ASSERT(pwm_ir.running == 0);

  increaseWhiteBrightness(PRESS);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS + HOLD_BRIGHTNESS_JUMP + 1);
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
  TEST_ASSERT(pwm_ir.running == 0);
}

// verify nice case of increaseIRBrightness
TEST(PWM_Handler, increaseIRBrightnessNiceCase)
{
  extern pwmStruct pwm_white;
  extern pwmStruct pwm_ir;

  increaseIRBrightness(PRESS);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
  TEST_ASSERT(pwm_white.running == 0);
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS + 1);
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());

  increaseIRBrightness(HOLD);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
  TEST_ASSERT(pwm_white.running == 0);
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS + 1 + HOLD_BRIGHTNESS_JUMP);
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());

  initWhitePWM();
  initIRPWM();

  increaseIRBrightness(HOLD);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
  TEST_ASSERT(pwm_white.running == 0);
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS + HOLD_BRIGHTNESS_JUMP);
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());

  increaseIRBrightness(PRESS);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
  TEST_ASSERT(pwm_white.running == 0);
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS + HOLD_BRIGHTNESS_JUMP + 1);
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());
}

// verify nice case of setWhitePWM
TEST(PWM_Handler, setWhitePWMNiceCase)
{
  extern pwmStruct pwm_white;
  extern pwmStruct pwm_ir;

  setWhitePWM();
  // White should be on and be set to white pwm
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  // IR should be off
  TEST_ASSERT(pwm_ir.running == 0);
}

// verify nice case of setIRPWM
TEST(PWM_Handler, setIRPWMNiceCase)
{
  extern pwmStruct pwm_white;
  extern pwmStruct pwm_ir;

  setIRPWM();
  // IR should be on and be set to IR pwm
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());
  // White should be off
  TEST_ASSERT(pwm_white.running == 0);
}

// verify nice case of turnOffWhitePWM
TEST(PWM_Handler, turnOffWhitePWMNiceCase)
{
  extern pwmStruct pwm_white;
  extern pwmStruct pwm_ir;

  setWhitePWM();
  turnOffWhitePWM();
  // White should be off
  TEST_ASSERT(pwm_white.running == 0);
}

// verify nice case of turnOffIRPWM
TEST(PWM_Handler, turnOffIRPWMNiceCase)
{
  extern pwmStruct pwm_white;
  extern pwmStruct pwm_ir;

  setIRPWM();
  turnOffIRPWM();
  // IR should be off
  TEST_ASSERT(pwm_ir.running == 0);
}

// verify nice case of setWhiteBrightness
TEST(PWM_Handler, setWhiteBrightnessNiceCase)
{
  int8_t brightness = MIN_BRIGHTNESS;
  setWhiteBrightness(brightness);
  TEST_ASSERT(getWhiteBrightness() == brightness);
  brightness = MAX_BRIGHTNESS;
  setWhiteBrightness(brightness);
  TEST_ASSERT(getWhiteBrightness() == brightness);
  brightness = HALF_BRIGHTNESS;
  setWhiteBrightness(brightness);
  TEST_ASSERT(getWhiteBrightness() == brightness);
  brightness = 32;
  setWhiteBrightness(brightness);
  TEST_ASSERT(getWhiteBrightness() == brightness);
  brightness = 16;
  setWhiteBrightness(brightness);
  TEST_ASSERT(getWhiteBrightness() == brightness);
}

// verify nice case of setIRBrightness
TEST(PWM_Handler, setIRBrightnessNiceCase)
{
  int8_t brightness = MIN_BRIGHTNESS;
  setIRBrightness(brightness);
  TEST_ASSERT(getIRBrightness() == brightness);
  brightness = MAX_BRIGHTNESS;
  setIRBrightness(brightness);
  TEST_ASSERT(getIRBrightness() == brightness);
  brightness = HALF_BRIGHTNESS;
  setIRBrightness(brightness);
  TEST_ASSERT(getIRBrightness() == brightness);
  brightness = 32;
  setIRBrightness(brightness);
  TEST_ASSERT(getIRBrightness() == brightness);
  brightness = 16;
  setIRBrightness(brightness);
  TEST_ASSERT(getIRBrightness() == brightness);
}

// verify nice case of getWhitePWM
TEST(PWM_Handler, getWhitePWMNiceCase)
{
  const float white_pwm_steps = (MAX_WHITE_PW - MIN_WHITE_PW) / (BRIGHTNESS_STEPS - 1);
  int8_t brightness = MIN_BRIGHTNESS;
  uint8_t expected_pwm = ((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
  setWhiteBrightness(brightness);
  TEST_ASSERT(getWhitePWM() == expected_pwm);
  brightness = MAX_BRIGHTNESS;
  expected_pwm = ((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
  setWhiteBrightness(brightness);
  TEST_ASSERT(getWhitePWM() == expected_pwm);
  brightness = HALF_BRIGHTNESS;
  expected_pwm = ((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
  setWhiteBrightness(brightness);
  TEST_ASSERT(getWhitePWM() == expected_pwm);
  brightness = 17;
  expected_pwm = ((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
  setWhiteBrightness(brightness);
  TEST_ASSERT(getWhitePWM() == expected_pwm);
  brightness = 42;
  expected_pwm = ((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
  setWhiteBrightness(brightness);
  TEST_ASSERT(getWhitePWM() == expected_pwm);
}

// verify nice case of getIRPWM
TEST(PWM_Handler, getIRPWMNiceCase)
{
  const float IR_pwm_steps = (MAX_IR_PW - MIN_IR_PW) / (BRIGHTNESS_STEPS - 1);
  int8_t brightness = MIN_BRIGHTNESS;
  uint8_t expected_pwm = ((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
  setIRBrightness(brightness);
  TEST_ASSERT(getIRPWM() == expected_pwm);
  brightness = MAX_BRIGHTNESS;
  expected_pwm = ((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
  setIRBrightness(brightness);
  TEST_ASSERT(getIRPWM() == expected_pwm);
  brightness = HALF_BRIGHTNESS;
  expected_pwm = ((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
  setIRBrightness(brightness);
  TEST_ASSERT(getIRPWM() == expected_pwm);
  brightness = 12;
  expected_pwm = ((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
  setIRBrightness(brightness);
  TEST_ASSERT(getIRPWM() == expected_pwm);
  brightness = 39;
  expected_pwm = ((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
  setIRBrightness(brightness);
  TEST_ASSERT(getIRPWM() == expected_pwm);
}

// verify nice case of getWhitePWM
TEST(PWM_Handler, getWhitePWMThermistorNiceCase)
{
  const float white_pwm_steps = (MAX_WHITE_PW - MIN_WHITE_PW) / (BRIGHTNESS_STEPS - 1);
  const int8_t brightness = MAX_BRIGHTNESS;
  const uint8_t expected_pwm = ((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
  const uint8_t expected_cool_pwm = expected_pwm;
  const uint8_t expected_warm_pwm = (uint8_t)(expected_pwm * WARM_PWM_RATIO);
  const uint8_t expected_hot_pwm  = (uint8_t)(expected_pwm * HOT_PWM_RATIO);
  setWhiteBrightness(brightness);

  thermistor_value = HEATING_WARM_THERM;
  TEST_ASSERT(getWhitePWM() == expected_warm_pwm);

  thermistor_value = HEATING_HOT_THERM;
  TEST_ASSERT(getWhitePWM() == expected_hot_pwm);

  thermistor_value = COOLING_COOL_THERM;
  TEST_ASSERT(getWhitePWM() == expected_cool_pwm);
}

// verify nice case of getIRPWM
TEST(PWM_Handler, getIRPWMThermistorNiceCase)
{
  const float IR_pwm_steps = (MAX_IR_PW - MIN_IR_PW) / (BRIGHTNESS_STEPS - 1);
  const int8_t brightness = MAX_BRIGHTNESS;
  const uint8_t expected_pwm = ((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
  const uint8_t expected_cool_pwm = expected_pwm;
  const uint8_t expected_warm_pwm = (uint8_t)(expected_pwm * WARM_PWM_RATIO);
  const uint8_t expected_hot_pwm  = (uint8_t)(expected_pwm * HOT_PWM_RATIO);
  setIRBrightness(brightness);

  thermistor_value = HEATING_WARM_THERM;
  TEST_ASSERT(getIRPWM() == expected_warm_pwm);

  thermistor_value = HEATING_HOT_THERM;
  TEST_ASSERT(getIRPWM() == expected_hot_pwm);

  thermistor_value = COOLING_COOL_THERM;
  TEST_ASSERT(getIRPWM() == expected_cool_pwm);
}

/* EDGE CASES */
// verify edge case of decreaseWhiteBrightness
TEST(PWM_Handler, decreaseWhiteBrightnessEdgeCase)
{
  extern pwmStruct pwm_white;
  extern pwmStruct pwm_ir;

  setWhiteBrightness(MIN_BRIGHTNESS);
  decreaseWhiteBrightness(PRESS);
  TEST_ASSERT(getWhiteBrightness() == MIN_BRIGHTNESS);
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
  TEST_ASSERT(pwm_ir.running == 0);

  decreaseWhiteBrightness(HOLD);
  TEST_ASSERT(getWhiteBrightness() == MIN_BRIGHTNESS);
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
  TEST_ASSERT(pwm_ir.running == 0);

  initWhitePWM();
  initIRPWM();

  setWhiteBrightness(MIN_BRIGHTNESS);
  decreaseWhiteBrightness(HOLD);
  TEST_ASSERT(getWhiteBrightness() == MIN_BRIGHTNESS);
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change

  TEST_ASSERT(pwm_ir.running == 0);
  decreaseWhiteBrightness(PRESS);
  TEST_ASSERT(getWhiteBrightness() == MIN_BRIGHTNESS);
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
  TEST_ASSERT(pwm_ir.running == 0);
}

// verify edge case of decreaseIRBrightness
TEST(PWM_Handler, decreaseIRBrightnessEdgeCase)
{
  extern pwmStruct pwm_white;
  extern pwmStruct pwm_ir;

  setIRBrightness(MIN_BRIGHTNESS);
  decreaseIRBrightness(PRESS);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
  TEST_ASSERT(pwm_white.running == 0);
  TEST_ASSERT(getIRBrightness()    == MIN_BRIGHTNESS);
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());

  decreaseIRBrightness(HOLD);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
  TEST_ASSERT(pwm_white.running == 0);
  TEST_ASSERT(getIRBrightness()    == MIN_BRIGHTNESS);
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());

  initWhitePWM();
  initIRPWM();

  setIRBrightness(MIN_BRIGHTNESS);
  decreaseIRBrightness(HOLD);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
  TEST_ASSERT(pwm_white.running == 0);
  TEST_ASSERT(getIRBrightness()    == MIN_BRIGHTNESS);
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());

  decreaseIRBrightness(PRESS);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
  TEST_ASSERT(pwm_white.running == 0);
  TEST_ASSERT(getIRBrightness()    == MIN_BRIGHTNESS);
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());
}

// verify edge case of increaseWhiteBrightness
TEST(PWM_Handler, increaseWhiteBrightnessEdgeCase)
{
  extern pwmStruct pwm_white;
  extern pwmStruct pwm_ir;

  setWhiteBrightness(MAX_BRIGHTNESS);
  increaseWhiteBrightness(PRESS);
  TEST_ASSERT(getWhiteBrightness() == MAX_BRIGHTNESS);
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
  TEST_ASSERT(pwm_ir.running == 0);

  increaseWhiteBrightness(HOLD);
  TEST_ASSERT(getWhiteBrightness() == MAX_BRIGHTNESS);
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
  TEST_ASSERT(pwm_ir.running == 0);

  initWhitePWM();
  initIRPWM();

  setWhiteBrightness(MAX_BRIGHTNESS);
  increaseWhiteBrightness(HOLD);
  TEST_ASSERT(getWhiteBrightness() == MAX_BRIGHTNESS);
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
  TEST_ASSERT(pwm_ir.running == 0);

  increaseWhiteBrightness(PRESS);
  TEST_ASSERT(getWhiteBrightness() == MAX_BRIGHTNESS);
  TEST_ASSERT(pwm_white.running == 1);
  TEST_ASSERT(pwm_white.pulse_width == getWhitePWM());
  TEST_ASSERT(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
  TEST_ASSERT(pwm_ir.running == 0);
}

// verify edge case of increaseIRBrightness
TEST(PWM_Handler, increaseIRBrightnessEdgeCase)
{
  extern pwmStruct pwm_white;
  extern pwmStruct pwm_ir;

  setIRBrightness(MAX_BRIGHTNESS);
  increaseIRBrightness(PRESS);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
  TEST_ASSERT(pwm_white.running == 0);
  TEST_ASSERT(getIRBrightness()    == MAX_BRIGHTNESS);
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());

  increaseIRBrightness(HOLD);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
  TEST_ASSERT(pwm_white.running == 0);
  TEST_ASSERT(getIRBrightness()    == MAX_BRIGHTNESS);
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());

  initWhitePWM();
  initIRPWM();

  setIRBrightness(MAX_BRIGHTNESS);
  increaseIRBrightness(HOLD);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
  TEST_ASSERT(pwm_white.running == 0);
  TEST_ASSERT(getIRBrightness()    == MAX_BRIGHTNESS);
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());

  increaseIRBrightness(PRESS);
  TEST_ASSERT(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
  TEST_ASSERT(pwm_white.running == 0);
  TEST_ASSERT(getIRBrightness()    == MAX_BRIGHTNESS);
  TEST_ASSERT(pwm_ir.running == 1);
  TEST_ASSERT(pwm_ir.pulse_width == getIRPWM());
}

// verify edge case of setWhiteBrightness
TEST(PWM_Handler, setWhiteBrightnessEdgeCase)
{
  int8_t brightness = MIN_BRIGHTNESS - 1;
  setWhiteBrightness(brightness);
  TEST_ASSERT(getWhiteBrightness() == MIN_BRIGHTNESS);
  brightness = MAX_BRIGHTNESS + 1;
  setWhiteBrightness(brightness);
  TEST_ASSERT(getWhiteBrightness() == MAX_BRIGHTNESS);
  brightness = 100;
  setWhiteBrightness(brightness);
  TEST_ASSERT(getWhiteBrightness() == MAX_BRIGHTNESS);
  brightness = -75;
  setWhiteBrightness(brightness);
  TEST_ASSERT(getWhiteBrightness() == MIN_BRIGHTNESS);
}

// verify edge case of setIRBrightness
TEST(PWM_Handler, setIRBrightnessEdgeCase)
{
  int8_t brightness = MIN_BRIGHTNESS - 1;
  setIRBrightness(brightness);
  TEST_ASSERT(getIRBrightness() == MIN_BRIGHTNESS);
  brightness = MAX_BRIGHTNESS + 1;
  setIRBrightness(brightness);
  TEST_ASSERT(getIRBrightness() == MAX_BRIGHTNESS);
  brightness = 52;
  setIRBrightness(brightness);
  TEST_ASSERT(getIRBrightness() == MAX_BRIGHTNESS);
  brightness = -121;
  setIRBrightness(brightness);
  TEST_ASSERT(getIRBrightness() == MIN_BRIGHTNESS);
}
/* end pwm_handler tests */

TEST_GROUP_RUNNER(PWM_Handler)
{
  RUN_TEST_CASE(PWM_Handler, decreaseWhiteBrightnessNiceCase);
  RUN_TEST_CASE(PWM_Handler, decreaseIRBrightnessNiceCase);
  RUN_TEST_CASE(PWM_Handler, increaseWhiteBrightnessNiceCase);
  RUN_TEST_CASE(PWM_Handler, increaseIRBrightnessNiceCase);
  RUN_TEST_CASE(PWM_Handler, setWhitePWMNiceCase);
  RUN_TEST_CASE(PWM_Handler, setIRPWMNiceCase);
  RUN_TEST_CASE(PWM_Handler, turnOffWhitePWMNiceCase);
  RUN_TEST_CASE(PWM_Handler, turnOffIRPWMNiceCase);
  RUN_TEST_CASE(PWM_Handler, setWhiteBrightnessNiceCase);
  RUN_TEST_CASE(PWM_Handler, setIRBrightnessNiceCase);
  RUN_TEST_CASE(PWM_Handler, getWhitePWMNiceCase);
  RUN_TEST_CASE(PWM_Handler, getIRPWMNiceCase);
  RUN_TEST_CASE(PWM_Handler, getWhitePWMThermistorNiceCase);
  RUN_TEST_CASE(PWM_Handler, getIRPWMThermistorNiceCase);
  RUN_TEST_CASE(PWM_Handler, decreaseWhiteBrightnessEdgeCase);
  RUN_TEST_CASE(PWM_Handler, decreaseIRBrightnessEdgeCase);
  RUN_TEST_CASE(PWM_Handler, increaseWhiteBrightnessEdgeCase);
  RUN_TEST_CASE(PWM_Handler, increaseIRBrightnessEdgeCase);
  RUN_TEST_CASE(PWM_Handler, setWhiteBrightnessEdgeCase);
  RUN_TEST_CASE(PWM_Handler, setIRBrightnessEdgeCase);
}