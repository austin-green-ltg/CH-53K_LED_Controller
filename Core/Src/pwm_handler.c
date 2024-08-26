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
//                visible light. Output is determined by a ledBrightness
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
#include "stm32l412xx-bsp.h"
#include "thermistor_handler.h"
#include "my_printf.h"

// #define REVERSE_BRIGHTNESS

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
    #define TIM15_PRESCALER    (65535)
    const float Tim15CntPerSec = (CLK_FREQ_HZ / (float)TIM15_PRESCALER);
#endif /* ENABLE_UART_DEBUGGING */

/* Brightness Steps */
const uint8_t MaxBrightness     = (BRIGHTNESS_STEPS - 1)                    ; // 49
const uint8_t MinBrightness     = (0)                                       ; // 0
const uint8_t HalfBrightness    = ((uint8_t)((BRIGHTNESS_STEPS - 1) / 2.0f)); // Rounds down to 24

/* Pulse Width Values */
#define PW_PERIOD (255)             // Period of PWM timer
const float MinPw  = (0)        ;   // relative pulse width
const float MaxPw  = (PW_PERIOD);   // relative pulse width

/* Thermal State Constants */
const float WarmPwmRatio    = (0.90f);
const float HotPwmRatio     = (0.50f);

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
// Brightness to PWM value array (out of 255)
static const uint8_t PwmArray [ BRIGHTNESS_STEPS ] = {  0, 5, 10, 16, 21, 26, 31, 36, 42, 47,
                                                        52, 57, 62, 68, 73, 78, 83, 88, 94, 99,
                                                        104, 109, 114, 120, 125, 130, 135, 141, 146, 151,
                                                        156, 161, 167, 172, 177, 182, 187, 193, 198, 203,
                                                        208, 213, 219, 224, 229, 234, 239, 245, 250, 255 };
// Initial led brightness, changed to half brightness on init
static int8_t ledBrightness = 0;

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
    static uint16_t min_time  = 0;
    static uint16_t max_time  = 0;
    static uint16_t half_time = 0;
#endif /* ENABLE_UART_DEBUGGING */

// Init PwmArray var
// Set brightness to half value, enable pwm, and turn off
void InitPwm(void)
{
    ledBrightness = HalfBrightness;
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("Init PWM\n");
    #endif /* ENABLE_UART_DEBUGGING */
    EnablePWM1();
    TurnOffPwm();
    return;
}

// Decrease brightness by 1 (for button press) or 3 (for button hold)
// This functions can be made to increase brightness value with the
// REVERSE_BRIGHTNESS flag
// Afterwards, set pwm output.
void DecreaseBrightness( uint8_t button_held )
{
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        // set this to ensure that when we sweep we are using the exact value from when we start
        if (ledBrightness == MinBrightness)
        {
            min_time  = TIM15->CNT;
            half_time = 0;
            max_time  = 0;
        }
        else if (ledBrightness == HalfBrightness)
        {
            half_time = TIM15->CNT;
        }
        else if (ledBrightness == MaxBrightness)
        {
            min_time  = 0;
            half_time = 0;
            max_time  = TIM15->CNT;
        }
    #endif /* ENABLE_UART_DEBUGGING */

#ifdef REVERSE_BRIGHTNESS
    // increase ledBrightness
    if (button_held == BUTTON_PRESSED) ledBrightness += HOLD_BRIGHTNESS_JUMP;
    else                               ledBrightness++;

    // prevent from going above MaxBrightness
    if (ledBrightness > MaxBrightness) ledBrightness = MaxBrightness;
#else
    // decrease ledBrightness
    if (button_held == BUTTON_PRESSED) ledBrightness -= HOLD_BRIGHTNESS_JUMP;
    else                               ledBrightness--;

    // prevent from going below MinBrightness
    if (ledBrightness < MinBrightness) ledBrightness = MinBrightness;
#endif /* REVERSE_BRIGHTNESS */

    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("Decreased to %d / %d\n", ledBrightness, MaxBrightness);

        if (ledBrightness == MinBrightness)
        {
            min_time = TIM15->CNT;
            if ((half_time != 0) && button_held) printf("Swept half time to min time in %f seconds\n", (double)(min_time - half_time) / (double)Tim15CntPerSec);
            if ((max_time  != 0) && button_held) printf("Swept max time to min time in %f seconds\n",  (double)(min_time - max_time) / (double)Tim15CntPerSec);
        }
        else if (ledBrightness == HalfBrightness)
        {
            half_time = TIM15->CNT;
            if ((min_time != 0) && button_held) printf("Swept min time to half time in %f seconds\n", (double)(half_time - min_time) / (double)Tim15CntPerSec);
            if ((max_time != 0) && button_held) printf("Swept max time to half time in %f seconds\n", (double)(half_time - max_time) / (double)Tim15CntPerSec);
        }
        else if (ledBrightness == MaxBrightness)
        {
            max_time = TIM15->CNT;
            if ((min_time  != 0) && button_held) printf("Swept min time to max time in %f seconds\n",  (double)(max_time - min_time) / (double)Tim15CntPerSec);
            if ((half_time != 0) && button_held) printf("Swept half time to max time in %f seconds\n", (double)(max_time - half_time) / (double)Tim15CntPerSec);
        }
    #endif /* ENABLE_UART_DEBUGGING */

    // set ledBrightness
    SetPwm();

    return;
}

// Increase brightness by 1 (for button press) or 3 (for button hold)
// This functions can be made to decrease brightness value with the
// REVERSE_BRIGHTNESS flag
// Afterwards, set pwm output.
void IncreaseBrightness( uint8_t button_held )
{
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        // set this to ensure that when we sweep we are using the exact value from when we start
        if (ledBrightness == MinBrightness)
        {
            min_time  = TIM15->CNT;
            half_time = 0;
            max_time  = 0;
        }
        else if (ledBrightness == HalfBrightness)
        {
            half_time = TIM15->CNT;
        }
        else if (ledBrightness == MaxBrightness)
        {
            min_time  = 0;
            half_time = 0;
            max_time  = TIM15->CNT;
        }
    #endif /* ENABLE_UART_DEBUGGING */

#ifdef REVERSE_BRIGHTNESS
    // decrease ledBrightness
    if (button_held == BUTTON_PRESSED) ledBrightness -= HOLD_BRIGHTNESS_JUMP;
    else                               ledBrightness--;

    // prevent from going below MinBrightness
    if (ledBrightness < MinBrightness) ledBrightness = MinBrightness;
#else
    // increase ledBrightness
    if (button_held == BUTTON_PRESSED) ledBrightness += HOLD_BRIGHTNESS_JUMP;
    else                               ledBrightness++;

    // prevent from going above MaxBrightness
    if (ledBrightness > MaxBrightness) ledBrightness = MaxBrightness;
#endif /* REVERSE_BRIGHTNESS */

    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("Increased to %d / %d\n", ledBrightness, MaxBrightness);

        if (ledBrightness == MinBrightness)
        {
            min_time = TIM15->CNT;
            if ((half_time != 0) && button_held) printf("Swept half time to min time in %f seconds\n", (double)(min_time - half_time) / (double)Tim15CntPerSec);
            if ((max_time  != 0) && button_held) printf("Swept max time to min time in %f seconds\n",  (double)(min_time - max_time) / (double)Tim15CntPerSec);
        }
        else if (ledBrightness == HalfBrightness)
        {
            half_time = TIM15->CNT;
            if ((min_time != 0) && button_held) printf("Swept min time to half time in %f seconds\n", (double)(half_time - min_time) / (double)Tim15CntPerSec);
            if ((max_time != 0) && button_held) printf("Swept max time to half time in %f seconds\n", (double)(half_time - max_time) / (double)Tim15CntPerSec);
        }
        else if (ledBrightness == MaxBrightness)
        {
            max_time = TIM15->CNT;
            if ((min_time  != 0) && button_held) printf("Swept min time to max time in %f seconds\n",  (double)(max_time - min_time) / (double)Tim15CntPerSec);
            if ((half_time != 0) && button_held) printf("Swept half time to max time in %f seconds\n", (double)(max_time - half_time) / (double)Tim15CntPerSec);
        }
    #endif /* ENABLE_UART_DEBUGGING */

    // set ledBrightness
    SetPwm();

    return;
}

// set PWM based on pwm value
void SetPwm( void )
{
    uint32_t pulse_width = GetPwm();

    SetPW11(pulse_width);
#ifdef REVERSE_BRIGHTNESS
    if (pulse_width == MaxPw) TurnOffPwm();
#else
    if (pulse_width == MinPw) TurnOffPwm();
#endif /* REVERSE_BRIGHTNESS */
    else StartPWM11();

    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("Turn on to %u / %u\n", pulse_width, (uint8_t)PW_PERIOD);
    #endif /* ENABLE_UART_DEBUGGING */
}

// turn off PWM
void TurnOffPwm( void )
{
    StopPWM11();
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("Turn off\n");
    #endif /* ENABLE_UART_DEBUGGING */
}

// Return ledBrightness variable
int8_t GetBrightness( void )
{
    return (ledBrightness);
}

// Set ledBrightness variable, guards to ensure we don't go over max or min
void SetBrightness( int8_t brightness )
{
    if      (brightness > MaxBrightness) ledBrightness = MaxBrightness;
    else if (brightness < MinBrightness) ledBrightness = MinBrightness;
    else                                 ledBrightness = brightness;
    return;
}

// Get the PWM value based on the brightness and the temperature range
uint8_t GetPwm( void )
{
    uint8_t pwm = 0;
    TemperatureRange_e temperature_range = GetTemperatureRange();

    switch(temperature_range)
    {
        case TempCool:
            pwm = PwmArray[ledBrightness];
            break;

        case TempWarm:
            pwm = (uint8_t)(PwmArray[ledBrightness] * WarmPwmRatio + 0.5f);
            break;

        case TempHot:
            pwm = (uint8_t)(PwmArray[ledBrightness] * HotPwmRatio + 0.5f);
            break;

        default:
            pwm = 0;
            break;

    }

    return (pwm);
}
