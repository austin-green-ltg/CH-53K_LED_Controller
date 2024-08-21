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
    void    InitPwm(void);                              // Init Pwm var
    /* Decrease/Increase brightness levels */
    void    DecreaseBrightness( uint8_t button_held );  // decrease brightness
    void    IncreaseBrightness( uint8_t button_held );  // increase brightness
    /* Set PWM value based on internal brightness */
    void    SetPwm( void );                             // turn on and set PWM
    void    TurnOffPwm( void );                         // turn of PWM
    /* Get or set internal values */
    int8_t  GetBrightness( void );                      // get value of Brightness
    void    SetBrightness( int8_t brightness );         // set value of Brightness
    uint8_t GetPwm( void );                             // get value of current PWM

#endif /* INC_pwm_handlerh */
