/*============================================================================
* ET: embedded test; very simple test example
============================================================================*/
// #include "pwm_handler.h"
#include "et.h" /* ET: embedded test */
#include "button_handler.h"  /* CUT */
#include "delay_handler.h"  /* CUT */
#include "pwm_handler.h"    /* CUT */
#include "thermistor_handler.h"    /* CUT */
#include <stdio.h>

#define PRESS (0)
#define HOLD  (1)

void setup(void) {
    /* executed before *every* non-skipped test */
    initWhitePWM();
    initIRPWM();
}

void teardown(void) {
    /* executed after *every* non-skipped and non-failing test */
}

/* test group --------------------------------------------------------------*/
TEST_GROUP("Requirements")
{

    /* start requirements tests */
    // Init at 50% brightness
    // Verify Initial Values
    TEST("Initial Values")
    {
        VERIFY(getWhiteBrightness() == HALF_BRIGHTNESS);
        VERIFY(getIRBrightness()    == HALF_BRIGHTNESS);
        // There are 49 steps from min brightness, find brightness that is halfway in discrete steps
        const uint8_t white_pwm_init = (MAX_WHITE_PW - MIN_WHITE_PW) / (BRIGHTNESS_STEPS - 1) * HALF_BRIGHTNESS + MIN_WHITE_PW + 0.5f;
        const uint8_t ir_pwm_init =    (MAX_IR_PW - MIN_IR_PW)       / (BRIGHTNESS_STEPS - 1) * HALF_BRIGHTNESS + MIN_IR_PW + 0.5f;
        VERIFY(getWhitePWM() == white_pwm_init);
        VERIFY(getIRPWM()    == ir_pwm_init);
    }

    // 50 Dimming steps
    TEST("Dimming Steps")
    {
        VERIFY(BRIGHTNESS_STEPS == 50);
    }

    // Linear Dimming
    TEST("Linear Dimming")
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
            VERIFY(((whiteBrightness2 - whiteBrightness1) >= (whiteBrightnessStep - 1)) &&
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
            VERIFY(((irBrightness2 - irBrightness1) >= (irBrightnessStep - 1)) &&
                   ((irBrightness2 - irBrightness1) <= (irBrightnessStep + 1)));
        }

    }

    // Remember led states
    TEST("Remember LED State")
    {
        uint8_t expectedWhite = 0;
        uint8_t expectedIR = 0;
        // Sweep up through all brightness levels with white press
        setWhiteBrightness(MIN_BRIGHTNESS);
        expectedIR = getIRBrightness();

        while(getWhiteBrightness() < MAX_BRIGHTNESS)
        {
          increaseWhiteBrightness(PRESS);
          VERIFY(expectedIR == getIRBrightness());
        }

        // Sweep up through all brightness levels with white hold
        setWhiteBrightness(MIN_BRIGHTNESS);
        expectedIR = getIRBrightness();

        while(getWhiteBrightness() < MAX_BRIGHTNESS)
        {
          increaseWhiteBrightness(HOLD);
          VERIFY(expectedIR == getIRBrightness());
        }

        // Sweep up through all brightness levels with white alternating press and hold
        setWhiteBrightness(MIN_BRIGHTNESS);
        expectedIR = getIRBrightness();

        while(getWhiteBrightness() < MAX_BRIGHTNESS)
        {
          increaseWhiteBrightness(getWhiteBrightness() % 2);
          VERIFY(expectedIR == getIRBrightness());
        }

        // Sweep up through all brightness levels with ir press
        setIRBrightness(MIN_BRIGHTNESS);
        expectedWhite = getWhiteBrightness();

        while(getIRBrightness() < MAX_BRIGHTNESS)
        {
          increaseIRBrightness(PRESS);
          VERIFY(expectedWhite == getWhiteBrightness());
        }

        // Sweep up through all brightness levels with ir hold
        setIRBrightness(MIN_BRIGHTNESS);
        expectedWhite = getWhiteBrightness();

        while(getIRBrightness() < MAX_BRIGHTNESS)
        {
          increaseIRBrightness(HOLD);
          VERIFY(expectedWhite == getWhiteBrightness());
        }

        // Sweep up through all brightness levels with ir alternating press and hold
        setIRBrightness(MIN_BRIGHTNESS);
        expectedWhite = getWhiteBrightness();

        while(getIRBrightness() < MAX_BRIGHTNESS)
        {
          increaseIRBrightness(getIRBrightness() % 2);
          VERIFY(expectedWhite == getWhiteBrightness());
        }

        // Sweep down through all brightness levels with white press
        setWhiteBrightness(MAX_BRIGHTNESS);
        expectedIR = getIRBrightness();

        while(getWhiteBrightness() > MIN_BRIGHTNESS)
        {
          decreaseWhiteBrightness(PRESS);
          VERIFY(expectedIR == getIRBrightness());
        }

        // Sweep down through all brightness levels with white hold
        setWhiteBrightness(MAX_BRIGHTNESS);
        expectedIR = getIRBrightness();

        while(getWhiteBrightness() > MIN_BRIGHTNESS)
        {
          decreaseWhiteBrightness(HOLD);
          VERIFY(expectedIR == getIRBrightness());
        }

        // Sweep up through all brightness levels with white alternating press and hold
        setWhiteBrightness(MAX_BRIGHTNESS);
        expectedIR = getIRBrightness();

        while(getWhiteBrightness() > MIN_BRIGHTNESS)
        {
          decreaseWhiteBrightness(getWhiteBrightness() % 2);
          VERIFY(expectedIR == getIRBrightness());
        }

        // Sweep down through all brightness levels with ir press
        setIRBrightness(MAX_BRIGHTNESS);
        expectedWhite = getWhiteBrightness();

        while(getIRBrightness() > MIN_BRIGHTNESS)
        {
          decreaseIRBrightness(PRESS);
          VERIFY(expectedWhite == getWhiteBrightness());
        }

        // Sweep down through all brightness levels with ir hold
        setIRBrightness(MAX_BRIGHTNESS);
        expectedWhite = getWhiteBrightness();

        while(getIRBrightness() > MIN_BRIGHTNESS)
        {
          decreaseIRBrightness(HOLD);
          VERIFY(expectedWhite == getWhiteBrightness());
        }

        // Sweep down through all brightness levels with ir alternating press and hold
        setIRBrightness(MAX_BRIGHTNESS);
        expectedWhite = getWhiteBrightness();

        while(getIRBrightness() > MIN_BRIGHTNESS)
        {
          decreaseIRBrightness(getIRBrightness() % 2);
          VERIFY(expectedWhite == getWhiteBrightness());
        }
    }

    // Bright and Dim have press and hold states
      // Press state occurs when debounced pressed for less than 500ms
      // Hold state occurs when debounced pressed for more than 500ms
    TEST("Hold Check")
    {
        // Ensure greatest delay time is less than 500ms
        for (uint8_t i = MIN_BRIGHTNESS; i < BRIGHTNESS_STEPS; i++)
        {
          VERIFY(brightnessDelay(i) < 500);
        }

        // Ensure two consecutive delay times are greater than 500ms
        uint16_t delay1 = brightnessDelay(MIN_BRIGHTNESS);

        for (uint8_t i = 1; i < BRIGHTNESS_STEPS; i++)
        {
            uint16_t delay2 = brightnessDelay(MIN_BRIGHTNESS + i);
            VERIFY((delay2 + delay1) > 500);
            delay1 = delay2;
        }
    }

    // Sweep
        // 50% to full bright in 4.5 +- 0.5 seconds
        // 50% to full dim in 3.25 +- 0.5 seconds
        // 100% to 0% in 6 +- 1.5 seconds
        // 0% to 100% in 6 +- 1.5 seconds// This test ensures that the sweep time is within spec
    TEST("Theoretical Sweep")
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


            VERIFY(sweep_fifty_to_onehundred_time_ms <= fifty_to_onehundred_time_ms + fifty_to_onehundred_ff_ms);
            VERIFY(sweep_fifty_to_onehundred_time_ms >= fifty_to_onehundred_time_ms - fifty_to_onehundred_ff_ms);
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

            VERIFY(sweep_fifty_to_zero_time_ms <= fifty_to_zero_time_ms + fifty_to_zero_ff_ms);
            VERIFY(sweep_fifty_to_zero_time_ms >= fifty_to_zero_time_ms - fifty_to_zero_ff_ms);
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

            VERIFY(sweep_zero_to_onehundred_time_ms <= zero_to_onehundred_time_ms + zero_to_onehundred_ff_ms);
            VERIFY(sweep_zero_to_onehundred_time_ms >= zero_to_onehundred_time_ms - zero_to_onehundred_ff_ms);
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

            VERIFY(sweep_onehundred_to_zero_time_ms <= onehundred_to_zero_time_ms + onehundred_to_zero_ff_ms);
            VERIFY(sweep_onehundred_to_zero_time_ms >= onehundred_to_zero_time_ms - onehundred_to_zero_ff_ms);
        }
    }
    /* end requirements tests */

} /* TEST_GROUP() */