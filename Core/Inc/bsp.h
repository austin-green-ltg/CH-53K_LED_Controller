/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_bsph
#define INC_bsph

#include <stdint.h>

/* Private defines -----------------------------------------------------------*/
#ifdef USE_HAL_DRIVER

#include "stm32f3xx_hal.h"

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

#define BUTTON_PRESSED (GPIO_PIN_SET)

#else

#define BRIGHT_Pin 0
#define BRIGHT_GPIO_Port 0
#define DIM_Pin 0
#define DIM_GPIO_Port 0
#define SWITCH_LED_Pin 0
#define SWITCH_LED_GPIO_Port 0
#define LED_STATUS_Pin 0
#define LED_STATUS_GPIO_Port 0
#define WHITE_LED_Pin 0
#define WHITE_LED_GPIO_Port 0
#define IR_LED_Pin 0
#define IR_LED_GPIO_Port 0

#define GPIO_PinState uint8_t

#define BUTTON_PRESSED (1)

typedef struct
{
  uint8_t is_running;
  uint32_t pulse_width;
} pwmStruct;

typedef struct
{
  uint8_t is_running;
  uint32_t time;
} timerStruct;

#endif

#define CLK_FREQ_HZ (8000000)
#define TIM2_CLK_DEV (1)
#define TIM2_CLK_PRESCALER (8000)
#define TIM2_CLK_KHZ (CLK_FREQ_HZ / TIM2_CLK_DEV / TIM2_CLK_PRESCALER / 1000) // 1 / ms

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
int32_t getThermistorValue( void );
void writeMem( uint32_t address, const char* const string, const uint32_t bytes);
void readMem( uint32_t address, char* string, uint32_t bytes_to_read);
void sendUARTChar(char c);


#endif /* INC_bsph */
