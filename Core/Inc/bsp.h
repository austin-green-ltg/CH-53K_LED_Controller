/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H
#define __BSP_H

#include "stm32f3xx_hal.h"

/* Private defines -----------------------------------------------------------*/
#define BRIGHT_Pin GPIO_PIN_1
#define BRIGHT_GPIO_Port GPIOA
#define DIM_Pin GPIO_PIN_3
#define DIM_GPIO_Port GPIOA
#define SWITCH_LED_Pin GPIO_PIN_5
#define SWITCH_LED_GPIO_Port GPIOA
#define LED_STATUS_Pin GPIO_PIN_7
#define LED_STATUS_GPIO_Port GPIOA
#define WHITE_LED_Pin GPIO_PIN_8
#define WHITE_LED_GPIO_Port GPIOA
#define IR_LED_Pin GPIO_PIN_9
#define IR_LED_GPIO_Port GPIOA

GPIO_PinState readTogglePin( void );
GPIO_PinState readDimPin( void );
GPIO_PinState readBrightPin( void );
void startPWM11( void );
void startPWM12( void );
void stopPWM11( void );
void stopPWM12( void );
void setPW11( uint32_t pulse_width );
void setPW12( uint32_t pulse_width );
void startTIM2( void );
void restartTIM2( void );
uint32_t getTIM2Cnt( void );
void setTIM2Cnt( uint32_t cnt );
void sendUARTChar(char c);


#endif /* __BSP_H */
