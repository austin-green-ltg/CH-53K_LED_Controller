// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: pwm_handler.c
//
// Description: Handles the PWM output of the lights for both IR and
//                visible light. Output is determined by a *Brightness
//                variable that is controlled by this file.
//
// Revision History:
// Date       - Name         -  Ver -  Remarks
// 07/31/2024 - Austin Green -  1.0 -  Initial Document
//
// Notes: Depends on the board support package bsp
//        thermistor_handler is for getting the temperature range of
//          of the device. Lowers output at higher temperatures
//        my_printf is for debugging purposes, doesn't do anything if
//          ENABLE_UART_DEBUGGING is not defined
//
// ***************************************************************************

/* Private includes ----------------------------------------------------------*/
#include "pwm_handler.h"
#include "bsp.h"
#include "thermistor_handler.h"
#include "my_printf.h"

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
    #define TIM3_PRESCALER    (65535)
    const float Tim3CntPerSec = (CLK_FREQ_HZ / (float)TIM3_PRESCALER);
#endif /* ENABLE_UART_DEBUGGING */

/* Brightness Steps */
const uint8_t MaxBrightness     = (BRIGHTNESS_STEPS - 1)                    ; // 49
const uint8_t MinBrightness     = (0)                                       ; // 0
const uint8_t HalfBrightness    = ((uint8_t)((BRIGHTNESS_STEPS - 1) / 2.0f)); // Rounds down to 24

/* Pulse Width Values */
#define PW_PERIOD (255)                        // Period of PWM timer
const float MinWhitePw  = (PW_PERIOD / 10.0f); // relative pulse width
const float MaxWhitePw  = (PW_PERIOD)        ; // relative pulse width
const float MinIrPw     = (PW_PERIOD / 10.0f); // relative pulse width
const float MaxIrPw     = (PW_PERIOD)        ; // relative pulse width

/* Thermal State Constants */
const float WarmPwmRatio    = (0.90f);
const float HotPwmRatio     = (0.50f);

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static const uint8_t WhitePwm [ BRIGHTNESS_STEPS ] = {  26, 30, 35, 40, 44, 49, 54, 58, 63, 68,
                                                        72, 77, 82, 86, 91, 96, 100, 105, 110, 114,
                                                        119, 124, 129, 133, 138, 143, 147, 152, 157, 161,
                                                        166, 171, 175, 180, 185, 189, 194, 199, 203, 208,
                                                        213, 218, 222, 227, 232, 236, 241, 246, 250, 255};
static const uint8_t IrPwm    [ BRIGHTNESS_STEPS ] = {  26, 30, 35, 40, 44, 49, 54, 58, 63, 68,
                                                        72, 77, 82, 86, 91, 96, 100, 105, 110, 114,
                                                        119, 124, 129, 133, 138, 143, 147, 152, 157, 161,
                                                        166, 171, 175, 180, 185, 189, 194, 199, 203, 208,
                                                        213, 218, 222, 227, 232, 236, 241, 246, 250, 255};
static int8_t  whiteBrightness = 0;
static int8_t  irBrightness = 0;

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
    static uint16_t white_min_time  = 0;
    static uint16_t white_max_time  = 0;
    static uint16_t white_half_time = 0;
    static uint16_t ir_min_time     = 0;
    static uint16_t ir_max_time     = 0;
    static uint16_t ir_half_time    = 0;
#endif /* ENABLE_UART_DEBUGGING */

// Init WhitePwm var
// i = [MinWhitePw, MaxWhitePw]
void InitWhitePwm(void)
{
    whiteBrightness = HalfBrightness;
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("Init %s PWM\n", "White");
    #endif /* ENABLE_UART_DEBUGGING */
    EnablePWM1();
    TurnOffWhitePwm();
    return;
}

// Init IrPwm var
// i = [MinIrPw, MaxIrPw]
void InitIrPwm(void)
{
    irBrightness = HalfBrightness;
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("Init %s PWM\n", "IR");
    #endif /* ENABLE_UART_DEBUGGING */
    EnablePWM1();
    TurnOffIrPwm();
    return;
}

void DecreaseWhiteBrightness( uint8_t button_held )
{
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        // set this to ensure that when we sweep we are using the exact value from when we start
        if (whiteBrightness == MinBrightness)
        {
            white_min_time  = TIM3->CNT;
            white_half_time = 0;
            white_max_time  = 0;
        }
        else if (whiteBrightness == HalfBrightness)
        {
            white_half_time = TIM3->CNT;
        }
        else if (whiteBrightness == MaxBrightness)
        {
            white_min_time  = 0;
            white_half_time = 0;
            white_max_time  = TIM3->CNT;
        }
    #endif /* ENABLE_UART_DEBUGGING */
    // decrease whiteBrightness
    if (button_held == BUTTON_PRESSED) whiteBrightness -= HOLD_BRIGHTNESS_JUMP;
    else                               whiteBrightness--;

    // prevent from going below MinBrightness
    if (whiteBrightness < MinBrightness) whiteBrightness = MinBrightness;

    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("%s decreased to %d / %d\n", "White", whiteBrightness, MaxBrightness);

        if (whiteBrightness == MinBrightness)
        {
            white_min_time = TIM3->CNT;
            if ((white_half_time != 0) && button_held) printf("Swept half time to min time in %f seconds\n", (double)(white_min_time - white_half_time) / (double)Tim3CntPerSec);
            if ((white_max_time  != 0) && button_held) printf("Swept max time to min time in %f seconds\n",  (double)(white_min_time - white_max_time) / (double)Tim3CntPerSec);
        }
        else if (whiteBrightness == HalfBrightness)
        {
            white_half_time = TIM3->CNT;
            if ((white_min_time != 0) && button_held) printf("Swept min time to half time in %f seconds\n", (double)(white_half_time - white_min_time) / (double)Tim3CntPerSec);
            if ((white_max_time != 0) && button_held) printf("Swept max time to half time in %f seconds\n", (double)(white_half_time - white_max_time) / (double)Tim3CntPerSec);
        }
        else if (whiteBrightness == MaxBrightness)
        {
            white_max_time = TIM3->CNT;
            if ((white_min_time  != 0) && button_held) printf("Swept min time to max time in %f seconds\n",  (double)(white_max_time - white_min_time) / (double)Tim3CntPerSec);
            if ((white_half_time != 0) && button_held) printf("Swept half time to max time in %f seconds\n", (double)(white_max_time - white_half_time) / (double)Tim3CntPerSec);
        }
    #endif /* ENABLE_UART_DEBUGGING */

    // set whiteBrightness
    SetWhitePwm();

    return;
}

void DecreaseIRBrightness( uint8_t button_held )
{
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        // set this to ensure that when we sweep we are using the exact value from when we start
        if (irBrightness == MinBrightness)
        {
            ir_min_time  = TIM3->CNT;
            ir_half_time = 0;
            ir_max_time  = 0;
        }
        else if (irBrightness == HalfBrightness)
        {
            ir_half_time = TIM3->CNT;
        }
        else if (irBrightness == MaxBrightness)
        {
            ir_min_time  = 0;
            ir_half_time = 0;
            ir_max_time  = TIM3->CNT;
        }
    #endif /* ENABLE_UART_DEBUGGING */

    // decrease irBrightness
    if (button_held == BUTTON_PRESSED) irBrightness -= HOLD_BRIGHTNESS_JUMP;
    else                               irBrightness--;

    // prevent from going below MinBrightness
    if (irBrightness < MinBrightness) irBrightness = MinBrightness;

    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("%s decreased to %d / %d\n", "IR", irBrightness, MaxBrightness);

        if (irBrightness == MinBrightness)
        {
            ir_min_time = TIM3->CNT;
            if ((ir_half_time != 0) && button_held) printf("Swept half time to min time in %f seconds\n", (double)(ir_min_time - ir_half_time) / (double)Tim3CntPerSec);
            if ((ir_max_time  != 0) && button_held) printf("Swept max time to min time in %f seconds\n",  (double)(ir_min_time - ir_max_time) / (double)Tim3CntPerSec);
        }
        else if (irBrightness == HalfBrightness)
        {
            ir_half_time = TIM3->CNT;
            if ((ir_min_time != 0) && button_held) printf("Swept min time to half time in %f seconds\n", (double)(ir_half_time - ir_min_time) / (double)Tim3CntPerSec);
            if ((ir_max_time != 0) && button_held) printf("Swept max time to half time in %f seconds\n", (double)(ir_half_time - ir_max_time) / (double)Tim3CntPerSec);
        }
        else if (irBrightness == MaxBrightness)
        {
            ir_max_time = TIM3->CNT;
            if ((ir_min_time  != 0) && button_held) printf("Swept min time to max time in %f seconds\n",  (double)(ir_max_time - ir_min_time) / (double)Tim3CntPerSec);
            if ((ir_half_time != 0) && button_held) printf("Swept half time to max time in %f seconds\n", (double)(ir_max_time - ir_half_time) / (double)Tim3CntPerSec);
        }
    #endif /* ENABLE_UART_DEBUGGING */

    // set irBrightness
    SetIrPwm();

    return;
}

void IncreaseWhiteBrightness( uint8_t button_held )
{
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        // set this to ensure that when we sweep we are using the exact value from when we start
        if (whiteBrightness == MinBrightness)
        {
            white_min_time  = TIM3->CNT;
            white_half_time = 0;
            white_max_time  = 0;
        }
        else if (whiteBrightness == HalfBrightness)
        {
            white_half_time = TIM3->CNT;
        }
        else if (whiteBrightness == MaxBrightness)
        {
            white_min_time  = 0;
            white_half_time = 0;
            white_max_time  = TIM3->CNT;
        }
    #endif /* ENABLE_UART_DEBUGGING */
    // increase whiteBrightness
    if (button_held == BUTTON_PRESSED) whiteBrightness += HOLD_BRIGHTNESS_JUMP;
    else                               whiteBrightness++;

    // prevent from going above MaxBrightness
    if (whiteBrightness > MaxBrightness) whiteBrightness = MaxBrightness;

    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("%s increased to %d / %d\n", "White", whiteBrightness, MaxBrightness);

        if (whiteBrightness == MinBrightness)
        {
            white_min_time = TIM3->CNT;
            if ((white_half_time != 0) && button_held) printf("Swept half time to min time in %f seconds\n", (double)(white_min_time - white_half_time) / (double)Tim3CntPerSec);
            if ((white_max_time  != 0) && button_held) printf("Swept max time to min time in %f seconds\n",  (double)(white_min_time - white_max_time) / (double)Tim3CntPerSec);
        }
        else if (whiteBrightness == HalfBrightness)
        {
            white_half_time = TIM3->CNT;
            if ((white_min_time != 0) && button_held) printf("Swept min time to half time in %f seconds\n", (double)(white_half_time - white_min_time) / (double)Tim3CntPerSec);
            if ((white_max_time != 0) && button_held) printf("Swept max time to half time in %f seconds\n", (double)(white_half_time - white_max_time) / (double)Tim3CntPerSec);
        }
        else if (whiteBrightness == MaxBrightness)
        {
            white_max_time = TIM3->CNT;
            if ((white_min_time  != 0) && button_held) printf("Swept min time to max time in %f seconds\n",  (double)(white_max_time - white_min_time) / (double)Tim3CntPerSec);
            if ((white_half_time != 0) && button_held) printf("Swept half time to max time in %f seconds\n", (double)(white_max_time - white_half_time) / (double)Tim3CntPerSec);
        }
    #endif /* ENABLE_UART_DEBUGGING */

    // set whiteBrightness
    SetWhitePwm();

    return;
}

void IncreaseIRBrightness( uint8_t button_held )
{
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        // set this to ensure that when we sweep we are using the exact value from when we start
        if (irBrightness == MinBrightness)
        {
            ir_min_time  = TIM3->CNT;
            ir_half_time = 0;
            ir_max_time  = 0;
        }
        else if (irBrightness == HalfBrightness)
        {
            ir_half_time = TIM3->CNT;
        }
        else if (irBrightness == MaxBrightness)
        {
            ir_min_time  = 0;
            ir_half_time = 0;
            ir_max_time  = TIM3->CNT;
        }
    #endif /* ENABLE_UART_DEBUGGING */
    // increase irBrightness
    if (button_held == BUTTON_PRESSED) irBrightness += HOLD_BRIGHTNESS_JUMP;
    else                               irBrightness++;

    // prevent from going above MaxBrightness
    if (irBrightness > MaxBrightness) irBrightness = MaxBrightness;

    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("%s increased to %d / %d\n", "IR", irBrightness, MaxBrightness);

        if (irBrightness == MinBrightness)
        {
            ir_min_time = TIM3->CNT;
            if ((ir_half_time != 0) && button_held) printf("Swept half time to min time in %f seconds\n", (double)(ir_min_time - ir_half_time) / (double)Tim3CntPerSec);
            if ((ir_max_time  != 0) && button_held) printf("Swept max time to min time in %f seconds\n",  (double)(ir_min_time - ir_max_time) / (double)Tim3CntPerSec);
        }
        else if (irBrightness == HalfBrightness)
        {
            ir_half_time = TIM3->CNT;
            if ((ir_min_time != 0) && button_held) printf("Swept min time to half time in %f seconds\n", (double)(ir_half_time - ir_min_time) / (double)Tim3CntPerSec);
            if ((ir_max_time != 0) && button_held) printf("Swept max time to half time in %f seconds\n", (double)(ir_half_time - ir_max_time) / (double)Tim3CntPerSec);
        }
        else if (irBrightness == MaxBrightness)
        {
            ir_max_time = TIM3->CNT;
            if ((ir_min_time  != 0) && button_held) printf("Swept min time to max time in %f seconds\n",  (double)(ir_max_time - ir_min_time) / (double)Tim3CntPerSec);
            if ((ir_half_time != 0) && button_held) printf("Swept half time to max time in %f seconds\n", (double)(ir_max_time - ir_half_time) / (double)Tim3CntPerSec);
        }
    #endif /* ENABLE_UART_DEBUGGING */

    // set irBrightness
    SetIrPwm();

    return;
}

// set White PWM
void SetWhitePwm( void )
{
    uint32_t pulse_width = GetWhitePwm();
    SetPW11(pulse_width);
    StartPWM11();
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("Turn on %s to %u / %u\n", "White", pulse_width, (uint8_t)PW_PERIOD);
    #endif /* ENABLE_UART_DEBUGGING */
}

// set IR PWM
void SetIrPwm( void )
{
    uint32_t pulse_width = GetIrPwm();
    SetPW12(pulse_width);
    StartPWM12();
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("Turn on %s to %u / %u\n", "IR", pulse_width, (uint8_t)PW_PERIOD);
    #endif /* ENABLE_UART_DEBUGGING */
    return;
}

// set White PWM
void TurnOffWhitePwm( void )
{
    StopPWM11();
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("Turn off %s\n", "White");
    #endif /* ENABLE_UART_DEBUGGING */
}

// set IR PWM
void TurnOffIrPwm( void )
{
    StopPWM12();
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("Turn off %s\n", "IR");
    #endif /* ENABLE_UART_DEBUGGING */
    return;
}

int8_t GetWhiteBrightness( void )
{
    return (whiteBrightness);
}

int8_t GetIRBrightness( void )
{
    return (irBrightness);
}

void SetWhiteBrightness( int8_t brightness )
{
    if      (brightness > MaxBrightness) whiteBrightness = MaxBrightness;
    else if (brightness < MinBrightness) whiteBrightness = MinBrightness;
    else                                  whiteBrightness = brightness;
    return;
}

void SetIRBrightness( int8_t brightness )
{
    if      (brightness > MaxBrightness) irBrightness = MaxBrightness;
    else if (brightness < MinBrightness) irBrightness = MinBrightness;
    else                                  irBrightness = brightness;
    return;
}

uint8_t GetWhitePwm( void )
{
    uint8_t pwm = 0;
    TemperatureRange_e temperature_range = GetTemperatureRange();

    switch(temperature_range)
    {
        case TempCool:
            pwm = WhitePwm[whiteBrightness];
            break;

        case TempWarm:
            pwm = (uint8_t)(WhitePwm[whiteBrightness] * WarmPwmRatio + 0.5f);
            break;

        case TempHot:
            pwm = (uint8_t)(WhitePwm[whiteBrightness] * HotPwmRatio + 0.5f);
            break;

        default:
            pwm = 0;
            break;

    }

    return (pwm);
}

uint8_t GetIrPwm( void )
{
    uint8_t pwm = 0;
    TemperatureRange_e temperature_range = GetTemperatureRange();

    switch(temperature_range)
    {
        case TempCool:
            pwm = IrPwm[irBrightness];
            break;

        case TempWarm:
            pwm = (uint8_t)(IrPwm[irBrightness] * WarmPwmRatio + 0.5f);
            break;

        case TempHot:
            pwm = (uint8_t)(IrPwm[irBrightness] * HotPwmRatio + 0.5f);
            break;

        default:
            pwm = 0;
            break;

    }

    return (pwm);
}
