/* Private includes ----------------------------------------------------------*/
#include "delay_handler.h"
#include "bsp.h"
#include "my_printf.h"
#include "pwm_handler.h"

void startDelayCounter(void)
{
  startTIM2();
  restartDelayCounter();
  return;
}

void restartDelayCounter(void)
{
  restartTIM2();
  return;
}

uint8_t delayHit(uint32_t delay_ms)
{
  uint8_t isDelayHit = (getTIM2Cnt() >= (delay_ms * TIM2_CLK_KHZ));
  return isDelayHit;
}

uint16_t brightnessDelay(int8_t brightness)
{
  return ((50 + brightness) * 5);
}