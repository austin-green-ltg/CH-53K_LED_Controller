#include "unity_fixture.h"      /* UNITY */
#include "pwm_handler.h"        /* CUT */
#include "thermistor_handler.h" /* CUT */
#include "stm32l412xx-bsp.h"    /* CUT */

#define PRESS (0)
#define HOLD  (1)

#define HEATING_WARM_THERM (100000)
#define HEATING_HOT_THERM  (120000)
#define COOLING_COOL_THERM (80000)
#define COOLING_WARM_THERM (100000)

extern int32_t thermistor_value;

extern const uint8_t    MinBrightness   ;
extern const uint8_t    MaxBrightness   ;
extern const uint8_t    HalfBrightness  ;
extern const float      MinPw           ;
extern const float      MaxPw           ;
extern const float      WarmPwmRatio    ;
extern const float      HotPwmRatio     ;

TEST_GROUP(PWM_Handler);

TEST_SETUP(PWM_Handler)
{
    /* executed before *every* non-skipped test */
    InitPwm();
}

TEST_TEAR_DOWN(PWM_Handler)
{
    /* executed after *every* non-skipped and non-failing test */
    thermistor_value = 0; // return to default value
    GetTemperatureRange(); // set thermistor range
}

/* start pwm_handler tests */
/* NICE CASES */
// verify nice case of DecreaseBrightness
TEST(PWM_Handler, DecreaseBrightnessNiceCase)
{
    extern PwmStruct pwm;

    DecreaseBrightness(PRESS);
    TEST_ASSERT(GetBrightness() == HalfBrightness - 1);
    TEST_ASSERT(pwm.is_running == 1);
    TEST_ASSERT(pwm.pulse_width == GetPwm());

    DecreaseBrightness(HOLD);
    TEST_ASSERT(GetBrightness() == HalfBrightness - 1 - HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm.is_running == 1);
    TEST_ASSERT(pwm.pulse_width == GetPwm());

    InitPwm();

    DecreaseBrightness(HOLD);
    TEST_ASSERT(GetBrightness() == HalfBrightness - HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm.is_running == 1);
    TEST_ASSERT(pwm.pulse_width == GetPwm());

    DecreaseBrightness(PRESS);
    TEST_ASSERT(GetBrightness() == HalfBrightness - HOLD_BRIGHTNESS_JUMP - 1);
    TEST_ASSERT(pwm.is_running == 1);
    TEST_ASSERT(pwm.pulse_width == GetPwm());
}

// verify nice case of IncreaseBrightness
TEST(PWM_Handler, IncreaseBrightnessNiceCase)
{
    extern PwmStruct pwm;

    IncreaseBrightness(PRESS);
    TEST_ASSERT(GetBrightness() == HalfBrightness + 1);
    TEST_ASSERT(pwm.is_running == 1);
    TEST_ASSERT(pwm.pulse_width == GetPwm());

    IncreaseBrightness(HOLD);
    TEST_ASSERT(GetBrightness() == HalfBrightness + 1 + HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm.is_running == 1);
    TEST_ASSERT(pwm.pulse_width == GetPwm());

    InitPwm();

    IncreaseBrightness(HOLD);
    TEST_ASSERT(GetBrightness() == HalfBrightness + HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm.is_running == 1);
    TEST_ASSERT(pwm.pulse_width == GetPwm());

    IncreaseBrightness(PRESS);
    TEST_ASSERT(GetBrightness() == HalfBrightness + HOLD_BRIGHTNESS_JUMP + 1);
    TEST_ASSERT(pwm.is_running == 1);
    TEST_ASSERT(pwm.pulse_width == GetPwm());
}

// verify nice case of SetPwm
TEST(PWM_Handler, SetPwmNiceCase)
{
    extern PwmStruct pwm;

    SetPwm();
    // should be on and be set to pwm
    TEST_ASSERT(pwm.is_running == 1);
    TEST_ASSERT(pwm.pulse_width == GetPwm());
}

// verify nice case of TurnOffPwm
TEST(PWM_Handler, TurnOffPwmNiceCase)
{
    extern PwmStruct pwm;

    SetPwm();
    TurnOffPwm();
    // should be off
    TEST_ASSERT(pwm.is_running == 0);
}

// verify nice case of SetBrightness
TEST(PWM_Handler, SetBrightnessNiceCase)
{
    int8_t brightness = MinBrightness;
    SetBrightness(brightness);
    TEST_ASSERT(GetBrightness() == brightness);
    brightness = MaxBrightness;
    SetBrightness(brightness);
    TEST_ASSERT(GetBrightness() == brightness);
    brightness = HalfBrightness;
    SetBrightness(brightness);
    TEST_ASSERT(GetBrightness() == brightness);
    brightness = 32;
    SetBrightness(brightness);
    TEST_ASSERT(GetBrightness() == brightness);
    brightness = 16;
    SetBrightness(brightness);
    TEST_ASSERT(GetBrightness() == brightness);
}

// verify nice case of GetPwm
TEST(PWM_Handler, GetPwmNiceCase)
{
    const float pwm_steps = (MaxPw - MinPw) / ((float)BRIGHTNESS_STEPS - 1.0f);
    int8_t brightness = MinBrightness;
    uint8_t expected_pwm = (uint8_t)((brightness * pwm_steps) + MinPw + 0.5f);
    SetBrightness(brightness);
    TEST_ASSERT_EQUAL_UINT8(expected_pwm, GetPwm());
    brightness = MaxBrightness;
    expected_pwm = (uint8_t)((brightness * pwm_steps) + MinPw + 0.5f);
    SetBrightness(brightness);
    TEST_ASSERT_EQUAL_UINT8(expected_pwm, GetPwm());
    brightness = HalfBrightness;
    expected_pwm = (uint8_t)((brightness * pwm_steps) + MinPw + 0.5f);
    SetBrightness(brightness);
    TEST_ASSERT_EQUAL_UINT8(expected_pwm, GetPwm());
    brightness = 17;
    expected_pwm = (uint8_t)((brightness * pwm_steps) + MinPw + 0.5f);
    SetBrightness(brightness);
    TEST_ASSERT_EQUAL_UINT8(expected_pwm, GetPwm());
    brightness = 42;
    expected_pwm = (uint8_t)((brightness * pwm_steps) + MinPw + 0.5f);
    SetBrightness(brightness);
    TEST_ASSERT_EQUAL_UINT8(expected_pwm, GetPwm());
}

// verify nice case of GetPwm
TEST(PWM_Handler, GetPwmThermistorNiceCase)
{
    const float pwm_steps = (MaxPw - MinPw) / ((float)BRIGHTNESS_STEPS - 1.0f);
    const int8_t brightness = MaxBrightness;
    const uint8_t expected_pwm = (uint8_t)((brightness * pwm_steps) + MinPw + 0.5f);
    const uint8_t expected_cool_pwm = expected_pwm;
    const uint8_t expected_warm_pwm = (uint8_t)(expected_pwm * WarmPwmRatio + 0.5);
    const uint8_t expected_hot_pwm  = (uint8_t)(expected_pwm * HotPwmRatio + 0.5);
    SetBrightness(brightness);

    thermistor_value = HEATING_WARM_THERM;
    TEST_ASSERT(GetPwm() == expected_warm_pwm);

    thermistor_value = HEATING_HOT_THERM;
    TEST_ASSERT(GetPwm() == expected_hot_pwm);

    thermistor_value = COOLING_COOL_THERM;
    TEST_ASSERT(GetPwm() == expected_cool_pwm);
}

/* EDGE CASES */
// verify edge case of DecreaseBrightness
TEST(PWM_Handler, DecreaseBrightnessEdgeCase)
{
    extern PwmStruct pwm;

    SetBrightness(MinBrightness);
    DecreaseBrightness(PRESS);
    TEST_ASSERT(GetBrightness() == MinBrightness);
    TEST_ASSERT(pwm.is_running == 0);
    TEST_ASSERT(pwm.pulse_width == GetPwm());

    DecreaseBrightness(HOLD);
    TEST_ASSERT(GetBrightness() == MinBrightness);
    TEST_ASSERT(pwm.is_running == 0);
    TEST_ASSERT(pwm.pulse_width == GetPwm());

    InitPwm();

    SetBrightness(MinBrightness);
    DecreaseBrightness(HOLD);
    TEST_ASSERT(GetBrightness() == MinBrightness);
    TEST_ASSERT(pwm.is_running == 0);
    TEST_ASSERT(pwm.pulse_width == GetPwm());

    DecreaseBrightness(PRESS);
    TEST_ASSERT(GetBrightness() == MinBrightness);
    TEST_ASSERT(pwm.is_running == 0);
    TEST_ASSERT(pwm.pulse_width == GetPwm());
}

// verify edge case of IncreaseBrightness
TEST(PWM_Handler, IncreaseBrightnessEdgeCase)
{
    extern PwmStruct pwm;

    SetBrightness(MaxBrightness);
    IncreaseBrightness(PRESS);
    TEST_ASSERT(GetBrightness() == MaxBrightness);
    TEST_ASSERT(pwm.is_running == 1);
    TEST_ASSERT(pwm.pulse_width == GetPwm());

    IncreaseBrightness(HOLD);
    TEST_ASSERT(GetBrightness() == MaxBrightness);
    TEST_ASSERT(pwm.is_running == 1);
    TEST_ASSERT(pwm.pulse_width == GetPwm());

    InitPwm();

    SetBrightness(MaxBrightness);
    IncreaseBrightness(HOLD);
    TEST_ASSERT(GetBrightness() == MaxBrightness);
    TEST_ASSERT(pwm.is_running == 1);
    TEST_ASSERT(pwm.pulse_width == GetPwm());

    IncreaseBrightness(PRESS);
    TEST_ASSERT(GetBrightness() == MaxBrightness);
    TEST_ASSERT(pwm.is_running == 1);
    TEST_ASSERT(pwm.pulse_width == GetPwm());
}

// verify edge case of SetBrightness
TEST(PWM_Handler, SetBrightnessEdgeCase)
{
    int8_t brightness = MinBrightness - 1;
    SetBrightness(brightness);
    TEST_ASSERT(GetBrightness() == MinBrightness);
    brightness = MaxBrightness + 1;
    SetBrightness(brightness);
    TEST_ASSERT(GetBrightness() == MaxBrightness);
    brightness = 100;
    SetBrightness(brightness);
    TEST_ASSERT(GetBrightness() == MaxBrightness);
    brightness = -75;
    SetBrightness(brightness);
    TEST_ASSERT(GetBrightness() == MinBrightness);
}
/* end pwm_handler tests */

TEST_GROUP_RUNNER(PWM_Handler)
{
    RUN_TEST_CASE(PWM_Handler, DecreaseBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, IncreaseBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, SetPwmNiceCase);
    RUN_TEST_CASE(PWM_Handler, TurnOffPwmNiceCase);
    RUN_TEST_CASE(PWM_Handler, SetBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, GetPwmNiceCase);
    RUN_TEST_CASE(PWM_Handler, GetPwmThermistorNiceCase);
    RUN_TEST_CASE(PWM_Handler, DecreaseBrightnessEdgeCase);
    RUN_TEST_CASE(PWM_Handler, IncreaseBrightnessEdgeCase);
    RUN_TEST_CASE(PWM_Handler, SetBrightnessEdgeCase);
}