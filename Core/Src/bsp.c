#include "bsp.h"

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;
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

// Returns raw thermistor ADC value
int32_t getThermistorValue( void )
{
  // TODO
  return -1;
}

void sendUARTChar(char c)
{
  HAL_UART_Transmit(&huart2, (uint8_t *) &c, sizeof(uint8_t), 0xFFFF);
	return;
}