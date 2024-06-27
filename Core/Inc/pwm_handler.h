
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PWM_HANDLER_H
#define __PWM_HANDLER_H

#include <stdint.h>

#define BRIGHTNESS_STEPS (50)
#define MAX_BRIGHTNESS   (BRIGHTNESS_STEPS - 1)
#define MIN_BRIGHTNESS   (0)
#define HALF_BRIGHTNESS  ((uint8_t)(MAX_BRIGHTNESS / 2)) // Rounds down to 24

#define HOLD_BRIGHTNESS_JUMP (3)

#define PW_PERIOD    (255)               // Period of PWM timer
#define MIN_WHITE_PW (PW_PERIOD / 10.0f) // relative pulse width
#define MAX_WHITE_PW (PW_PERIOD)         // relative pulse width
#define MIN_IR_PW    (PW_PERIOD / 10.0f) // relative pulse width
#define MAX_IR_PW    (PW_PERIOD)         // relative pulse width

void    initWhitePWM(void);                             // Init whitePWM var
void    initIRPWM(void);                                // Init irPWM var
void    decreaseWhiteBrightness( uint8_t button_held ); // decrease White brightness
void    decreaseIRBrightness( uint8_t button_held );    // decrease IR brightness
void    increaseWhiteBrightness( uint8_t button_held ); // increase White brightness
void    increaseIRBrightness( uint8_t button_held );    // increase IR brightness
void    setWhitePWM( void );                            // turn on and set White PWM
void    setIRPWM( void );                               // turn on and set IR PWM
void    turnOffWhitePWM( void );                        // turn of White PWM
void    turnOffIRPWM( void );                           // turn of IR PWM
int8_t  getWhiteBrightness( void );                     // get value of whiteBrightness
int8_t  getIRBrightness( void );                        // get value of irBrightness
void    setWhiteBrightness( int8_t brightness );        // set value of whiteBrightness
void    setIRBrightness( int8_t brightness );           // set value of irBrightness
uint8_t getWhitePWM( void );                            // get value of current white PWM
uint8_t getIRPWM( void );                               // get value of current IR PWM

#endif /* __PWM_HANDLER_H */
