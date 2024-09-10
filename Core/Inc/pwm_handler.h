/*****************************************************************************
 *
 *  @attention
 * Copyright © 2007 Luminator Mark IV
 * All rights reserved.
 * Any use without the prior written consent of Luminator Mark IV
 * is strictly prohibited.
 *
 *****************************************************************************
 *****************************************************************************
 *
 * @file pwm_handler.h
 *
 * @brief Handles the PWM output of the lights. Output is determined by a
 *          Brightness variable that is controlled by this file.
 *
 * Revision History:
 * Date       - Name         -  Ver -  Remarks
 * 07/31/2024 - Austin Green -  1.0 -  Initial Document
 * 09/10/2024 - Austin Green -  2.0 -  Doxyfile documentation
 *
 * Notes:
 *
 *****************************************************************************/

#ifndef INC_pwm_handlerh
    #define INC_pwm_handlerh

    #include <stdint.h>

    /* Brightness Steps */
    #define BRIGHTNESS_STEPS        (50)
    #define HOLD_BRIGHTNESS_JUMP    (3)

  /**
    * @brief Init PwmArray var
    * Set brightness to half value, enable pwm, and turn off
    */
  void    InitPwm(void);                              // Init Pwm var

  /**
    * @brief Decrease brightness by 1 (for button press) or 3 (for button hold)
    *         This functions can be made to increase brightness value with the
    *         REVERSE_BRIGHTNESS flag
    *         Afterwards, set pwm output.
    * @param[in] button_held If the button is being held (decrements by 3 if so)
    */
  void    DecreaseBrightness( uint8_t button_held );  // decrease brightness

  /**
    * @brief Increase brightness by 1 (for button press) or 3 (for button hold)
    *         This functions can be made to decrease brightness value with the
    *         REVERSE_BRIGHTNESS flag
    *         Afterwards, set pwm output.
    * @param[in] button_held If the button is being held (increments by 3 if so)
    */
  void    IncreaseBrightness( uint8_t button_held );  // increase brightness

  /**
    * @brief set PWM based on pwm value
    */
  void    SetPwm( void );                             // turn on and set PWM

  /**
    * @brief turn off PWM
    */
  void    TurnOffPwm( void );                         // turn of PWM

  /**
    * @brief Return ledBrightness variable
    * @param[out] Current LED brightness level
    */
  int8_t  GetBrightness( void );                      // get value of Brightness

  /**
    * @brief Set ledBrightness variable, guards to ensure we don't go over max or min
    * @param[in] brightness Brightess to set
    */
  void    SetBrightness( int8_t brightness );         // set value of Brightness

  /**
    * @brief Get the PWM value based on the brightness and the temperature range
    * @param[out] Current PWM value
    */
  uint8_t GetPwm( void );                             // get value of current PWM

#endif /* INC_pwm_handlerh */
