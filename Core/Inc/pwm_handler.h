// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: pwm_handler.h
//
// Description: Handles the PWM output of the lights. Output is 
//                  determined by a Brightness variable that is 
//                  controlled by this file.
//
// Revision History:
// Date       - Name         -  Ver -  Remarks
// 07/31/2024 - Austin Green -  1.0 -  Initial Document
//
// Notes:
//
// ***************************************************************************

#ifndef INC_pwm_handlerh
    #define INC_pwm_handlerh

    #include <stdint.h>

    /* Brightness Steps */
    #define BRIGHTNESS_STEPS        (50)
    #define HOLD_BRIGHTNESS_JUMP    (3)

    /* Initialize system */
    void    InitWhitePwm(void);                             // Init WhitePwm var
    /* Decrease/Increase brightness levels */
    void    DecreaseWhiteBrightness( uint8_t button_held ); // decrease White brightness
    void    IncreaseWhiteBrightness( uint8_t button_held ); // increase White brightness
    /* Set PWM value based on internal brightness */
    void    SetWhitePwm( void );                            // turn on and set White PWM
    void    TurnOffWhitePwm( void );                        // turn of White PWM
    /* Get or set internal values */
    int8_t  GetWhiteBrightness( void );                     // get value of whiteBrightness
    void    SetWhiteBrightness( int8_t brightness );        // set value of whiteBrightness
    uint8_t GetWhitePwm( void );                            // get value of current white PWM

#endif /* INC_pwm_handlerh */
