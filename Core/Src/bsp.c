#include "bsp.h"

/* Private variables ---------------------------------------------------------*/
#ifdef USE_HAL_DRIVER
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;
#endif

#ifdef USE_HAL_DRIVER
GPIO_PinState readTogglePin( void )
{
  return (HAL_GPIO_ReadPin(SWITCH_LED_GPIO_Port, SWITCH_LED_Pin));
}

GPIO_PinState readDimPin( void )
{
  return (HAL_GPIO_ReadPin(DIM_GPIO_Port, DIM_Pin));
}

GPIO_PinState readBrightPin( void )
{
  return (HAL_GPIO_ReadPin(BRIGHT_GPIO_Port, BRIGHT_Pin));
}

void startPWM11( void )
{
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

void startPWM12( void )
{
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
}

void stopPWM11( void )
{
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

void stopPWM12( void )
{
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
}

void setPW11( uint32_t pulse_width )
{
  TIM1->CCR1 = pulse_width;
}

void setPW12( uint32_t pulse_width )
{
  TIM1->CCR2 = pulse_width;
}

void startTIM2( void )
{
  HAL_TIM_Base_Start(&htim2);
  return;
}

void restartTIM2( void )
{
  TIM2->CNT = 0;
  return;
}

uint32_t getTIM2Cnt( void )
{
  return TIM2->CNT;
}

void sendUARTChar(char c)
{
  HAL_UART_Transmit(&huart2, (uint8_t *) &c, sizeof(uint8_t), 0xFFFF);
	return;
}
#else
#include "time.h"
static time_t timer;

pwmStruct pwm_white;
pwmStruct pwm_ir;

// TODO Dummy Functions
GPIO_PinState readTogglePin( void )
{
  return (0);
}

GPIO_PinState readDimPin( void )
{
  return (0);
}

GPIO_PinState readBrightPin( void )
{
  return (0);
}

void startPWM11( void )
{
  pwm_white.running = 1;
  return;
}

void startPWM12( void )
{
  pwm_ir.running = 1;
  return;
}

void stopPWM11( void )
{
  pwm_white.running = 0;
  return;
}

void stopPWM12( void )
{
  pwm_ir.running = 0;
  return;
}

void setPW11( uint32_t pulse_width )
{
  pwm_white.pulse_width = pulse_width;
	return;
}

void setPW12( uint32_t pulse_width )
{
  pwm_ir.pulse_width = pulse_width;
	return;
}

void startTIM2( void )
{
  timer = time(NULL);
  return;
}

void restartTIM2( void )
{
  timer = time(NULL);
  return;
}

uint32_t getTIM2Cnt( void )
{
  float seconds = difftime(time(NULL), timer);
  return (uint32_t)(seconds / 1000.0f / TIM2_CLK_KHZ);
}

void sendUARTChar(char c)
{
  (void)c;
  return;
}
#endif