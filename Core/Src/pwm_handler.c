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
#define PW_PERIOD (255)                        // Period of PWM timer
const float MinWhitePw  = (0)           ; // relative pulse width
const float MaxWhitePw  = (PW_PERIOD)   ; // relative pulse width

/* Thermal State Constants */
const float WarmPwmRatio    = (0.90f);
const float HotPwmRatio     = (0.50f);

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static const uint8_t WhitePwm [ BRIGHTNESS_STEPS ] = {  0, 5, 10, 16, 21, 26, 31, 36, 42, 47,
                                                        52, 57, 62, 68, 73, 78, 83, 88, 94, 99,
                                                        104, 109, 114, 120, 125, 130, 135, 141, 146, 151,
                                                        156, 161, 167, 172, 177, 182, 187, 193, 198, 203,
                                                        208, 213, 219, 224, 229, 234, 239, 245, 250, 255 };
static int8_t  whiteBrightness = 0;

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
    static uint16_t white_min_time  = 0;
    static uint16_t white_max_time  = 0;
    static uint16_t white_half_time = 0;
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

void DecreaseWhiteBrightness( uint8_t button_held )
{
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        // set this to ensure that when we sweep we are using the exact value from when we start
        if (whiteBrightness == MinBrightness)
        {
            white_min_time  = TIM15->CNT;
            white_half_time = 0;
            white_max_time  = 0;
        }
        else if (whiteBrightness == HalfBrightness)
        {
            white_half_time = TIM15->CNT;
        }
        else if (whiteBrightness == MaxBrightness)
        {
            white_min_time  = 0;
            white_half_time = 0;
            white_max_time  = TIM15->CNT;
        }
    #endif /* ENABLE_UART_DEBUGGING */

#ifdef REVERSE_BRIGHTNESS
    // increase whiteBrightness
    if (button_held == BUTTON_PRESSED) whiteBrightness += HOLD_BRIGHTNESS_JUMP;
    else                               whiteBrightness++;

    // prevent from going above MaxBrightness
    if (whiteBrightness > MaxBrightness) whiteBrightness = MaxBrightness;
#else
    // decrease whiteBrightness
    if (button_held == BUTTON_PRESSED) whiteBrightness -= HOLD_BRIGHTNESS_JUMP;
    else                               whiteBrightness--;

    // prevent from going below MinBrightness
    if (whiteBrightness < MinBrightness) whiteBrightness = MinBrightness;
#endif /* REVERSE_BRIGHTNESS */

    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("%s decreased to %d / %d\n", "White", whiteBrightness, MaxBrightness);

        if (whiteBrightness == MinBrightness)
        {
            white_min_time = TIM15->CNT;
            if ((white_half_time != 0) && button_held) printf("Swept half time to min time in %f seconds\n", (double)(white_min_time - white_half_time) / (double)Tim15CntPerSec);
            if ((white_max_time  != 0) && button_held) printf("Swept max time to min time in %f seconds\n",  (double)(white_min_time - white_max_time) / (double)Tim15CntPerSec);
        }
        else if (whiteBrightness == HalfBrightness)
        {
            white_half_time = TIM15->CNT;
            if ((white_min_time != 0) && button_held) printf("Swept min time to half time in %f seconds\n", (double)(white_half_time - white_min_time) / (double)Tim15CntPerSec);
            if ((white_max_time != 0) && button_held) printf("Swept max time to half time in %f seconds\n", (double)(white_half_time - white_max_time) / (double)Tim15CntPerSec);
        }
        else if (whiteBrightness == MaxBrightness)
        {
            white_max_time = TIM15->CNT;
            if ((white_min_time  != 0) && button_held) printf("Swept min time to max time in %f seconds\n",  (double)(white_max_time - white_min_time) / (double)Tim15CntPerSec);
            if ((white_half_time != 0) && button_held) printf("Swept half time to max time in %f seconds\n", (double)(white_max_time - white_half_time) / (double)Tim15CntPerSec);
        }
    #endif /* ENABLE_UART_DEBUGGING */

    // set whiteBrightness
    SetWhitePwm();

    return;
}

void IncreaseWhiteBrightness( uint8_t button_held )
{
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        // set this to ensure that when we sweep we are using the exact value from when we start
        if (whiteBrightness == MinBrightness)
        {
            white_min_time  = TIM15->CNT;
            white_half_time = 0;
            white_max_time  = 0;
        }
        else if (whiteBrightness == HalfBrightness)
        {
            white_half_time = TIM15->CNT;
        }
        else if (whiteBrightness == MaxBrightness)
        {
            white_min_time  = 0;
            white_half_time = 0;
            white_max_time  = TIM15->CNT;
        }
    #endif /* ENABLE_UART_DEBUGGING */

#ifdef REVERSE_BRIGHTNESS
    // decrease whiteBrightness
    if (button_held == BUTTON_PRESSED) whiteBrightness -= HOLD_BRIGHTNESS_JUMP;
    else                               whiteBrightness--;

    // prevent from going below MinBrightness
    if (whiteBrightness < MinBrightness) whiteBrightness = MinBrightness;
#else
    // increase whiteBrightness
    if (button_held == BUTTON_PRESSED) whiteBrightness += HOLD_BRIGHTNESS_JUMP;
    else                               whiteBrightness++;

    // prevent from going above MaxBrightness
    if (whiteBrightness > MaxBrightness) whiteBrightness = MaxBrightness;
#endif /* REVERSE_BRIGHTNESS */

    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("%s increased to %d / %d\n", "White", whiteBrightness, MaxBrightness);

        if (whiteBrightness == MinBrightness)
        {
            white_min_time = TIM15->CNT;
            if ((white_half_time != 0) && button_held) printf("Swept half time to min time in %f seconds\n", (double)(white_min_time - white_half_time) / (double)Tim15CntPerSec);
            if ((white_max_time  != 0) && button_held) printf("Swept max time to min time in %f seconds\n",  (double)(white_min_time - white_max_time) / (double)Tim15CntPerSec);
        }
        else if (whiteBrightness == HalfBrightness)
        {
            white_half_time = TIM15->CNT;
            if ((white_min_time != 0) && button_held) printf("Swept min time to half time in %f seconds\n", (double)(white_half_time - white_min_time) / (double)Tim15CntPerSec);
            if ((white_max_time != 0) && button_held) printf("Swept max time to half time in %f seconds\n", (double)(white_half_time - white_max_time) / (double)Tim15CntPerSec);
        }
        else if (whiteBrightness == MaxBrightness)
        {
            white_max_time = TIM15->CNT;
            if ((white_min_time  != 0) && button_held) printf("Swept min time to max time in %f seconds\n",  (double)(white_max_time - white_min_time) / (double)Tim15CntPerSec);
            if ((white_half_time != 0) && button_held) printf("Swept half time to max time in %f seconds\n", (double)(white_max_time - white_half_time) / (double)Tim15CntPerSec);
        }
    #endif /* ENABLE_UART_DEBUGGING */

    // set whiteBrightness
    SetWhitePwm();

    return;
}

// set White PWM
void SetWhitePwm( void )
{
    uint32_t pulse_width = GetWhitePwm();

    SetPW11(pulse_width);
#ifdef REVERSE_BRIGHTNESS
    if (pulse_width == MaxWhitePw) TurnOffWhitePwm();
#else
    if (pulse_width == MinWhitePw) TurnOffWhitePwm();
#endif /* REVERSE_BRIGHTNESS */
    else StartPWM11();

    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("Turn on %s to %u / %u\n", "White", pulse_width, (uint8_t)PW_PERIOD);
    #endif /* ENABLE_UART_DEBUGGING */
}

// set White PWM
void TurnOffWhitePwm( void )
{
    StopPWM11();
    #ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
        printf("Turn off %s\n", "White");
    #endif /* ENABLE_UART_DEBUGGING */
}

int8_t GetWhiteBrightness( void )
{
    return (whiteBrightness);
}

void SetWhiteBrightness( int8_t brightness )
{
    if      (brightness > MaxBrightness) whiteBrightness = MaxBrightness;
    else if (brightness < MinBrightness) whiteBrightness = MinBrightness;
    else                                  whiteBrightness = brightness;
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
