/*============================================================================
* ET: embedded test; very simple test example
============================================================================*/
// #include "pwm_handler.h"
#include "et.h" /* ET: embedded test */
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

void setup(void) {
    /* executed before *every* non-skipped test */
    initWhitePWM();
    initIRPWM();
}

void teardown(void) {
    /* executed after *every* non-skipped and non-failing test */
    thermistor_value = 0; // return to default value
    get_temperature_range(); // set thermistor range
}

/* test group --------------------------------------------------------------*/
TEST_GROUP("PWM Handler")
{

    /* start pwm_handler tests */
    /* NICE CASES */
    // verify nice case of decreaseWhiteBrightness
    TEST("decreaseWhiteBrightness nice case")
    {
        extern pwmStruct pwm_white;
        extern pwmStruct pwm_ir;

        decreaseWhiteBrightness(PRESS);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS - 1);
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
        VERIFY(pwm_ir.running == 0);

        decreaseWhiteBrightness(HOLD);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS - 1 - HOLD_BRIGHTNESS_JUMP);
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
        VERIFY(pwm_ir.running == 0);

        initWhitePWM();
        initIRPWM();

        decreaseWhiteBrightness(HOLD);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS - HOLD_BRIGHTNESS_JUMP);
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change

        VERIFY(pwm_ir.running == 0);
        decreaseWhiteBrightness(PRESS);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS - HOLD_BRIGHTNESS_JUMP - 1);
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
        VERIFY(pwm_ir.running == 0);
    }

    // verify nice case of decreaseIRBrightness
    TEST("decreaseIRBrightness nice case")
    {
        extern pwmStruct pwm_white;
        extern pwmStruct pwm_ir;

        decreaseIRBrightness(PRESS);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
        VERIFY(pwm_white.running == 0);
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS - 1);
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());

        decreaseIRBrightness(HOLD);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
        VERIFY(pwm_white.running == 0);
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS - 1 - HOLD_BRIGHTNESS_JUMP);
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());

        initWhitePWM();
        initIRPWM();

        decreaseIRBrightness(HOLD);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
        VERIFY(pwm_white.running == 0);
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS - HOLD_BRIGHTNESS_JUMP);
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());

        decreaseIRBrightness(PRESS);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
        VERIFY(pwm_white.running == 0);
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS - HOLD_BRIGHTNESS_JUMP - 1);
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());
    }

    // verify nice case of increaseWhiteBrightness
    TEST("increaseWhiteBrightness nice case")
    {
        extern pwmStruct pwm_white;
        extern pwmStruct pwm_ir;

        increaseWhiteBrightness(PRESS);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS + 1);
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
        VERIFY(pwm_ir.running == 0);

        increaseWhiteBrightness(HOLD);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS + 1 + HOLD_BRIGHTNESS_JUMP);
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
        VERIFY(pwm_ir.running == 0);

        initWhitePWM();
        initIRPWM();

        increaseWhiteBrightness(HOLD);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS + HOLD_BRIGHTNESS_JUMP);
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
        VERIFY(pwm_ir.running == 0);

        increaseWhiteBrightness(PRESS);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS + HOLD_BRIGHTNESS_JUMP + 1);
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
        VERIFY(pwm_ir.running == 0);
    }

    // verify nice case of increaseIRBrightness
    TEST("increaseIRBrightness nice case")
    {
        extern pwmStruct pwm_white;
        extern pwmStruct pwm_ir;

        increaseIRBrightness(PRESS);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
        VERIFY(pwm_white.running == 0);
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS + 1);
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());

        increaseIRBrightness(HOLD);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
        VERIFY(pwm_white.running == 0);
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS + 1 + HOLD_BRIGHTNESS_JUMP);
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());

        initWhitePWM();
        initIRPWM();

        increaseIRBrightness(HOLD);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
        VERIFY(pwm_white.running == 0);
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS + HOLD_BRIGHTNESS_JUMP);
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());

        increaseIRBrightness(PRESS);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
        VERIFY(pwm_white.running == 0);
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS + HOLD_BRIGHTNESS_JUMP + 1);
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());
    }

    // verify nice case of setWhitePWM
    TEST("setWhitePWM nice case")
    {
        extern pwmStruct pwm_white;
        extern pwmStruct pwm_ir;

        setWhitePWM();
        // White should be on and be set to white pwm
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        // IR should be off
        VERIFY(pwm_ir.running == 0);
    }

    // verify nice case of setIRPWM
    TEST("setIRPWM nice case")
    {
        extern pwmStruct pwm_white;
        extern pwmStruct pwm_ir;

        setIRPWM();
        // IR should be on and be set to IR pwm
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());
        // White should be off
        VERIFY(pwm_white.running == 0);
    }

    // verify nice case of turnOffWhitePWM
    TEST("turnOffWhitePWM nice case")
    {
        extern pwmStruct pwm_white;
        extern pwmStruct pwm_ir;

        setWhitePWM();
        turnOffWhitePWM();
        // White should be off
        VERIFY(pwm_white.running == 0);
    }

    // verify nice case of turnOffIRPWM
    TEST("turnOffIRPWM nice case")
    {
        extern pwmStruct pwm_white;
        extern pwmStruct pwm_ir;

        setIRPWM();
        turnOffIRPWM();
        // IR should be off
        VERIFY(pwm_ir.running == 0);
    }

    // verify nice case of setWhiteBrightness
    TEST("setWhiteBrightness nice case")
    {
        int8_t brightness = MIN_BRIGHTNESS;
        setWhiteBrightness(brightness);
        VERIFY(getWhiteBrightness() == brightness);
        brightness = MAX_BRIGHTNESS;
        setWhiteBrightness(brightness);
        VERIFY(getWhiteBrightness() == brightness);
        brightness = HALF_BRIGHTNESS;
        setWhiteBrightness(brightness);
        VERIFY(getWhiteBrightness() == brightness);
        brightness = 32;
        setWhiteBrightness(brightness);
        VERIFY(getWhiteBrightness() == brightness);
        brightness = 16;
        setWhiteBrightness(brightness);
        VERIFY(getWhiteBrightness() == brightness);
    }

    // verify nice case of setIRBrightness
    TEST("setIRBrightness nice case")
    {
        int8_t brightness = MIN_BRIGHTNESS;
        setIRBrightness(brightness);
        VERIFY(getIRBrightness() == brightness);
        brightness = MAX_BRIGHTNESS;
        setIRBrightness(brightness);
        VERIFY(getIRBrightness() == brightness);
        brightness = HALF_BRIGHTNESS;
        setIRBrightness(brightness);
        VERIFY(getIRBrightness() == brightness);
        brightness = 32;
        setIRBrightness(brightness);
        VERIFY(getIRBrightness() == brightness);
        brightness = 16;
        setIRBrightness(brightness);
        VERIFY(getIRBrightness() == brightness);
    }

    // verify nice case of getWhitePWM
    TEST("getWhitePWM nice case")
    {
        const float white_pwm_steps = (MAX_WHITE_PW - MIN_WHITE_PW) / (BRIGHTNESS_STEPS - 1);
        int8_t brightness = MIN_BRIGHTNESS;
        uint8_t expected_pwm = ((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
        setWhiteBrightness(brightness);
        VERIFY(getWhitePWM() == expected_pwm);
        brightness = MAX_BRIGHTNESS;
        expected_pwm = ((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
        setWhiteBrightness(brightness);
        VERIFY(getWhitePWM() == expected_pwm);
        brightness = HALF_BRIGHTNESS;
        expected_pwm = ((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
        setWhiteBrightness(brightness);
        VERIFY(getWhitePWM() == expected_pwm);
        brightness = 17;
        expected_pwm = ((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
        setWhiteBrightness(brightness);
        VERIFY(getWhitePWM() == expected_pwm);
        brightness = 42;
        expected_pwm = ((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
        setWhiteBrightness(brightness);
        VERIFY(getWhitePWM() == expected_pwm);
    }

    // verify nice case of getIRPWM
    TEST("getIRPWM nice case")
    {
        const float IR_pwm_steps = (MAX_IR_PW - MIN_IR_PW) / (BRIGHTNESS_STEPS - 1);
        int8_t brightness = MIN_BRIGHTNESS;
        uint8_t expected_pwm = ((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
        setIRBrightness(brightness);
        VERIFY(getIRPWM() == expected_pwm);
        brightness = MAX_BRIGHTNESS;
        expected_pwm = ((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
        setIRBrightness(brightness);
        VERIFY(getIRPWM() == expected_pwm);
        brightness = HALF_BRIGHTNESS;
        expected_pwm = ((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
        setIRBrightness(brightness);
        VERIFY(getIRPWM() == expected_pwm);
        brightness = 12;
        expected_pwm = ((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
        setIRBrightness(brightness);
        VERIFY(getIRPWM() == expected_pwm);
        brightness = 39;
        expected_pwm = ((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
        setIRBrightness(brightness);
        VERIFY(getIRPWM() == expected_pwm);
    }

    // verify nice case of getWhitePWM
    TEST("getWhitePWM Thermistor nice case")
    {
        const float white_pwm_steps = (MAX_WHITE_PW - MIN_WHITE_PW) / (BRIGHTNESS_STEPS - 1);
        const int8_t brightness = MAX_BRIGHTNESS;
        const uint8_t expected_pwm = ((brightness * white_pwm_steps) + MIN_WHITE_PW + 0.5f);
        const uint8_t expected_cool_pwm = expected_pwm;
        const uint8_t expected_warm_pwm = (uint8_t)(expected_pwm * WARM_PWM_RATIO);
        const uint8_t expected_hot_pwm  = (uint8_t)(expected_pwm * HOT_PWM_RATIO);
        setWhiteBrightness(brightness);

        thermistor_value = HEATING_WARM_THERM;
        VERIFY(getWhitePWM() == expected_warm_pwm);

        thermistor_value = HEATING_HOT_THERM;
        VERIFY(getWhitePWM() == expected_hot_pwm);

        thermistor_value = COOLING_COOL_THERM;
        VERIFY(getWhitePWM() == expected_cool_pwm);
    }

    // verify nice case of getIRPWM
    TEST("getIRPWM Thermistor nice case")
    {
        const float IR_pwm_steps = (MAX_IR_PW - MIN_IR_PW) / (BRIGHTNESS_STEPS - 1);
        const int8_t brightness = MAX_BRIGHTNESS;
        const uint8_t expected_pwm = ((brightness * IR_pwm_steps) + MIN_IR_PW + 0.5f);
        const uint8_t expected_cool_pwm = expected_pwm;
        const uint8_t expected_warm_pwm = (uint8_t)(expected_pwm * WARM_PWM_RATIO);
        const uint8_t expected_hot_pwm  = (uint8_t)(expected_pwm * HOT_PWM_RATIO);
        setIRBrightness(brightness);

        thermistor_value = HEATING_WARM_THERM;
        VERIFY(getIRPWM() == expected_warm_pwm);

        thermistor_value = HEATING_HOT_THERM;
        VERIFY(getIRPWM() == expected_hot_pwm);

        thermistor_value = COOLING_COOL_THERM;
        VERIFY(getIRPWM() == expected_cool_pwm);
    }

    /* EDGE CASES */
    // verify edge case of decreaseWhiteBrightness
    TEST("decreaseWhiteBrightness edge case")
    {
        extern pwmStruct pwm_white;
        extern pwmStruct pwm_ir;

        setWhiteBrightness(MIN_BRIGHTNESS);
        decreaseWhiteBrightness(PRESS);
        VERIFY(getWhiteBrightness() == MIN_BRIGHTNESS);
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
        VERIFY(pwm_ir.running == 0);

        decreaseWhiteBrightness(HOLD);
        VERIFY(getWhiteBrightness() == MIN_BRIGHTNESS);
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
        VERIFY(pwm_ir.running == 0);

        initWhitePWM();
        initIRPWM();

        setWhiteBrightness(MIN_BRIGHTNESS);
        decreaseWhiteBrightness(HOLD);
        VERIFY(getWhiteBrightness() == MIN_BRIGHTNESS);
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change

        VERIFY(pwm_ir.running == 0);
        decreaseWhiteBrightness(PRESS);
        VERIFY(getWhiteBrightness() == MIN_BRIGHTNESS);
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
        VERIFY(pwm_ir.running == 0);
    }

    // verify edge case of decreaseIRBrightness
    TEST("decreaseIRBrightness edge case")
    {
        extern pwmStruct pwm_white;
        extern pwmStruct pwm_ir;

        setIRBrightness(MIN_BRIGHTNESS);
        decreaseIRBrightness(PRESS);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
        VERIFY(pwm_white.running == 0);
        VERIFY(getIRBrightness()    == MIN_BRIGHTNESS);
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());

        decreaseIRBrightness(HOLD);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
        VERIFY(pwm_white.running == 0);
        VERIFY(getIRBrightness()    == MIN_BRIGHTNESS);
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());

        initWhitePWM();
        initIRPWM();

        setIRBrightness(MIN_BRIGHTNESS);
        decreaseIRBrightness(HOLD);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
        VERIFY(pwm_white.running == 0);
        VERIFY(getIRBrightness()    == MIN_BRIGHTNESS);
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());

        decreaseIRBrightness(PRESS);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
        VERIFY(pwm_white.running == 0);
        VERIFY(getIRBrightness()    == MIN_BRIGHTNESS);
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());
    }

    // verify edge case of increaseWhiteBrightness
    TEST("increaseWhiteBrightness edge case")
    {
        extern pwmStruct pwm_white;
        extern pwmStruct pwm_ir;

        setWhiteBrightness(MAX_BRIGHTNESS);
        increaseWhiteBrightness(PRESS);
        VERIFY(getWhiteBrightness() == MAX_BRIGHTNESS);
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
        VERIFY(pwm_ir.running == 0);

        increaseWhiteBrightness(HOLD);
        VERIFY(getWhiteBrightness() == MAX_BRIGHTNESS);
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
        VERIFY(pwm_ir.running == 0);

        initWhitePWM();
        initIRPWM();

        setWhiteBrightness(MAX_BRIGHTNESS);
        increaseWhiteBrightness(HOLD);
        VERIFY(getWhiteBrightness() == MAX_BRIGHTNESS);
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
        VERIFY(pwm_ir.running == 0);

        increaseWhiteBrightness(PRESS);
        VERIFY(getWhiteBrightness() == MAX_BRIGHTNESS);
        VERIFY(pwm_white.running == 1);
        VERIFY(pwm_white.pulse_width == getWhitePWM());
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS); // IR should not change
        VERIFY(pwm_ir.running == 0);
    }

    // verify edge case of increaseIRBrightness
    TEST("increaseIRBrightness edge case")
    {
        extern pwmStruct pwm_white;
        extern pwmStruct pwm_ir;

        setIRBrightness(MAX_BRIGHTNESS);
        increaseIRBrightness(PRESS);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
        VERIFY(pwm_white.running == 0);
        VERIFY(getIRBrightness()    == MAX_BRIGHTNESS);
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());

        increaseIRBrightness(HOLD);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
        VERIFY(pwm_white.running == 0);
        VERIFY(getIRBrightness()    == MAX_BRIGHTNESS);
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());

        initWhitePWM();
        initIRPWM();

        setIRBrightness(MAX_BRIGHTNESS);
        increaseIRBrightness(HOLD);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
        VERIFY(pwm_white.running == 0);
        VERIFY(getIRBrightness()    == MAX_BRIGHTNESS);
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());

        increaseIRBrightness(PRESS);
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS); // White should not change
        VERIFY(pwm_white.running == 0);
        VERIFY(getIRBrightness()    == MAX_BRIGHTNESS);
        VERIFY(pwm_ir.running == 1);
        VERIFY(pwm_ir.pulse_width == getIRPWM());
    }

    // verify edge case of setWhiteBrightness
    TEST("setWhiteBrightness edge case")
    {
        int8_t brightness = MIN_BRIGHTNESS - 1;
        setWhiteBrightness(brightness);
        VERIFY(getWhiteBrightness() == MIN_BRIGHTNESS);
        brightness = MAX_BRIGHTNESS + 1;
        setWhiteBrightness(brightness);
        VERIFY(getWhiteBrightness() == MAX_BRIGHTNESS);
        brightness = 100;
        setWhiteBrightness(brightness);
        VERIFY(getWhiteBrightness() == MAX_BRIGHTNESS);
        brightness = -75;
        setWhiteBrightness(brightness);
        VERIFY(getWhiteBrightness() == MIN_BRIGHTNESS);
    }

    // verify edge case of setIRBrightness
    TEST("setIRBrightness edge case")
    {
        int8_t brightness = MIN_BRIGHTNESS - 1;
        setIRBrightness(brightness);
        VERIFY(getIRBrightness() == MIN_BRIGHTNESS);
        brightness = MAX_BRIGHTNESS + 1;
        setIRBrightness(brightness);
        VERIFY(getIRBrightness() == MAX_BRIGHTNESS);
        brightness = 52;
        setIRBrightness(brightness);
        VERIFY(getIRBrightness() == MAX_BRIGHTNESS);
        brightness = -121;
        setIRBrightness(brightness);
        VERIFY(getIRBrightness() == MIN_BRIGHTNESS);
    }
    /* end pwm_handler tests */

} /* TEST_GROUP() */