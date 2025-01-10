/*****************************************************************************
 *
 *  @attention
 *  Copyright (c) 2022, 2023 Luminator, An LTG Company
 *  All rights reserved.
 *  Any use without the prior written consent of Luminator,
 *  An LTG Company is strictly prohibited.
 *
 *****************************************************************************
 *****************************************************************************
 *
 * @file pwm_handler.c
 *
 * @brief Handles the PWM output of the lights. Output is determined by a
 *          Brightness variable that is controlled by this file.
 *
 * Revision History:
 * Date       - Name         -  Ver -  Remarks
 * 07/31/2024 - Austin Green -  1.0 -  Initial Document
 * 09/10/2024 - Austin Green -  2.0 -  Doxyfile documentation
 *
 * Notes: Depends on the board support package bsp
 *        temperature_handler is for getting the temperature range of
 *          of the device. Lowers output at higher temperatures
 *        my_printf is for debugging purposes, doesn't do anything if
 *          ENABLE_UART_DEBUGGING is not defined
 *
 *****************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include <stdio.h>

#include "pwm_handler.h"
#include "stm32l412xx-bsp.h"
#include "temperature_handler.h"
#include "logger.h"
#include "my_printf.h"

/* uncommment to reverse brightness direction (dim brightens and bight dims) */
// #define REVERSE_BRIGHTNESS

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
    #define TIM15_PRESCALER    (65535)
    const float Tim15CntPerSec = ( CLK_FREQ_HZ / ( float ) TIM15_PRESCALER );
#endif /* ENABLE_UART_DEBUGGING */

/* Brightness Steps */
/** Max Brightness Step (50) */
const uint8_t MaxBrightness = ( BRIGHTNESS_STEPS - 1 )
                              ; // 50
/** Min Brightness Step (0) */
const uint8_t MinBrightness = ( 0 )
                              ; // 0
/** Half Brightness Step (24) */
const uint8_t HalfBrightness = ( ( uint8_t ) ( ( BRIGHTNESS_STEPS - 1 ) /
                                 2.0f ) ); // 25

/* Pulse Width Values */
#define PW_PERIOD (255)             // Period of PWM timer
/** Min pulse width value (0) */
const float MinPw = ( 0 ) ; // relative pulse width
/** Max pulse width value (PW_PERIOD(255)) */
const float MaxPw = ( PW_PERIOD ); // relative pulse width

/* Thermal State Constants */
/** Warm thermal state pwm constant */
const float WarmPwmRatio = ( 0.90f );
/** Hot thermal state pwm constant */
const float HotPwmRatio = ( 0.50f );

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/** Brightness to PWM value array (out of 255) */
static const uint8_t PwmArray [ BRIGHTNESS_STEPS ] = { 0,
                                                       5, 10, 15, 20, 26, 31, 36, 41, 46, 51,
                                                       56, 61, 66, 71, 77, 82, 87, 92, 97, 102,
                                                       107, 112, 117, 122, 128, 133, 138, 143, 148, 153,
                                                       158, 163, 168, 173, 179, 184, 189, 194, 199, 204,
                                                       209, 214, 219, 224, 230, 235, 240, 245, 250, 255
                                                     };

/** Initial led brightness, changed to half brightness on init */
static int8_t visBrightness = 0;
static int8_t irBrightness = 0;

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
    static uint16_t vis_min_time  = 0;
    static uint16_t vis_max_time  = 0;
    static uint16_t vis_half_time = 0;
    static uint16_t ir_min_time  = 0;
    static uint16_t ir_max_time  = 0;
    static uint16_t ir_half_time = 0;
#endif /* ENABLE_UART_DEBUGGING */

/**
  * @brief Init PwmArray var
  * Set brightness to half value, enable pwm, and turn off
  */
void InitPwm ( void )
{
    char stringVis [ PWM_LOG_SIZE ];
    char stringIr [ PWM_LOG_SIZE ];
    char stringPrevInit [ PWM_LOG_SIZE ];

    ReadLog ( STARTING_PWM_ADDRESS, stringVis,
              PWM_LOG_SIZE );

    ReadLog ( STARTING_PWM_ADDRESS + PWM_LOG_SIZE, stringIr,
              PWM_LOG_SIZE );

    ReadLog ( STARTING_PWM_ADDRESS + 2 * PWM_LOG_SIZE, stringPrevInit,
              PWM_LOG_SIZE );

    if ( stringPrevInit [ 0 ] == 0 )
    {
        visBrightness = HalfBrightness;
        irBrightness = HalfBrightness;
    }
    else
    {
        visBrightness = ( uint8_t ) stringVis [ 0 ];
        irBrightness = ( uint8_t ) stringIr [ 0 ];
    }

    WriteLog ( STARTING_PWM_ADDRESS + 2 * PWM_LOG_SIZE, "1",
               PWM_LOG_SIZE );

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
    printf ( "Init PWM\n" );
#endif /* ENABLE_UART_DEBUGGING */
    EnablePWM1();
    TurnOffPwm ( 0 ); // turn off Visible
    TurnOffPwm ( 1 ); // Turn off IR
    return;
}

/**
  * @brief Decrease brightness by 1 (for button press) or 3 (for button hold)
  *         This functions can be made to increase brightness value with the
  *         REVERSE_BRIGHTNESS flag
  *         Afterwards, set pwm output.
  * @param[in] button_held If the button is being held (decrements by 3 if so)
  * @param[in] isIr Are we controlling IR or Visible LEDs
  */
void DecreaseBrightness ( uint8_t button_held, uint8_t isIr )
{
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */

    if ( isIr )
    {
        // set this to ensure that when we sweep we are using the exact value from when we start
        if ( irBrightness == MinBrightness )
        {
            ir_min_time = TIM15->CNT;
            ir_half_time = 0;
            ir_max_time = 0;
        }
        else if ( irBrightness == HalfBrightness )
        {
            ir_half_time = TIM15->CNT;
        }
        else if ( irBrightness == MaxBrightness )
        {
            ir_min_time = 0;
            ir_half_time = 0;
            ir_max_time = TIM15->CNT;
        }
    }
    else
    {
        // set this to ensure that when we sweep we are using the exact value from when we start
        if ( visBrightness == MinBrightness )
        {
            vis_min_time = TIM15->CNT;
            vis_half_time = 0;
            vis_max_time = 0;
        }
        else if ( visBrightness == HalfBrightness )
        {
            vis_half_time = TIM15->CNT;
        }
        else if ( visBrightness == MaxBrightness )
        {
            vis_min_time = 0;
            vis_half_time = 0;
            vis_max_time = TIM15->CNT;
        }
    }

#endif /* ENABLE_UART_DEBUGGING */

#ifdef REVERSE_BRIGHTNESS

    if ( isIr )
    {
        // increase irBrightness
        if ( button_held == BUTTON_PRESSED )
        {
            irBrightness += HOLD_BRIGHTNESS_JUMP;
        }
        else
        {
            irBrightness++;
        }

        // prevent from going above MaxBrightness
        if ( irBrightness > MaxBrightness )
        {
            irBrightness = MaxBrightness;
        }
    }
    else
    {
        // increase visBrightness
        if ( button_held == BUTTON_PRESSED )
        {
            visBrightness += HOLD_BRIGHTNESS_JUMP;
        }
        else
        {
            visBrightness++;
        }

        // prevent from going above MaxBrightness
        if ( visBrightness > MaxBrightness )
        {
            visBrightness = MaxBrightness;
        }
    }

#else

    if ( isIr )
    {
        // decrease irBrightness
        if ( button_held == BUTTON_PRESSED )
        {
            irBrightness -= HOLD_BRIGHTNESS_JUMP;
        }
        else
        {
            irBrightness--;
        }

        // prevent from going below MinBrightness
        if ( irBrightness < MinBrightness )
        {
            irBrightness = MinBrightness;
        }
    }
    else
    {
        // decrease visBrightness
        if ( button_held == BUTTON_PRESSED )
        {
            visBrightness -= HOLD_BRIGHTNESS_JUMP;
        }
        else
        {
            visBrightness--;
        }

        // prevent from going below MinBrightness
        if ( visBrightness < MinBrightness )
        {
            visBrightness = MinBrightness;
        }
    }

#endif /* REVERSE_BRIGHTNESS */

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */

    if ( isIr )
    {
        printf ( "Decreased IR to %d / %d\n", irBrightness, MaxBrightness );

        if ( irBrightness == MinBrightness )
        {
            ir_min_time = TIM15->CNT;

            if ( ( ir_half_time != 0 ) && button_held )
            {
                printf ( "Swept half time to min time in %f seconds\n",
                         ( double ) ( ir_min_time - ir_half_time ) / ( double ) Tim15CntPerSec );
            }

            if ( ( ir_max_time != 0 ) && button_held )
            {
                printf ( "Swept max time to min time in %f seconds\n",
                         ( double ) ( ir_min_time - ir_max_time ) / ( double ) Tim15CntPerSec );
            }
        }
        else if ( irBrightness == HalfBrightness )
        {
            ir_half_time = TIM15->CNT;

            if ( ( ir_min_time != 0 ) && button_held )
            {
                printf ( "Swept min time to half time in %f seconds\n",
                         ( double ) ( ir_half_time - ir_min_time ) / ( double ) Tim15CntPerSec );
            }

            if ( ( ir_max_time != 0 ) && button_held )
            {
                printf ( "Swept max time to half time in %f seconds\n",
                         ( double ) ( ir_half_time - ir_max_time ) / ( double ) Tim15CntPerSec );
            }
        }
        else if ( irBrightness == MaxBrightness )
        {
            ir_max_time = TIM15->CNT;

            if ( ( ir_min_time != 0 ) && button_held )
            {
                printf ( "Swept min time to max time in %f seconds\n",
                         ( double ) ( ir_max_time - ir_min_time ) / ( double ) Tim15CntPerSec );
            }

            if ( ( ir_half_time != 0 ) && button_held )
            {
                printf ( "Swept half time to max time in %f seconds\n",
                         ( double ) ( ir_max_time - ir_half_time ) / ( double ) Tim15CntPerSec );
            }
        }
    }
    else
    {
        printf ( "Decreased Visible to %d / %d\n", visBrightness, MaxBrightness );

        if ( visBrightness == MinBrightness )
        {
            vis_min_time = TIM15->CNT;

            if ( ( vis_half_time != 0 ) && button_held )
            {
                printf ( "Swept half time to min time in %f seconds\n",
                         ( double ) ( vis_min_time - vis_half_time ) / ( double ) Tim15CntPerSec );
            }

            if ( ( vis_max_time != 0 ) && button_held )
            {
                printf ( "Swept max time to min time in %f seconds\n",
                         ( double ) ( vis_min_time - vis_max_time ) / ( double ) Tim15CntPerSec );
            }
        }
        else if ( visBrightness == HalfBrightness )
        {
            vis_half_time = TIM15->CNT;

            if ( ( vis_min_time != 0 ) && button_held )
            {
                printf ( "Swept min time to half time in %f seconds\n",
                         ( double ) ( vis_half_time - vis_min_time ) / ( double ) Tim15CntPerSec );
            }

            if ( ( vis_max_time != 0 ) && button_held )
            {
                printf ( "Swept max time to half time in %f seconds\n",
                         ( double ) ( vis_half_time - vis_max_time ) / ( double ) Tim15CntPerSec );
            }
        }
        else if ( visBrightness == MaxBrightness )
        {
            vis_max_time = TIM15->CNT;

            if ( ( vis_min_time != 0 ) && button_held )
            {
                printf ( "Swept min time to max time in %f seconds\n",
                         ( double ) ( vis_max_time - vis_min_time ) / ( double ) Tim15CntPerSec );
            }

            if ( ( vis_half_time != 0 ) && button_held )
            {
                printf ( "Swept half time to max time in %f seconds\n",
                         ( double ) ( vis_max_time - vis_half_time ) / ( double ) Tim15CntPerSec );
            }
        }
    }


#endif /* ENABLE_UART_DEBUGGING */

    // set irBrightness/visBrightness
    SetPwm ( isIr );

    return;
}

/**
  * @brief Increase brightness by 1 (for button press) or 3 (for button hold)
  *         This functions can be made to decrease brightness value with the
  *         REVERSE_BRIGHTNESS flag
  *         Afterwards, set pwm output.
  * @param[in] button_held If the button is being held (increments by 3 if so)
  * @param[in] isIr Are we controlling IR or Visible LEDs
  */
void IncreaseBrightness ( uint8_t button_held, uint8_t isIr )
{
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */

    if ( isIr )
    {
        // set this to ensure that when we sweep we are using the exact value from when we start
        if ( irBrightness == MinBrightness )
        {
            ir_min_time = TIM15->CNT;
            ir_half_time = 0;
            ir_max_time = 0;
        }
        else if ( irBrightness == HalfBrightness )
        {
            ir_half_time = TIM15->CNT;
        }
        else if ( irBrightness == MaxBrightness )
        {
            ir_min_time = 0;
            ir_half_time = 0;
            ir_max_time = TIM15->CNT;
        }
    }
    else
    {
        // set this to ensure that when we sweep we are using the exact value from when we start
        if ( visBrightness == MinBrightness )
        {
            vis_min_time = TIM15->CNT;
            vis_half_time = 0;
            vis_max_time = 0;
        }
        else if ( visBrightness == HalfBrightness )
        {
            vis_half_time = TIM15->CNT;
        }
        else if ( visBrightness == MaxBrightness )
        {
            vis_min_time = 0;
            vis_half_time = 0;
            vis_max_time = TIM15->CNT;
        }
    }

#endif /* ENABLE_UART_DEBUGGING */

#ifdef REVERSE_BRIGHTNESS

    if ( isIr )
    {
        // decrease irBrightness
        if ( button_held == BUTTON_PRESSED )
        {
            irBrightness -= HOLD_BRIGHTNESS_JUMP;
        }
        else
        {
            irBrightness--;
        }

        // prevent from going below MinBrightness
        if ( irBrightness < MinBrightness )
        {
            irBrightness = MinBrightness;
        }
    }
    else
    {
        // decrease visBrightness
        if ( button_held == BUTTON_PRESSED )
        {
            visBrightness -= HOLD_BRIGHTNESS_JUMP;
        }
        else
        {
            visBrightness--;
        }

        // prevent from going below MinBrightness
        if ( visBrightness < MinBrightness )
        {
            visBrightness = MinBrightness;
        }
    }

#else

    if ( isIr )
    {
        // increase irBrightness
        if ( button_held == BUTTON_PRESSED )
        {
            irBrightness += HOLD_BRIGHTNESS_JUMP;
        }
        else
        {
            irBrightness++;
        }

        // prevent from going above MaxBrightness
        if ( irBrightness > MaxBrightness )
        {
            irBrightness = MaxBrightness;
        }
    }
    else
    {
        // increase visBrightness
        if ( button_held == BUTTON_PRESSED )
        {
            visBrightness += HOLD_BRIGHTNESS_JUMP;
        }
        else
        {
            visBrightness++;
        }

        // prevent from going above MaxBrightness
        if ( visBrightness > MaxBrightness )
        {
            visBrightness = MaxBrightness;
        }
    }

#endif /* REVERSE_BRIGHTNESS */

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */

    if ( isIr )
    {
        printf ( "Increased IR to %d / %d\n", irBrightness, MaxBrightness );

        if ( irBrightness == MinBrightness )
        {
            ir_min_time = TIM15->CNT;

            if ( ( ir_half_time != 0 ) && button_held )
            {
                printf ( "Swept half time to min time in %f seconds\n",
                         ( double ) ( ir_min_time - ir_half_time ) / ( double ) Tim15CntPerSec );
            }

            if ( ( ir_max_time != 0 ) && button_held )
            {
                printf ( "Swept max time to min time in %f seconds\n",
                         ( double ) ( ir_min_time - ir_max_time ) / ( double ) Tim15CntPerSec );
            }
        }
        else if ( irBrightness == HalfBrightness )
        {
            ir_half_time = TIM15->CNT;

            if ( ( ir_min_time != 0 ) && button_held )
            {
                printf ( "Swept min time to half time in %f seconds\n",
                         ( double ) ( ir_half_time - ir_min_time ) / ( double ) Tim15CntPerSec );
            }

            if ( ( ir_max_time != 0 ) && button_held )
            {
                printf ( "Swept max time to half time in %f seconds\n",
                         ( double ) ( ir_half_time - ir_max_time ) / ( double ) Tim15CntPerSec );
            }
        }
        else if ( irBrightness == MaxBrightness )
        {
            ir_max_time = TIM15->CNT;

            if ( ( ir_min_time != 0 ) && button_held )
            {
                printf ( "Swept min time to max time in %f seconds\n",
                         ( double ) ( ir_max_time - ir_min_time ) / ( double ) Tim15CntPerSec );
            }

            if ( ( ir_half_time != 0 ) && button_held )
            {
                printf ( "Swept half time to max time in %f seconds\n",
                         ( double ) ( ir_max_time - ir_half_time ) / ( double ) Tim15CntPerSec );
            }
        }
    }
    else
    {
        printf ( "Increased Visible to %d / %d\n", visBrightness, MaxBrightness );

        if ( visBrightness == MinBrightness )
        {
            vis_min_time = TIM15->CNT;

            if ( ( vis_half_time != 0 ) && button_held )
            {
                printf ( "Swept half time to min time in %f seconds\n",
                         ( double ) ( vis_min_time - vis_half_time ) / ( double ) Tim15CntPerSec );
            }

            if ( ( vis_max_time != 0 ) && button_held )
            {
                printf ( "Swept max time to min time in %f seconds\n",
                         ( double ) ( vis_min_time - vis_max_time ) / ( double ) Tim15CntPerSec );
            }
        }
        else if ( visBrightness == HalfBrightness )
        {
            vis_half_time = TIM15->CNT;

            if ( ( vis_min_time != 0 ) && button_held )
            {
                printf ( "Swept min time to half time in %f seconds\n",
                         ( double ) ( vis_half_time - vis_min_time ) / ( double ) Tim15CntPerSec );
            }

            if ( ( vis_max_time != 0 ) && button_held )
            {
                printf ( "Swept max time to half time in %f seconds\n",
                         ( double ) ( vis_half_time - vis_max_time ) / ( double ) Tim15CntPerSec );
            }
        }
        else if ( visBrightness == MaxBrightness )
        {
            vis_max_time = TIM15->CNT;

            if ( ( vis_min_time != 0 ) && button_held )
            {
                printf ( "Swept min time to max time in %f seconds\n",
                         ( double ) ( vis_max_time - vis_min_time ) / ( double ) Tim15CntPerSec );
            }

            if ( ( vis_half_time != 0 ) && button_held )
            {
                printf ( "Swept half time to max time in %f seconds\n",
                         ( double ) ( vis_max_time - vis_half_time ) / ( double ) Tim15CntPerSec );
            }
        }
    }


#endif /* ENABLE_UART_DEBUGGING */

    // set irBrightness/visBrightness
    SetPwm ( isIr );

    return;
}

/**
  * @brief set PWM based on pwm value
  * @param[in] isIr Are we controlling IR or Visible LEDs
  */
void SetPwm ( uint8_t isIr )
{
    uint32_t pulse_width = GetPwm ( isIr );
    TurnOffPwm (!isIr ); // turn off other pwm signal

    isIr ? SetPW12 ( pulse_width ) : SetPW11 ( pulse_width );
#ifdef REVERSE_BRIGHTNESS

    if ( pulse_width >= MaxPw )
    {
        TurnOffPwm ( isIr );
    }

#else

    if ( pulse_width <= MinPw )
    {
        TurnOffPwm ( isIr );
    }

#endif /* REVERSE_BRIGHTNESS */
    else
    {
        isIr ? StartPWM12() : StartPWM11();
    }

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
    printf ( "Turn on to %u / %u\n", pulse_width, ( uint8_t ) PW_PERIOD );
#endif /* ENABLE_UART_DEBUGGING */
}

/**
  * @brief turn off PWM
  * @param[in] isIr Are we controlling IR or Visible LEDs
  */
void TurnOffPwm ( uint8_t isIr )
{
    isIr ? StopPWM12() : StopPWM11();
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
    printf ( "Turn off" );
    isIr ? printf ( " IR\n" ) : printf ( " Visible\n" );
#endif /* ENABLE_UART_DEBUGGING */
}

/**
  * @brief Return Brightness variable
  * @param[in] isIr Are we controlling IR or Visible LEDs
  * @param[out] Current LED brightness level
  */
int8_t GetBrightness ( uint8_t isIr )
{
    return ( isIr ? irBrightness : visBrightness );
}

/**
  * @brief Set Brightness variable, guards to ensure we don't go over max or min
  * @param[in] brightness Brightness to set
  * @param[in] isIr Are we controlling IR or Visible LEDs
  */
void SetBrightness ( int8_t brightness, uint8_t isIr )
{
    if ( isIr )
    {
        if ( brightness > MaxBrightness )
        {
            irBrightness = MaxBrightness;
        }
        else if ( brightness < MinBrightness )
        {
            irBrightness = MinBrightness;
        }
        else
        {
            irBrightness = brightness;
        }
    }
    else
    {
        if ( brightness > MaxBrightness )
        {
            visBrightness = MaxBrightness;
        }
        else if ( brightness < MinBrightness )
        {
            visBrightness = MinBrightness;
        }
        else
        {
            visBrightness = brightness;
        }
    }

    return;
}

/**
  * @brief Get the PWM value based on the brightness and the temperature range
  * @param[in] isIr Are we controlling IR or Visible LEDs
  * @param[out] Current PWM value
  */
uint8_t GetPwm ( uint8_t isIr )
{
    uint8_t pwm = isIr ? PwmArray [ irBrightness ] : PwmArray [ visBrightness ];

    /* Disable temperature based PWM */
    // TemperatureRange_e temperature_range = GetTemperatureRange();
    //
    // switch ( temperature_range )
    // {
    // case TempCool:
    // // no change to PWM value
    // break;
    //
    // case TempWarm:
    // pwm = ( uint8_t ) ( pwm * WarmPwmRatio + 0.5f );
    // break;
    //
    // case TempHot:
    // pwm = ( uint8_t ) ( pwm * HotPwmRatio + 0.5f );
    // break;
    //
    // default:
    // pwm = 0;
    // break;
    //
    // }

    return ( pwm );
}

/**
  * @brief Logs PWM to storage
  */
void LogPwm ( void )
{
    const uint8_t numPwmLogs = TOTAL_PWM_LOGS - 1; // Don't include prev init
    char brightnessLog [ TOTAL_PWM_LOGS - 1 ]; // Don't include prev init


    for ( uint8_t i = 0; i < numPwmLogs; i++ )
    {
        brightnessLog [ i ] = ( char ) GetBrightness ( i );
    }

    WriteLog ( STARTING_PWM_ADDRESS, brightnessLog,
               PWM_LOG_SIZE * numPwmLogs );
}
