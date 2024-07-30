#include "bsp.h"

/* Private variables ---------------------------------------------------------*/
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

void writeMem( const uint32_t address, const char* const string, const uint32_t bytes )
{
  // TODO
  (void) address;
  (void) string;
  (void) bytes;
}
void readMem(  const uint32_t address, char* string, const uint32_t bytes )
{
  // TODO
  (void) address;
  (void) string;
  (void) bytes;
}

void sendUARTChar(char c)
{
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
  HAL_UART_Transmit(&huart2, (uint8_t *) &c, sizeof(uint8_t), 0xFFFF);
  return;
#endif /* ENABLE_UART_DEBUGGING */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}