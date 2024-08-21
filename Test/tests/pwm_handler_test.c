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

extern const uint8_t    MinBrightness  ;
extern const uint8_t    MaxBrightness  ;
extern const uint8_t    HalfBrightness ;
extern const float      MinWhitePw    ;
extern const float      MaxWhitePw    ;
extern const float      WarmPwmRatio  ;
extern const float      HotPwmRatio   ;

TEST_GROUP(PWM_Handler);

TEST_SETUP(PWM_Handler)
{
    /* executed before *every* non-skipped test */
    InitWhitePwm();
}

TEST_TEAR_DOWN(PWM_Handler)
{
    /* executed after *every* non-skipped and non-failing test */
    thermistor_value = 0; // return to default value
    GetTemperatureRange(); // set thermistor range
}

/* start pwm_handler tests */
/* NICE CASES */
// verify nice case of DecreaseWhiteBrightness
TEST(PWM_Handler, DecreaseWhiteBrightnessNiceCase)
{
    extern PwmStruct pwm_white;

    DecreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness - 1);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());

    DecreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness - 1 - HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());

    InitWhitePwm();

    DecreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness - HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());

    DecreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness - HOLD_BRIGHTNESS_JUMP - 1);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
}

// verify nice case of IncreaseWhiteBrightness
TEST(PWM_Handler, IncreaseWhiteBrightnessNiceCase)
{
    extern PwmStruct pwm_white;

    IncreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness + 1);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());

    IncreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness + 1 + HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());

    InitWhitePwm();

    IncreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness + HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());

    IncreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness + HOLD_BRIGHTNESS_JUMP + 1);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
}

// verify nice case of SetWhitePwm
TEST(PWM_Handler, SetWhitePwmNiceCase)
{
    extern PwmStruct pwm_white;

    SetWhitePwm();
    // White should be on and be set to white pwm
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
}

// verify nice case of TurnOffWhitePwm
TEST(PWM_Handler, TurnOffWhitePwmNiceCase)
{
    extern PwmStruct pwm_white;

    SetWhitePwm();
    TurnOffWhitePwm();
    // White should be off
    TEST_ASSERT(pwm_white.is_running == 0);
}

// verify nice case of SetWhiteBrightness
TEST(PWM_Handler, SetWhiteBrightnessNiceCase)
{
    int8_t brightness = MinBrightness;
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhiteBrightness() == brightness);
    brightness = MaxBrightness;
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhiteBrightness() == brightness);
    brightness = HalfBrightness;
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhiteBrightness() == brightness);
    brightness = 32;
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhiteBrightness() == brightness);
    brightness = 16;
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhiteBrightness() == brightness);
}

// verify nice case of GetWhitePwm
TEST(PWM_Handler, GetWhitePwmNiceCase)
{
    const float white_pwm_steps = (MaxWhitePw - MinWhitePw) / ((float)BRIGHTNESS_STEPS - 1.0f);
    int8_t brightness = MinBrightness;
    uint8_t expected_pwm = (uint8_t)((brightness * white_pwm_steps) + MinWhitePw + 0.5f);
    SetWhiteBrightness(brightness);
    TEST_ASSERT_EQUAL_UINT8(expected_pwm, GetWhitePwm());
    brightness = MaxBrightness;
    expected_pwm = (uint8_t)((brightness * white_pwm_steps) + MinWhitePw + 0.5f);
    SetWhiteBrightness(brightness);
    TEST_ASSERT_EQUAL_UINT8(expected_pwm, GetWhitePwm());
    brightness = HalfBrightness;
    expected_pwm = (uint8_t)((brightness * white_pwm_steps) + MinWhitePw + 0.5f);
    SetWhiteBrightness(brightness);
    TEST_ASSERT_EQUAL_UINT8(expected_pwm, GetWhitePwm());
    brightness = 17;
    expected_pwm = (uint8_t)((brightness * white_pwm_steps) + MinWhitePw + 0.5f);
    SetWhiteBrightness(brightness);
    TEST_ASSERT_EQUAL_UINT8(expected_pwm, GetWhitePwm());
    brightness = 42;
    expected_pwm = (uint8_t)((brightness * white_pwm_steps) + MinWhitePw + 0.5f);
    SetWhiteBrightness(brightness);
    TEST_ASSERT_EQUAL_UINT8(expected_pwm, GetWhitePwm());
}

// verify nice case of GetWhitePwm
TEST(PWM_Handler, GetWhitePwmThermistorNiceCase)
{
    const float white_pwm_steps = (MaxWhitePw - MinWhitePw) / ((float)BRIGHTNESS_STEPS - 1.0f);
    const int8_t brightness = MaxBrightness;
    const uint8_t expected_pwm = (uint8_t)((brightness * white_pwm_steps) + MinWhitePw + 0.5f);
    const uint8_t expected_cool_pwm = expected_pwm;
    const uint8_t expected_warm_pwm = (uint8_t)(expected_pwm * WarmPwmRatio + 0.5);
    const uint8_t expected_hot_pwm  = (uint8_t)(expected_pwm * HotPwmRatio + 0.5);
    SetWhiteBrightness(brightness);

    thermistor_value = HEATING_WARM_THERM;
    TEST_ASSERT(GetWhitePwm() == expected_warm_pwm);

    thermistor_value = HEATING_HOT_THERM;
    TEST_ASSERT(GetWhitePwm() == expected_hot_pwm);

    thermistor_value = COOLING_COOL_THERM;
    TEST_ASSERT(GetWhitePwm() == expected_cool_pwm);
}

/* EDGE CASES */
// verify edge case of DecreaseWhiteBrightness
TEST(PWM_Handler, DecreaseWhiteBrightnessEdgeCase)
{
    extern PwmStruct pwm_white;

    SetWhiteBrightness(MinBrightness);
    DecreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == MinBrightness);
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());

    DecreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == MinBrightness);
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());

    InitWhitePwm();

    SetWhiteBrightness(MinBrightness);
    DecreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == MinBrightness);
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());

    DecreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == MinBrightness);
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
}

// verify edge case of IncreaseWhiteBrightness
TEST(PWM_Handler, IncreaseWhiteBrightnessEdgeCase)
{
    extern PwmStruct pwm_white;

    SetWhiteBrightness(MaxBrightness);
    IncreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == MaxBrightness);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());

    IncreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == MaxBrightness);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());

    InitWhitePwm();

    SetWhiteBrightness(MaxBrightness);
    IncreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == MaxBrightness);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());

    IncreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == MaxBrightness);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
}

// verify edge case of SetWhiteBrightness
TEST(PWM_Handler, SetWhiteBrightnessEdgeCase)
{
    int8_t brightness = MinBrightness - 1;
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhiteBrightness() == MinBrightness);
    brightness = MaxBrightness + 1;
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhiteBrightness() == MaxBrightness);
    brightness = 100;
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhiteBrightness() == MaxBrightness);
    brightness = -75;
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhiteBrightness() == MinBrightness);
}
/* end pwm_handler tests */

TEST_GROUP_RUNNER(PWM_Handler)
{
    RUN_TEST_CASE(PWM_Handler, DecreaseWhiteBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, IncreaseWhiteBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, SetWhitePwmNiceCase);
    RUN_TEST_CASE(PWM_Handler, TurnOffWhitePwmNiceCase);
    RUN_TEST_CASE(PWM_Handler, SetWhiteBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, GetWhitePwmNiceCase);
    RUN_TEST_CASE(PWM_Handler, GetWhitePwmThermistorNiceCase);
    RUN_TEST_CASE(PWM_Handler, DecreaseWhiteBrightnessEdgeCase);
    RUN_TEST_CASE(PWM_Handler, IncreaseWhiteBrightnessEdgeCase);
    RUN_TEST_CASE(PWM_Handler, SetWhiteBrightnessEdgeCase);
}