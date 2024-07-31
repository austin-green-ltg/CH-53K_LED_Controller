#include "unity_fixture.h"      /* UNITY */
#include "pwm_handler.h"        /* CUT */
#include "thermistor_handler.h" /* CUT */
#include "bsp.h"                /* CUT */

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
    InitWhitePWM();
    InitIRPWM();
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
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS - 1);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    DecreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS - 1 - HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    InitWhitePWM();
    InitIRPWM();

    DecreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS - HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS); // IR should not change

    TEST_ASSERT(pwm_ir.is_running == 0);
    DecreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS - HOLD_BRIGHTNESS_JUMP - 1);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);
}

// verify nice case of DecreaseIRBrightness
TEST(PWM_Handler, DecreaseIRBrightnessNiceCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    DecreaseIRBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS - 1);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());

    DecreaseIRBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS - 1 - HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());

    InitWhitePWM();
    InitIRPWM();

    DecreaseIRBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS - HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());

    DecreaseIRBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS - HOLD_BRIGHTNESS_JUMP - 1);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());
}

// verify nice case of IncreaseWhiteBrightness
TEST(PWM_Handler, IncreaseWhiteBrightnessNiceCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    IncreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS + 1);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    IncreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS + 1 + HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    InitWhitePWM();
    InitIRPWM();

    IncreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS + HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    IncreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS + HOLD_BRIGHTNESS_JUMP + 1);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);
}

// verify nice case of IncreaseIRBrightness
TEST(PWM_Handler, IncreaseIRBrightnessNiceCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    IncreaseIRBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS + 1);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());

    IncreaseIRBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS + 1 + HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());

    InitWhitePWM();
    InitIRPWM();

    IncreaseIRBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS + HOLD_BRIGHTNESS_JUMP);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());

    IncreaseIRBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS + HOLD_BRIGHTNESS_JUMP + 1);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());
}

// verify nice case of SetWhitePWM
TEST(PWM_Handler, SetWhitePWMNiceCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    SetWhitePWM();
    // White should be on and be set to white pwm
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    // IR should be off
    TEST_ASSERT(pwm_ir.is_running == 0);
}

// verify nice case of SetIRPWM
TEST(PWM_Handler, SetIRPWMNiceCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    SetIRPWM();
    // IR should be on and be set to IR pwm
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());
    // White should be off
    TEST_ASSERT(pwm_white.is_running == 0);
}

// verify nice case of TurnOffWhitePWM
TEST(PWM_Handler, TurnOffWhitePWMNiceCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    SetWhitePWM();
    TurnOffWhitePWM();
    // White should be off
    TEST_ASSERT(pwm_white.is_running == 0);
}

// verify nice case of TurnOffIRPWM
TEST(PWM_Handler, TurnOffIRPWMNiceCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    SetIRPWM();
    TurnOffIRPWM();
    // IR should be off
    TEST_ASSERT(pwm_ir.is_running == 0);
}

// verify nice case of SetWhiteBrightness
TEST(PWM_Handler, SetWhiteBrightnessNiceCase)
{
    int8_t brightness = MIN_BRIGHTNESS;
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhiteBrightness() == brightness);
    brightness = MAX_BRIGHTNESS;
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhiteBrightness() == brightness);
    brightness = HALF_BRIGHTNESS;
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
    int8_t brightness = MIN_BRIGHTNESS;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == brightness);
    brightness = MAX_BRIGHTNESS;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == brightness);
    brightness = HALF_BRIGHTNESS;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == brightness);
    brightness = 32;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == brightness);
    brightness = 16;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == brightness);
}

// verify nice case of GetWhitePWM
TEST(PWM_Handler, GetWhitePWMNiceCase)
{
    const float white_pwm_steps = (MAX_WHITE_PW - MIN_WHITE_PW) / (BRIGHTNESS_STEPS - 1);
    int8_t brightness = MIN_BRIGHTNESS;
    uint8_t expected_pwm = (uint8_t)((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhitePWM() == expected_pwm);
    brightness = MAX_BRIGHTNESS;
    expected_pwm = (uint8_t)((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhitePWM() == expected_pwm);
    brightness = HALF_BRIGHTNESS;
    expected_pwm = (uint8_t)((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhitePWM() == expected_pwm);
    brightness = 17;
    expected_pwm = (uint8_t)((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhitePWM() == expected_pwm);
    brightness = 42;
    expected_pwm = (uint8_t)((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhitePWM() == expected_pwm);
}

// verify nice case of GetIRPWM
TEST(PWM_Handler, GetIRPWMNiceCase)
{
    const float IR_pwm_steps = (MAX_IR_PW - MIN_IR_PW) / (BRIGHTNESS_STEPS - 1);
    int8_t brightness = MIN_BRIGHTNESS;
    uint8_t expected_pwm = (uint8_t)((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRPWM() == expected_pwm);
    brightness = MAX_BRIGHTNESS;
    expected_pwm = (uint8_t)((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRPWM() == expected_pwm);
    brightness = HALF_BRIGHTNESS;
    expected_pwm = (uint8_t)((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRPWM() == expected_pwm);
    brightness = 12;
    expected_pwm = (uint8_t)((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRPWM() == expected_pwm);
    brightness = 39;
    expected_pwm = (uint8_t)((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRPWM() == expected_pwm);
}

// verify nice case of GetWhitePWM
TEST(PWM_Handler, GetWhitePWMThermistorNiceCase)
{
    const float white_pwm_steps = (MAX_WHITE_PW - MIN_WHITE_PW) / (BRIGHTNESS_STEPS - 1);
    const int8_t brightness = MAX_BRIGHTNESS;
    const uint8_t expected_pwm = (uint8_t)((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
    const uint8_t expected_cool_pwm = expected_pwm;
    const uint8_t expected_warm_pwm = (uint8_t)(expected_pwm * WARM_PWM_RATIO + 0.5);
    const uint8_t expected_hot_pwm  = (uint8_t)(expected_pwm * HOT_PWM_RATIO + 0.5);
    SetWhiteBrightness(brightness);

    thermistor_value = HEATING_WARM_THERM;
    TEST_ASSERT(GetWhitePWM() == expected_warm_pwm);

    thermistor_value = HEATING_HOT_THERM;
    TEST_ASSERT(GetWhitePWM() == expected_hot_pwm);

    thermistor_value = COOLING_COOL_THERM;
    TEST_ASSERT(GetWhitePWM() == expected_cool_pwm);
}

// verify nice case of GetIRPWM
TEST(PWM_Handler, GetIRPWMThermistorNiceCase)
{
    const float IR_pwm_steps = (MAX_IR_PW - MIN_IR_PW) / (BRIGHTNESS_STEPS - 1);
    const int8_t brightness = MAX_BRIGHTNESS;
    const uint8_t expected_pwm = (uint8_t)((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
    const uint8_t expected_cool_pwm = expected_pwm;
    const uint8_t expected_warm_pwm = (uint8_t)(expected_pwm * WARM_PWM_RATIO + 0.5);
    const uint8_t expected_hot_pwm  = (uint8_t)(expected_pwm * HOT_PWM_RATIO + 0.5);
    SetIRBrightness(brightness);

    thermistor_value = HEATING_WARM_THERM;
    TEST_ASSERT(GetIRPWM() == expected_warm_pwm);

    thermistor_value = HEATING_HOT_THERM;
    TEST_ASSERT(GetIRPWM() == expected_hot_pwm);

    thermistor_value = COOLING_COOL_THERM;
    TEST_ASSERT(GetIRPWM() == expected_cool_pwm);
}

/* EDGE CASES */
// verify edge case of DecreaseWhiteBrightness
TEST(PWM_Handler, DecreaseWhiteBrightnessEdgeCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    SetWhiteBrightness(MIN_BRIGHTNESS);
    DecreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == MIN_BRIGHTNESS);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    DecreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == MIN_BRIGHTNESS);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    InitWhitePWM();
    InitIRPWM();

    SetWhiteBrightness(MIN_BRIGHTNESS);
    DecreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == MIN_BRIGHTNESS);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS); // IR should not change

    TEST_ASSERT(pwm_ir.is_running == 0);
    DecreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == MIN_BRIGHTNESS);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);
}

// verify edge case of DecreaseIRBrightness
TEST(PWM_Handler, DecreaseIRBrightnessEdgeCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    SetIRBrightness(MIN_BRIGHTNESS);
    DecreaseIRBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == MIN_BRIGHTNESS);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());

    DecreaseIRBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == MIN_BRIGHTNESS);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());

    InitWhitePWM();
    InitIRPWM();

    SetIRBrightness(MIN_BRIGHTNESS);
    DecreaseIRBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == MIN_BRIGHTNESS);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());

    DecreaseIRBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == MIN_BRIGHTNESS);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());
}

// verify edge case of IncreaseWhiteBrightness
TEST(PWM_Handler, IncreaseWhiteBrightnessEdgeCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    SetWhiteBrightness(MAX_BRIGHTNESS);
    IncreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == MAX_BRIGHTNESS);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    IncreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == MAX_BRIGHTNESS);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    InitWhitePWM();
    InitIRPWM();

    SetWhiteBrightness(MAX_BRIGHTNESS);
    IncreaseWhiteBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == MAX_BRIGHTNESS);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);

    IncreaseWhiteBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == MAX_BRIGHTNESS);
    TEST_ASSERT(pwm_white.is_running == 1);
    TEST_ASSERT(pwm_white.pulse_width == GetWhitePWM());
    TEST_ASSERT(GetIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
    TEST_ASSERT(pwm_ir.is_running == 0);
}

// verify edge case of IncreaseIRBrightness
TEST(PWM_Handler, IncreaseIRBrightnessEdgeCase)
{
    extern PwmStruct pwm_white;
    extern PwmStruct pwm_ir;

    SetIRBrightness(MAX_BRIGHTNESS);
    IncreaseIRBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == MAX_BRIGHTNESS);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());

    IncreaseIRBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == MAX_BRIGHTNESS);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());

    InitWhitePWM();
    InitIRPWM();

    SetIRBrightness(MAX_BRIGHTNESS);
    IncreaseIRBrightness(HOLD);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == MAX_BRIGHTNESS);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());

    IncreaseIRBrightness(PRESS);
    TEST_ASSERT(GetWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
    TEST_ASSERT(pwm_white.is_running == 0);
    TEST_ASSERT(GetIRBrightness()    == MAX_BRIGHTNESS);
    TEST_ASSERT(pwm_ir.is_running == 1);
    TEST_ASSERT(pwm_ir.pulse_width == GetIRPWM());
}

// verify edge case of SetWhiteBrightness
TEST(PWM_Handler, SetWhiteBrightnessEdgeCase)
{
    int8_t brightness = MIN_BRIGHTNESS - 1;
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhiteBrightness() == MIN_BRIGHTNESS);
    brightness = MAX_BRIGHTNESS + 1;
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhiteBrightness() == MAX_BRIGHTNESS);
    brightness = 100;
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhiteBrightness() == MAX_BRIGHTNESS);
    brightness = -75;
    SetWhiteBrightness(brightness);
    TEST_ASSERT(GetWhiteBrightness() == MIN_BRIGHTNESS);
}

// verify edge case of SetIRBrightness
TEST(PWM_Handler, SetIRBrightnessEdgeCase)
{
    int8_t brightness = MIN_BRIGHTNESS - 1;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == MIN_BRIGHTNESS);
    brightness = MAX_BRIGHTNESS + 1;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == MAX_BRIGHTNESS);
    brightness = 52;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == MAX_BRIGHTNESS);
    brightness = -121;
    SetIRBrightness(brightness);
    TEST_ASSERT(GetIRBrightness() == MIN_BRIGHTNESS);
}
/* end pwm_handler tests */

TEST_GROUP_RUNNER(PWM_Handler)
{
    RUN_TEST_CASE(PWM_Handler, DecreaseWhiteBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, DecreaseIRBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, IncreaseWhiteBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, IncreaseIRBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, SetWhitePWMNiceCase);
    RUN_TEST_CASE(PWM_Handler, SetIRPWMNiceCase);
    RUN_TEST_CASE(PWM_Handler, TurnOffWhitePWMNiceCase);
    RUN_TEST_CASE(PWM_Handler, TurnOffIRPWMNiceCase);
    RUN_TEST_CASE(PWM_Handler, SetWhiteBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, SetIRBrightnessNiceCase);
    RUN_TEST_CASE(PWM_Handler, GetWhitePWMNiceCase);
    RUN_TEST_CASE(PWM_Handler, GetIRPWMNiceCase);
    RUN_TEST_CASE(PWM_Handler, GetWhitePWMThermistorNiceCase);
    RUN_TEST_CASE(PWM_Handler, GetIRPWMThermistorNiceCase);
    RUN_TEST_CASE(PWM_Handler, DecreaseWhiteBrightnessEdgeCase);
    RUN_TEST_CASE(PWM_Handler, DecreaseIRBrightnessEdgeCase);
    RUN_TEST_CASE(PWM_Handler, IncreaseWhiteBrightnessEdgeCase);
    RUN_TEST_CASE(PWM_Handler, IncreaseIRBrightnessEdgeCase);
    RUN_TEST_CASE(PWM_Handler, SetWhiteBrightnessEdgeCase);
    RUN_TEST_CASE(PWM_Handler, SetIRBrightnessEdgeCase);
}