/* Private includes ----------------------------------------------------------*/
#include "delay_handler.h"
#include "bsp.h"
#include "my_printf.h"

/* Private define ------------------------------------------------------------*/
#define CLK_FREQ_HZ (8000000)
#define TIM2_CLK_DEV (1)
#define TIM2_CLK_PRESCALER (8000)
#define TIM2_CLK_KHZ (CLK_FREQ_HZ / TIM2_CLK_DEV / TIM2_CLK_PRESCALER / 1000) // 1 / ms

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