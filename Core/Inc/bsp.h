/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_bsph
#define INC_bsph

#include <stdint.h>

/* Private defines -----------------------------------------------------------*/
#ifdef STM32F303x8

#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_system.h"
#include "stm32f3xx_ll_exti.h"
#include "stm32f3xx_ll_cortex.h"
#include "stm32f3xx_ll_utils.h"
#include "stm32f3xx_ll_pwr.h"
#include "stm32f3xx_ll_dma.h"
#include "stm32f3xx_ll_tim.h"
#include "stm32f3xx_ll_usart.h"
#include "stm32f3xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

#include "tim.h"
#include "gpio.h"

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
#include "usart.h"
#endif /* ENABLE_UART_DEBUGGING */

#define BRIGHT_Pin LL_GPIO_PIN_1
#define BRIGHT_GPIO_Port GPIOA
#define DIM_Pin LL_GPIO_PIN_3
#define DIM_GPIO_Port GPIOA
#define SWITCH_LED_Pin LL_GPIO_PIN_5
#define SWITCH_LED_GPIO_Port GPIOA
#define WHITE_LED_Pin LL_GPIO_PIN_8
#define WHITE_LED_GPIO_Port GPIOA
#define IR_LED_Pin LL_GPIO_PIN_9
#define IR_LED_GPIO_Port GPIOA

#else

#define BRIGHT_Pin 0
#define BRIGHT_GPIO_Port 0
#define DIM_Pin 0
#define DIM_GPIO_Port 0
#define SWITCH_LED_Pin 0
#define SWITCH_LED_GPIO_Port 0
#define WHITE_LED_Pin 0
#define WHITE_LED_GPIO_Port 0
#define IR_LED_Pin 0
#define IR_LED_GPIO_Port 0

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

#define GPIO_PinState uint8_t

#define BUTTON_PRESSED    (1)
#define BUTTON_UNPRESSED  (0)

#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
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
/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);


#endif /* INC_bsph */
