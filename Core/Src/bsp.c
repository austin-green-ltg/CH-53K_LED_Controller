#include "bsp.h"

/* Private variables ---------------------------------------------------------*/
GPIO_PinState readTogglePin( void )
{
  return (LL_GPIO_IsInputPinSet(SWITCH_LED_GPIO_Port, SWITCH_LED_Pin));
}

GPIO_PinState readDimPin( void )
{
  return (LL_GPIO_IsInputPinSet(DIM_GPIO_Port, DIM_Pin));
}

GPIO_PinState readBrightPin( void )
{
  return (LL_GPIO_IsInputPinSet(BRIGHT_GPIO_Port, BRIGHT_Pin));
}

void enablePWM1( void )
{
  LL_TIM_EnableAllOutputs(TIM1);

  LL_TIM_EnableCounter(TIM1);
}

void disablePWM1( void )
{
  LL_TIM_DisableAllOutputs(TIM1);

  LL_TIM_DisableCounter(TIM1);
}

void startPWM11( void )
{
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
}

void startPWM12( void )
{
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2);
}

void stopPWM11( void )
{
  LL_TIM_CC_DisableChannel(TIM1, LL_TIM_CHANNEL_CH1);
}

void stopPWM12( void )
{
  LL_TIM_CC_DisableChannel(TIM1, LL_TIM_CHANNEL_CH2);
}

void setPW11( uint32_t pulse_width )
{
  LL_TIM_OC_SetCompareCH1(TIM1, pulse_width);
}

void setPW12( uint32_t pulse_width )
{
  LL_TIM_OC_SetCompareCH2(TIM1, pulse_width);
}

void startTIM2( void )
{
  LL_TIM_EnableCounter(TIM2);

  return;
}

void restartTIM2( void )
{
  LL_TIM_SetCounter(TIM2, 0);
  return;
}

uint32_t getTIM2Cnt( void )
{
  return LL_TIM_GetCounter(TIM2);
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
  while(!LL_USART_IsActiveFlag_TXE(USART2)){};
  LL_USART_TransmitData8(USART2, (uint8_t)(c & 0xFFU));
  while(LL_USART_IsActiveFlag_TC(USART2)){}
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