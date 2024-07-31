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
// Description: Handles the PWM output of the lights for both IR and
//                visible light. Output is determined by a *Brightness
//                variable that is controlled by this file.
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
    #define BRIGHTNESS_STEPS (50)
    #define MAX_BRIGHTNESS   (BRIGHTNESS_STEPS - 1)
    #define MIN_BRIGHTNESS   (0)
    #define HALF_BRIGHTNESS  ((uint8_t)(MAX_BRIGHTNESS / 2)) // Rounds down to 24

    #define HOLD_BRIGHTNESS_JUMP (3)

    /* Pulse Width Values */
    #define PW_PERIOD    (255)               // Period of PWM timer
    #define MIN_WHITE_PW (PW_PERIOD / 10.0f) // relative pulse width
    #define MAX_WHITE_PW (PW_PERIOD)         // relative pulse width
    #define MIN_IR_PW    (PW_PERIOD / 10.0f) // relative pulse width
    #define MAX_IR_PW    (PW_PERIOD)         // relative pulse width

    /* Thermal State Constants */
    #define WARM_PWM_RATIO  (0.90f)
    #define HOT_PWM_RATIO   (0.50f)

    /* Initialize system */
    void    InitWhitePWM(void);                             // Init whitePWM var
    void    InitIRPWM(void);                                // Init irPWM var
    /* Decrease/Increase brightness levels */
    void    DecreaseWhiteBrightness( uint8_t button_held ); // decrease White brightness
    void    DecreaseIRBrightness( uint8_t button_held );    // decrease IR brightness
    void    IncreaseWhiteBrightness( uint8_t button_held ); // increase White brightness
    void    IncreaseIRBrightness( uint8_t button_held );    // increase IR brightness
    /* Set PWM value based on internal brightness */
    void    SetWhitePWM( void );                            // turn on and set White PWM
    void    SetIRPWM( void );                               // turn on and set IR PWM
    void    TurnOffWhitePWM( void );                        // turn of White PWM
    void    TurnOffIRPWM( void );                           // turn of IR PWM
    /* Get or set internal values */
    int8_t  GetWhiteBrightness( void );                     // get value of whiteBrightness
    int8_t  GetIRBrightness( void );                        // get value of irBrightness
    void    SetWhiteBrightness( int8_t brightness );        // set value of whiteBrightness
    void    SetIRBrightness( int8_t brightness );           // set value of irBrightness
    uint8_t GetWhitePWM( void );                            // get value of current white PWM
    uint8_t GetIRPWM( void );                               // get value of current IR PWM

#endif /* INC_pwm_handlerh */
