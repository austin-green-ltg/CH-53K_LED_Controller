#include "unity_fixture.h"          /* UNITY */
#include "pwm_handler.h"            /* CUT */
#include "temperature_handler.h"    /* CUT */
#include "stm32l412xx-bsp.h"        /* CUT */

#define PRESS (0)
#define HOLD  (1)
enum {LED_VISIBLE = 0, LED_IR = 1};

#define HEATING_WARM_THERM (1000)
#define HEATING_HOT_THERM  (1200)
#define COOLING_COOL_THERM (800)
#define COOLING_WARM_THERM (1000)

extern int16_t thermistor_value_dC;

extern const uint8_t MinBrightness ;
extern const uint8_t MaxBrightness ;
extern const uint8_t HalfBrightness ;
extern const float MinPw ;
extern const float MaxPw ;
extern const float WarmPwmRatio ;
extern const float HotPwmRatio ;

TEST_GROUP ( PWM_Handler );

TEST_SETUP ( PWM_Handler )
{
    /* executed before *every* non-skipped test */
    InitPwm();
}

TEST_TEAR_DOWN ( PWM_Handler )
{
    /* executed after *every* non-skipped and non-failing test */
    thermistor_value_dC = 0; // return to default value
    GetTemperatureRange(); // set thermistor range
}

/* start pwm_handler tests */
/* NICE CASES */
// verify nice case of DecreaseBrightness
TEST ( PWM_Handler, DecreaseBrightnessNiceCase )
{
    extern PwmStruct pwm;

    /* Visible Tests */
    DecreaseBrightness ( PRESS, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == HalfBrightness - 1 );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );
    TEST_ASSERT ( GetBrightness ( LED_IR ) ==
                  HalfBrightness ); // IR should not change

    DecreaseBrightness ( HOLD, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == HalfBrightness - 1 -
                  HOLD_BRIGHTNESS_JUMP );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );
    TEST_ASSERT ( GetBrightness ( LED_IR ) ==
                  HalfBrightness ); // IR should not change

    InitPwm();

    DecreaseBrightness ( HOLD, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == HalfBrightness -
                  HOLD_BRIGHTNESS_JUMP );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );
    TEST_ASSERT ( GetBrightness ( LED_IR ) ==
                  HalfBrightness ); // IR should not change

    DecreaseBrightness ( PRESS, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == HalfBrightness -
                  HOLD_BRIGHTNESS_JUMP - 1 );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );
    TEST_ASSERT ( GetBrightness ( LED_IR ) ==
                  HalfBrightness ); // IR should not change

    /* IR Tests */
    InitPwm();

    DecreaseBrightness ( PRESS, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) ==
                  HalfBrightness ); // Visible should not change
    TEST_ASSERT ( GetBrightness ( LED_IR ) == HalfBrightness - 1 );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );

    DecreaseBrightness ( HOLD, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) ==
                  HalfBrightness ); // Visible should not change
    TEST_ASSERT ( GetBrightness ( LED_IR ) == HalfBrightness - 1 -
                  HOLD_BRIGHTNESS_JUMP );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );

    InitPwm();

    DecreaseBrightness ( HOLD, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) ==
                  HalfBrightness ); // Visible should not change
    TEST_ASSERT ( GetBrightness ( LED_IR ) == HalfBrightness -
                  HOLD_BRIGHTNESS_JUMP );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );

    DecreaseBrightness ( PRESS, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) ==
                  HalfBrightness ); // Visible should not change
    TEST_ASSERT ( GetBrightness ( LED_IR ) == HalfBrightness -
                  HOLD_BRIGHTNESS_JUMP - 1 );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );
}

// verify nice case of IncreaseBrightness
TEST ( PWM_Handler, IncreaseBrightnessNiceCase )
{
    extern PwmStruct pwm;

    /* Visible Tests */
    IncreaseBrightness ( PRESS, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == HalfBrightness + 1 );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );
    TEST_ASSERT ( GetBrightness ( LED_IR ) ==
                  HalfBrightness ); // IR should not change

    IncreaseBrightness ( HOLD, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == HalfBrightness + 1 +
                  HOLD_BRIGHTNESS_JUMP );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );
    TEST_ASSERT ( GetBrightness ( LED_IR ) ==
                  HalfBrightness ); // IR should not change

    InitPwm();

    IncreaseBrightness ( HOLD, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == HalfBrightness +
                  HOLD_BRIGHTNESS_JUMP );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );
    TEST_ASSERT ( GetBrightness ( LED_IR ) ==
                  HalfBrightness ); // IR should not change

    IncreaseBrightness ( PRESS, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == HalfBrightness +
                  HOLD_BRIGHTNESS_JUMP + 1 );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );
    TEST_ASSERT ( GetBrightness ( LED_IR ) ==
                  HalfBrightness ); // IR should not change

    /* IR Tests */
    InitPwm();

    IncreaseBrightness ( PRESS, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) ==
                  HalfBrightness ); // Visible should not change
    TEST_ASSERT ( GetBrightness ( LED_IR ) == HalfBrightness + 1 );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );

    IncreaseBrightness ( HOLD, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) ==
                  HalfBrightness ); // Visible should not change
    TEST_ASSERT ( GetBrightness ( LED_IR ) == HalfBrightness + 1 +
                  HOLD_BRIGHTNESS_JUMP );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );

    InitPwm();

    IncreaseBrightness ( HOLD, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) ==
                  HalfBrightness ); // Visible should not change
    TEST_ASSERT ( GetBrightness ( LED_IR ) == HalfBrightness +
                  HOLD_BRIGHTNESS_JUMP );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );

    IncreaseBrightness ( PRESS, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) ==
                  HalfBrightness ); // Visible should not change
    TEST_ASSERT ( GetBrightness ( LED_IR ) == HalfBrightness +
                  HOLD_BRIGHTNESS_JUMP + 1 );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );
}

// verify nice case of SetPwm
TEST ( PWM_Handler, SetPwmNiceCase )
{
    extern PwmStruct pwm;

    /* Visible Tests */
    SetPwm ( LED_VISIBLE );
    // Visible should be on and be set to visible pwm
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );

    /* IR Tests */

    SetPwm ( LED_IR );
    // IR should be on and be set to IR pwm
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );
}

// verify nice case of TurnOffPwm
TEST ( PWM_Handler, TurnOffPwmNiceCase )
{
    extern PwmStruct pwm;

    SetPwm ( LED_VISIBLE );
    TurnOffPwm();
    // Visible should be off
    TEST_ASSERT ( pwm.is_running == 0 );
}

// verify nice case of SetBrightness
TEST ( PWM_Handler, SetBrightnessNiceCase )
{

    /* Visible Tests */
    int8_t brightness = MinBrightness;
    SetBrightness ( brightness, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == brightness );
    brightness = MaxBrightness;
    SetBrightness ( brightness, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == brightness );
    brightness = HalfBrightness;
    SetBrightness ( brightness, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == brightness );
    brightness = 32;
    SetBrightness ( brightness, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == brightness );
    brightness = 16;
    SetBrightness ( brightness, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == brightness );

    /* IR Tests */
    brightness = MinBrightness;
    SetBrightness ( brightness, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_IR ) == brightness );
    brightness = MaxBrightness;
    SetBrightness ( brightness, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_IR ) == brightness );
    brightness = HalfBrightness;
    SetBrightness ( brightness, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_IR ) == brightness );
    brightness = 32;
    SetBrightness ( brightness, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_IR ) == brightness );
    brightness = 16;
    SetBrightness ( brightness, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_IR ) == brightness );
}

// verify nice case of GetPwm
TEST ( PWM_Handler, GetPwmNiceCase )
{

    /* Visible Tests */
    const float visible_pwm_steps = ( MaxPw - MinPw ) / ( ( float ) BRIGHTNESS_STEPS
                                    - 1.0f );
    int8_t brightness = MinBrightness;
    uint8_t expected_pwm = ( uint8_t ) ( ( brightness * visible_pwm_steps ) + MinPw
                                         + 0.5f );
    SetBrightness ( brightness, LED_VISIBLE );
    TEST_ASSERT_EQUAL_UINT8 ( expected_pwm, GetPwm ( LED_VISIBLE ) );
    brightness = MaxBrightness;
    expected_pwm = ( uint8_t ) ( ( brightness * visible_pwm_steps ) + MinPw +
                                 0.5f );
    SetBrightness ( brightness, LED_VISIBLE );
    TEST_ASSERT_EQUAL_UINT8 ( expected_pwm, GetPwm ( LED_VISIBLE ) );
    brightness = HalfBrightness;
    expected_pwm = ( uint8_t ) ( ( brightness * visible_pwm_steps ) + MinPw +
                                 0.5f );
    SetBrightness ( brightness, LED_VISIBLE );
    TEST_ASSERT_EQUAL_UINT8 ( expected_pwm, GetPwm ( LED_VISIBLE ) );
    brightness = 17;
    expected_pwm = ( uint8_t ) ( ( brightness * visible_pwm_steps ) + MinPw +
                                 0.5f );
    SetBrightness ( brightness, LED_VISIBLE );
    TEST_ASSERT_EQUAL_UINT8 ( expected_pwm, GetPwm ( LED_VISIBLE ) );
    brightness = 42;
    expected_pwm = ( uint8_t ) ( ( brightness * visible_pwm_steps ) + MinPw +
                                 0.5f );
    SetBrightness ( brightness, LED_VISIBLE );
    TEST_ASSERT_EQUAL_UINT8 ( expected_pwm, GetPwm ( LED_VISIBLE ) );

    /* IR Tests */
    const float IR_pwm_steps = ( MaxPw - MinPw ) / ( ( float ) BRIGHTNESS_STEPS -
                               1.0f );
    brightness = MinBrightness;
    expected_pwm = ( uint8_t ) ( ( brightness * IR_pwm_steps ) + MinPw + 0.5f );
    SetBrightness ( brightness, LED_IR );
    TEST_ASSERT_EQUAL_UINT8 ( expected_pwm, GetPwm ( LED_IR ) );
    brightness = MaxBrightness;
    expected_pwm = ( uint8_t ) ( ( brightness * IR_pwm_steps ) + MinPw + 0.5f );
    SetBrightness ( brightness, LED_IR );
    TEST_ASSERT_EQUAL_UINT8 ( expected_pwm, GetPwm ( LED_IR ) );
    brightness = HalfBrightness;
    expected_pwm = ( uint8_t ) ( ( brightness * IR_pwm_steps ) + MinPw + 0.5f );
    SetBrightness ( brightness, LED_IR );
    TEST_ASSERT_EQUAL_UINT8 ( expected_pwm, GetPwm ( LED_IR ) );
    brightness = 12;
    expected_pwm = ( uint8_t ) ( ( brightness * IR_pwm_steps ) + MinPw + 0.5f );
    SetBrightness ( brightness, LED_IR );
    TEST_ASSERT_EQUAL_UINT8 ( expected_pwm, GetPwm ( LED_IR ) );
    brightness = 39;
    expected_pwm = ( uint8_t ) ( ( brightness * IR_pwm_steps ) + MinPw + 0.5f );
    SetBrightness ( brightness, LED_IR );
    TEST_ASSERT_EQUAL_UINT8 ( expected_pwm, GetPwm ( LED_IR ) );
}

// verify nice case of GetPwm
TEST ( PWM_Handler, GetPwmThermistorNiceCase )
{

    /* Visible Tests */
    const float visible_pwm_steps = ( MaxPw - MinPw ) / ( ( float ) BRIGHTNESS_STEPS
                                    - 1.0f );
    const int8_t brightness = MaxBrightness;
    const uint8_t expected_vis_pwm = ( uint8_t ) ( ( brightness *
                                     visible_pwm_steps ) + MinPw + 0.5f );
    const uint8_t expected_cool_vis_pwm = expected_vis_pwm;
    const uint8_t expected_warm_vis_pwm = ( uint8_t ) ( expected_vis_pwm *
                                          WarmPwmRatio + 0.5 );
    const uint8_t expected_hot_vis_pwm = ( uint8_t ) ( expected_vis_pwm *
                                          HotPwmRatio + 0.5 );
    SetBrightness ( brightness, LED_VISIBLE );

    thermistor_value_dC = HEATING_WARM_THERM;
    TEST_ASSERT ( GetPwm ( LED_VISIBLE ) == expected_warm_vis_pwm );

    thermistor_value_dC = HEATING_HOT_THERM;
    TEST_ASSERT ( GetPwm ( LED_VISIBLE ) == expected_hot_vis_pwm );

    thermistor_value_dC = COOLING_COOL_THERM;
    TEST_ASSERT ( GetPwm ( LED_VISIBLE ) == expected_cool_vis_pwm );

    /* IR Tests */
    const float IR_pwm_steps = ( MaxPw - MinPw ) / ( ( float ) BRIGHTNESS_STEPS -
                               1.0f );
    const uint8_t expected_IR_pwm = ( uint8_t ) ( ( brightness * IR_pwm_steps ) +
                                    MinPw + 0.5f );
    const uint8_t expected_cool_IR_pwm = expected_IR_pwm;
    const uint8_t expected_warm_IR_pwm = ( uint8_t ) ( expected_IR_pwm *
                                         WarmPwmRatio + 0.5 );
    const uint8_t expected_hot_IR_pwm = ( uint8_t ) ( expected_IR_pwm * HotPwmRatio
                                         + 0.5 );
    SetBrightness ( brightness, LED_IR );

    thermistor_value_dC = HEATING_WARM_THERM;
    TEST_ASSERT ( GetPwm ( LED_IR ) == expected_warm_IR_pwm );

    thermistor_value_dC = HEATING_HOT_THERM;
    TEST_ASSERT ( GetPwm ( LED_IR ) == expected_hot_IR_pwm );

    thermistor_value_dC = COOLING_COOL_THERM;
    TEST_ASSERT ( GetPwm ( LED_IR ) == expected_cool_IR_pwm );
}

/* EDGE CASES */
// verify edge case of DecreaseBrightness
TEST ( PWM_Handler, DecreaseBrightnessEdgeCase )
{
    extern PwmStruct pwm;

    /* Visible Tests */
    SetBrightness ( MinBrightness, LED_VISIBLE );
    DecreaseBrightness ( PRESS, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == MinBrightness );
    TEST_ASSERT ( pwm.is_running == 0 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );
    TEST_ASSERT ( GetBrightness ( LED_IR ) ==
                  HalfBrightness ); // IR should not change
    TEST_ASSERT ( pwm.is_running == 0 );

    DecreaseBrightness ( HOLD, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == MinBrightness );
    TEST_ASSERT ( pwm.is_running == 0 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );
    TEST_ASSERT ( GetBrightness ( LED_IR ) ==
                  HalfBrightness ); // IR should not change
    TEST_ASSERT ( pwm.is_running == 0 );

    InitPwm();

    SetBrightness ( MinBrightness, LED_VISIBLE );
    DecreaseBrightness ( HOLD, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == MinBrightness );
    TEST_ASSERT ( pwm.is_running == 0 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );
    TEST_ASSERT ( GetBrightness ( LED_IR ) ==
                  HalfBrightness ); // IR should not change

    TEST_ASSERT ( pwm.is_running == 0 );
    DecreaseBrightness ( PRESS, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == MinBrightness );
    TEST_ASSERT ( pwm.is_running == 0 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );
    TEST_ASSERT ( GetBrightness ( LED_IR ) ==
                  HalfBrightness ); // IR should not change
    TEST_ASSERT ( pwm.is_running == 0 );

    /* IR Tests */
    InitPwm();

    SetBrightness ( MinBrightness, LED_IR );
    DecreaseBrightness ( PRESS, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) ==
                  HalfBrightness ); // Visible should not change
    TEST_ASSERT ( pwm.is_running == 0 );
    TEST_ASSERT ( GetBrightness ( LED_IR ) == MinBrightness );
    TEST_ASSERT ( pwm.is_running == 0 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );

    DecreaseBrightness ( HOLD, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) ==
                  HalfBrightness ); // Visible should not change
    TEST_ASSERT ( pwm.is_running == 0 );
    TEST_ASSERT ( GetBrightness ( LED_IR ) == MinBrightness );
    TEST_ASSERT ( pwm.is_running == 0 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );

    InitPwm();

    SetBrightness ( MinBrightness, LED_IR );
    DecreaseBrightness ( HOLD, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) ==
                  HalfBrightness ); // Visible should not change
    TEST_ASSERT ( pwm.is_running == 0 );
    TEST_ASSERT ( GetBrightness ( LED_IR ) == MinBrightness );
    TEST_ASSERT ( pwm.is_running == 0 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );

    DecreaseBrightness ( PRESS, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) ==
                  HalfBrightness ); // Visible should not change
    TEST_ASSERT ( pwm.is_running == 0 );
    TEST_ASSERT ( GetBrightness ( LED_IR ) == MinBrightness );
    TEST_ASSERT ( pwm.is_running == 0 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );
}

// verify edge case of IncreaseBrightness
TEST ( PWM_Handler, IncreaseBrightnessEdgeCase )
{
    extern PwmStruct pwm;

    /* Visible Tests */
    SetBrightness ( MaxBrightness, LED_VISIBLE );
    IncreaseBrightness ( PRESS, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == MaxBrightness );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );
    TEST_ASSERT ( GetBrightness ( LED_IR ) ==
                  HalfBrightness ); // IR should not change

    IncreaseBrightness ( HOLD, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == MaxBrightness );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );
    TEST_ASSERT ( GetBrightness ( LED_IR ) ==
                  HalfBrightness ); // IR should not change

    InitPwm();

    SetBrightness ( MaxBrightness, LED_VISIBLE );
    IncreaseBrightness ( HOLD, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == MaxBrightness );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );
    TEST_ASSERT ( GetBrightness ( LED_IR ) ==
                  HalfBrightness ); // IR should not change

    IncreaseBrightness ( PRESS, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == MaxBrightness );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_VISIBLE ) );
    TEST_ASSERT ( GetBrightness ( LED_IR ) ==
                  HalfBrightness ); // IR should not change

    /* IR Tests */
    InitPwm();

    SetBrightness ( MaxBrightness, LED_IR );
    IncreaseBrightness ( PRESS, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) ==
                  HalfBrightness ); // Visible should not change
    TEST_ASSERT ( GetBrightness ( LED_IR ) == MaxBrightness );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );

    IncreaseBrightness ( HOLD, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) ==
                  HalfBrightness ); // Visible should not change
    TEST_ASSERT ( GetBrightness ( LED_IR ) == MaxBrightness );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );

    InitPwm();

    SetBrightness ( MaxBrightness, LED_IR );
    IncreaseBrightness ( HOLD, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) ==
                  HalfBrightness ); // Visible should not change
    TEST_ASSERT ( GetBrightness ( LED_IR ) == MaxBrightness );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );

    IncreaseBrightness ( PRESS, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) ==
                  HalfBrightness ); // Visible should not change
    TEST_ASSERT ( GetBrightness ( LED_IR ) == MaxBrightness );
    TEST_ASSERT ( pwm.is_running == 1 );
    TEST_ASSERT ( pwm.pulse_width == GetPwm ( LED_IR ) );
}

// verify edge case of SetBrightness
TEST ( PWM_Handler, SetBrightnessEdgeCase )
{

    /* Visible Tests */
    int8_t brightness = MinBrightness - 1;
    SetBrightness ( brightness, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == MinBrightness );
    brightness = MaxBrightness + 1;
    SetBrightness ( brightness, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == MaxBrightness );
    brightness = 100;
    SetBrightness ( brightness, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == MaxBrightness );
    brightness = -75;
    SetBrightness ( brightness, LED_VISIBLE );
    TEST_ASSERT ( GetBrightness ( LED_VISIBLE ) == MinBrightness );

    /* IR Tests */
    brightness = MinBrightness - 1;
    SetBrightness ( brightness, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_IR ) == MinBrightness );
    brightness = MaxBrightness + 1;
    SetBrightness ( brightness, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_IR ) == MaxBrightness );
    brightness = 52;
    SetBrightness ( brightness, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_IR ) == MaxBrightness );
    brightness = -121;
    SetBrightness ( brightness, LED_IR );
    TEST_ASSERT ( GetBrightness ( LED_IR ) == MinBrightness );
}

/* end pwm_handler tests */

TEST_GROUP_RUNNER ( PWM_Handler )
{
    RUN_TEST_CASE ( PWM_Handler, DecreaseBrightnessNiceCase );
    RUN_TEST_CASE ( PWM_Handler, IncreaseBrightnessNiceCase );
    RUN_TEST_CASE ( PWM_Handler, SetPwmNiceCase );
    RUN_TEST_CASE ( PWM_Handler, TurnOffPwmNiceCase );
    RUN_TEST_CASE ( PWM_Handler, SetBrightnessNiceCase );
    RUN_TEST_CASE ( PWM_Handler, GetPwmNiceCase );
    RUN_TEST_CASE ( PWM_Handler, GetPwmThermistorNiceCase );
    RUN_TEST_CASE ( PWM_Handler, DecreaseBrightnessEdgeCase );
    RUN_TEST_CASE ( PWM_Handler, IncreaseBrightnessEdgeCase );
    RUN_TEST_CASE ( PWM_Handler, SetBrightnessEdgeCase );
}
