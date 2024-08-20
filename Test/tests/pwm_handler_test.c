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
extern const float      MinIrPw       ;
extern const float      MaxWhitePw    ;
extern const float      MaxIrPw       ;
extern const float      WarmPwmRatio  ;
extern const float      HotPwmRatio   ;

TEST_GROUP(PWM_Handler);

TEST_SETUP(PWM_Handler)
{
    /* executed before *every* non-skipped test */
    InitWhitePwm();
    InitIrPwm();
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
    extern PwmStruct pwm_ir;

    DecreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness - 1);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    DecreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness - 1 - HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    InitWhitePwm();
    InitIrPwm();

    DecreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness - HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness); // IR should not change

    TEST_ASSERT(pwm_ir.is_running == 0);
    DecreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness - HOLD_BRIGHTNESS_JUMP - 1);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);
}

// verify nice case of DecreaseIRBrightness
TEST(PWM_Handler, DecreaseIRBrightnessNiceCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    DecreaseIRBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness - 1);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());

    DecreaseIRBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness - 1 - HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());

    InitWhitePwm();
    InitIrPwm();

    DecreaseIRBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness - HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());

    DecreaseIRBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness - HOLD_BRIGHTNESS_JUMP - 1);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());
}

// verify nice case of IncreaseWhiteBrightness
TEST(PWM_Handler, IncreaseWhiteBrightnessNiceCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    IncreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness + 1);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    IncreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness + 1 + HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    InitWhitePwm();
    InitIrPwm();

    IncreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness + HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    IncreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness + HOLD_BRIGHTNESS_JUMP + 1);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);
}

// verify nice case of IncreaseIRBrightness
TEST(PWM_Handler, IncreaseIRBrightnessNiceCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    IncreaseIRBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness + 1);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());

    IncreaseIRBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness + 1 + HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());

    InitWhitePwm();
    InitIrPwm();

    IncreaseIRBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness + HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());

    IncreaseIRBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness + HOLD_BRIGHTNESS_JUMP + 1);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());
}

// verify nice case of SetWhitePwm
TEST(PWM_Handler, SetWhitePwmNiceCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    SetWhitePwm();
    // White should be on and be set to white pwm
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    // IR should be off
    TEST_ASSERT(pwm_ir.is_running == 0);
}

// verify nice case of SetIrPwm
TEST(PWM_Handler, SetIrPwmNiceCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    SetIrPwm();
    // IR should be on and be set to IR pwm
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());
    // White should be off
    TEST_ASSERT(pwm_white.is_running == 0);
}

// verify nice case of TurnOffWhitePwm
TEST(PWM_Handler, TurnOffWhitePwmNiceCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    SetWhitePwm();
    TurnOffWhitePwm();
    // White should be off
    TEST_ASSERT(pwm_white.is_running == 0);
}

// verify nice case of TurnOffIrPwm
TEST(PWM_Handler, TurnOffIrPwmNiceCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    SetIrPwm();
    TurnOffIrPwm();
    // IR should be off
    TEST_ASSERT(pwm_ir.is_running == 0);
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

// verify nice case of SetIRBrightness
TEST(PWM_Handler, SetIRBrightnessNiceCase)
{
    int8_t brightness = MinBrightness;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == brightness);
    brightness = MaxBrightness;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == brightness);
    brightness = HalfBrightness;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == brightness);
    brightness = 32;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == brightness);
    brightness = 16;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == brightness);
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

// verify nice case of GetIrPwm
TEST(PWM_Handler, GetIrPwmNiceCase)
{
    const float IR_pwm_steps = (MaxIrPw - MinIrPw) / ((float)BRIGHTNESS_STEPS - 1.0f);
    int8_t brightness = MinBrightness;
    uint8_t expected_pwm = (uint8_t)((brightness * IR_pwm_steps) + MinIrPw + 0.5f);
    SetIRBrightness(brightness);
    TEST_ASSERT_EQUAL_UINT8(expected_pwm, GetIrPwm());
    brightness = MaxBrightness;
    expected_pwm = (uint8_t)((brightness * IR_pwm_steps) + MinIrPw + 0.5f);
    SetIRBrightness(brightness);
    TEST_ASSERT_EQUAL_UINT8(expected_pwm, GetIrPwm());
    brightness = HalfBrightness;
    expected_pwm = (uint8_t)((brightness * IR_pwm_steps) + MinIrPw + 0.5f);
    SetIRBrightness(brightness);
    TEST_ASSERT_EQUAL_UINT8(expected_pwm, GetIrPwm());
    brightness = 12;
    expected_pwm = (uint8_t)((brightness * IR_pwm_steps) + MinIrPw + 0.5f);
    SetIRBrightness(brightness);
    TEST_ASSERT_EQUAL_UINT8(expected_pwm, GetIrPwm());
    brightness = 39;
    expected_pwm = (uint8_t)((brightness * IR_pwm_steps) + MinIrPw + 0.5f);
    SetIRBrightness(brightness);
    TEST_ASSERT_EQUAL_UINT8(expected_pwm, GetIrPwm());
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

// verify nice case of GetIrPwm
TEST(PWM_Handler, GetIrPwmThermistorNiceCase)
{
    const float IR_pwm_steps = (MaxIrPw - MinIrPw) / ((float)BRIGHTNESS_STEPS - 1.0f);
    const int8_t brightness = MaxBrightness;
    const uint8_t expected_pwm = (uint8_t)((brightness * IR_pwm_steps) + MinIrPw + 0.5f);
    const uint8_t expected_cool_pwm = expected_pwm;
    const uint8_t expected_warm_pwm = (uint8_t)(expected_pwm * WarmPwmRatio + 0.5);
    const uint8_t expected_hot_pwm  = (uint8_t)(expected_pwm * HotPwmRatio + 0.5);
    SetIRBrightness(brightness);

    thermistor_value = HEATING_WARM_THERM;
    TEST_ASSERT(GetIrPwm() == expected_warm_pwm);

    thermistor_value = HEATING_HOT_THERM;
    TEST_ASSERT(GetIrPwm() == expected_hot_pwm);

    thermistor_value = COOLING_COOL_THERM;
    TEST_ASSERT(GetIrPwm() == expected_cool_pwm);
}

/* EDGE CASES */
// verify edge case of DecreaseWhiteBrightness
TEST(PWM_Handler, DecreaseWhiteBrightnessEdgeCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    SetWhiteBrightness(MinBrightness);
    DecreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == MinBrightness);
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    DecreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == MinBrightness);
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    InitWhitePwm();
    InitIrPwm();

    SetWhiteBrightness(MinBrightness);
    DecreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == MinBrightness);
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness); // IR should not change

    TEST_ASSERT(pwm_ir.is_running == 0);
    DecreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == MinBrightness);
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);
}

// verify edge case of DecreaseIRBrightness
TEST(PWM_Handler, DecreaseIRBrightnessEdgeCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    SetIRBrightness(MinBrightness);
    DecreaseIRBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == MinBrightness);
    TEST_ASSERT(pwm_ir.is_running == 0);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());

    DecreaseIRBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == MinBrightness);
    TEST_ASSERT(pwm_ir.is_running == 0);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());

    InitWhitePwm();
    InitIrPwm();

    SetIRBrightness(MinBrightness);
    DecreaseIRBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == MinBrightness);
    TEST_ASSERT(pwm_ir.is_running == 0);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());

    DecreaseIRBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == MinBrightness);
    TEST_ASSERT(pwm_ir.is_running == 0);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());
}

// verify edge case of IncreaseWhiteBrightness
TEST(PWM_Handler, IncreaseWhiteBrightnessEdgeCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    SetWhiteBrightness(MaxBrightness);
    IncreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == MaxBrightness);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    IncreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == MaxBrightness);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    InitWhitePwm();
    InitIrPwm();

    SetWhiteBrightness(MaxBrightness);
    IncreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == MaxBrightness);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    IncreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == MaxBrightness);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePwm());
    TEST_ASSERT(GetIRBrightness()    == HalfBrightness); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);
}

// verify edge case of IncreaseIRBrightness
TEST(PWM_Handler, IncreaseIRBrightnessEdgeCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    SetIRBrightness(MaxBrightness);
    IncreaseIRBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == MaxBrightness);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());

    IncreaseIRBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == MaxBrightness);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());

    InitWhitePwm();
    InitIrPwm();

    SetIRBrightness(MaxBrightness);
    IncreaseIRBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == MaxBrightness);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());

    IncreaseIRBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HalfBrightness); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == MaxBrightness);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIrPwm());
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

// verify edge case of SetIRBrightness
TEST(PWM_Handler, SetIRBrightnessEdgeCase)
{
    int8_t brightness = MinBrightness - 1;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == MinBrightness);
    brightness = MaxBrightness + 1;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == MaxBrightness);
    brightness = 52;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == MaxBrightness);
    brightness = -121;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == MinBrightness);
}
/* end pwm_handler tests */

TEST_GROUP_RUNNER(PWM_Handler)
{
    RUN_TEST_CASE(PWM_Handler, DecreaseWhiteBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, DecreaseIRBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, IncreaseWhiteBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, IncreaseIRBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, SetWhitePwmNiceCase);
    RUN_TEST_CASE(PWM_Handler, SetIrPwmNiceCase);
    RUN_TEST_CASE(PWM_Handler, TurnOffWhitePwmNiceCase);
    RUN_TEST_CASE(PWM_Handler, TurnOffIrPwmNiceCase);
    RUN_TEST_CASE(PWM_Handler, SetWhiteBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, SetIRBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, GetWhitePwmNiceCase);
    RUN_TEST_CASE(PWM_Handler, GetIrPwmNiceCase);
    RUN_TEST_CASE(PWM_Handler, GetWhitePwmThermistorNiceCase);
    RUN_TEST_CASE(PWM_Handler, GetIrPwmThermistorNiceCase);
    RUN_TEST_CASE(PWM_Handler, DecreaseWhiteBrightnessEdgeCase);
    RUN_TEST_CASE(PWM_Handler, DecreaseIRBrightnessEdgeCase);
    RUN_TEST_CASE(PWM_Handler, IncreaseWhiteBrightnessEdgeCase);
    RUN_TEST_CASE(PWM_Handler, IncreaseIRBrightnessEdgeCase);
    RUN_TEST_CASE(PWM_Handler, SetWhiteBrightnessEdgeCase);
    RUN_TEST_CASE(PWM_Handler, SetIRBrightnessEdgeCase);
}