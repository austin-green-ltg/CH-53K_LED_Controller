/* Private includes ----------------------------------------------------------*/
#include "pwm_handler.h"
#include "thermistor_handler.h"
#include "bsp.h"
#include "my_printf.h"
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
#define CLOCK_HZ         (8000000)
#define TIM3_PRESCALER   (65535)
#define TIM3_CNT_PER_SEC (CLOCK_HZ / (double)TIM3_PRESCALER)
#endif /* ENABLE_UART_DEBUGGING */

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static const uint8_t whitePWM [ BRIGHTNESS_STEPS ] = {26, 30, 35, 40, 44, 49, 54, 58, 63, 68,
                                                      72, 77, 82, 86, 91, 96, 100, 105, 110, 114,
                                                      119, 124, 129, 133, 138, 143, 147, 152, 157, 161,
                                                      166, 171, 175, 180, 185, 189, 194, 199, 203, 208,
                                                      213, 218, 222, 227, 232, 236, 241, 246, 250, 255};
static const uint8_t irPWM    [ BRIGHTNESS_STEPS ] = {26, 30, 35, 40, 44, 49, 54, 58, 63, 68,
                                                      72, 77, 82, 86, 91, 96, 100, 105, 110, 114,
                                                      119, 124, 129, 133, 138, 143, 147, 152, 157, 161,
                                                      166, 171, 175, 180, 185, 189, 194, 199, 203, 208,
                                                      213, 218, 222, 227, 232, 236, 241, 246, 250, 255};
static int8_t  whiteBrightness = 0;
static int8_t  irBrightness = 0;

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
static uint16_t white_min_time  = 0;
static uint16_t white_max_time  = 0;
static uint16_t white_half_time = 0;
static uint16_t ir_min_time     = 0;
static uint16_t ir_max_time     = 0;
static uint16_t ir_half_time    = 0;
#endif /* ENABLE_UART_DEBUGGING */

// Init whitePWM var
// i = [MIN_WHITE_PW, MAX_WHITE_PW]
void initWhitePWM(void)
{
  whiteBrightness = HALF_BRIGHTNESS;
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
  printf("Init %s PWM\n", "White");
#endif /* ENABLE_UART_DEBUGGING */
  enablePWM1();
  turnOffWhitePWM();
  return;
}

// Init irPWM var
// i = [MIN_IR_PW, MAX_IR_PW]
void initIRPWM(void)
{
  irBrightness = HALF_BRIGHTNESS;
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
  printf("Init %s PWM\n", "IR");
#endif /* ENABLE_UART_DEBUGGING */
  enablePWM1();
  turnOffIRPWM();
  return;
}

void decreaseWhiteBrightness( uint8_t button_held )
{
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
  // set this to ensure that when we sweep we are using the exact value from when we start
  if (whiteBrightness == MIN_BRIGHTNESS)
  {
    white_min_time  = TIM3->CNT;
    white_half_time = 0;
    white_max_time  = 0;
  }
  else if (whiteBrightness == HALF_BRIGHTNESS)
  {
    white_half_time = TIM3->CNT;
  }
  else if (whiteBrightness == MAX_BRIGHTNESS)
  {
    white_min_time  = 0;
    white_half_time = 0;
    white_max_time  = TIM3->CNT;
  }
#endif /* ENABLE_UART_DEBUGGING */
  // decrease whiteBrightness
  if (button_held == BUTTON_PRESSED) whiteBrightness -= HOLD_BRIGHTNESS_JUMP;
  else                               whiteBrightness--;

  // prevent from going below MIN_BRIGHTNESS
  if (whiteBrightness < MIN_BRIGHTNESS) whiteBrightness = MIN_BRIGHTNESS;

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
  printf("%s decreased to %d / %d\n", "White", whiteBrightness, MAX_BRIGHTNESS);

  if (whiteBrightness == MIN_BRIGHTNESS)
  {
    white_min_time = TIM3->CNT;
    if ((white_half_time != 0) && button_held) printf("Swept half time to min time in %f seconds\n", (double)(white_min_time - white_half_time) / TIM3_CNT_PER_SEC);
    if ((white_max_time  != 0) && button_held) printf("Swept max time to min time in %f seconds\n",  (double)(white_min_time - white_max_time) / TIM3_CNT_PER_SEC);
  }
  else if (whiteBrightness == HALF_BRIGHTNESS)
  {
    white_half_time = TIM3->CNT;
    if ((white_min_time != 0) && button_held) printf("Swept min time to half time in %f seconds\n", (double)(white_half_time - white_min_time) / TIM3_CNT_PER_SEC);
    if ((white_max_time != 0) && button_held) printf("Swept max time to half time in %f seconds\n", (double)(white_half_time - white_max_time) / TIM3_CNT_PER_SEC);
  }
  else if (whiteBrightness == MAX_BRIGHTNESS)
  {
    white_max_time = TIM3->CNT;
    if ((white_min_time  != 0) && button_held) printf("Swept min time to max time in %f seconds\n",  (double)(white_max_time - white_min_time) / TIM3_CNT_PER_SEC);
    if ((white_half_time != 0) && button_held) printf("Swept half time to max time in %f seconds\n", (double)(white_max_time - white_half_time) / TIM3_CNT_PER_SEC);
  }
#endif /* ENABLE_UART_DEBUGGING */

  // set whiteBrightness
  setWhitePWM();

  return;
}

void decreaseIRBrightness( uint8_t button_held )
{
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
  // set this to ensure that when we sweep we are using the exact value from when we start
  if (irBrightness == MIN_BRIGHTNESS)
  {
    ir_min_time  = TIM3->CNT;
    ir_half_time = 0;
    ir_max_time  = 0;
  }
  else if (irBrightness == HALF_BRIGHTNESS)
  {
    ir_half_time = TIM3->CNT;
  }
  else if (irBrightness == MAX_BRIGHTNESS)
  {
    ir_min_time  = 0;
    ir_half_time = 0;
    ir_max_time  = TIM3->CNT;
  }
#endif /* ENABLE_UART_DEBUGGING */
  // decrease irBrightness
  if (button_held == BUTTON_PRESSED) irBrightness -= HOLD_BRIGHTNESS_JUMP;
  else                               irBrightness--;

  // prevent from going below MIN_BRIGHTNESS
  if (irBrightness < MIN_BRIGHTNESS) irBrightness = MIN_BRIGHTNESS;

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
  printf("%s decreased to %d / %d\n", "IR", irBrightness, MAX_BRIGHTNESS);

  if (irBrightness == MIN_BRIGHTNESS)
  {
    ir_min_time = TIM3->CNT;
    if ((ir_half_time != 0) && button_held) printf("Swept half time to min time in %f seconds\n", (double)(ir_min_time - ir_half_time) / TIM3_CNT_PER_SEC);
    if ((ir_max_time  != 0) && button_held) printf("Swept max time to min time in %f seconds\n",  (double)(ir_min_time - ir_max_time) / TIM3_CNT_PER_SEC);
  }
  else if (irBrightness == HALF_BRIGHTNESS)
  {
    ir_half_time = TIM3->CNT;
    if ((ir_min_time != 0) && button_held) printf("Swept min time to half time in %f seconds\n", (double)(ir_half_time - ir_min_time) / TIM3_CNT_PER_SEC);
    if ((ir_max_time != 0) && button_held) printf("Swept max time to half time in %f seconds\n", (double)(ir_half_time - ir_max_time) / TIM3_CNT_PER_SEC);
  }
  else if (irBrightness == MAX_BRIGHTNESS)
  {
    ir_max_time = TIM3->CNT;
    if ((ir_min_time  != 0) && button_held) printf("Swept min time to max time in %f seconds\n",  (double)(ir_max_time - ir_min_time) / TIM3_CNT_PER_SEC);
    if ((ir_half_time != 0) && button_held) printf("Swept half time to max time in %f seconds\n", (double)(ir_max_time - ir_half_time) / TIM3_CNT_PER_SEC);
  }
#endif /* ENABLE_UART_DEBUGGING */

  // set irBrightness
  setIRPWM();

  return;
}

void increaseWhiteBrightness( uint8_t button_held )
{
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
  // set this to ensure that when we sweep we are using the exact value from when we start
  if (whiteBrightness == MIN_BRIGHTNESS)
  {
    white_min_time  = TIM3->CNT;
    white_half_time = 0;
    white_max_time  = 0;
  }
  else if (whiteBrightness == HALF_BRIGHTNESS)
  {
    white_half_time = TIM3->CNT;
  }
  else if (whiteBrightness == MAX_BRIGHTNESS)
  {
    white_min_time  = 0;
    white_half_time = 0;
    white_max_time  = TIM3->CNT;
  }
#endif /* ENABLE_UART_DEBUGGING */
  // increase whiteBrightness
  if (button_held == BUTTON_PRESSED) whiteBrightness += HOLD_BRIGHTNESS_JUMP;
  else                               whiteBrightness++;

  // prevent from going above MAX_BRIGHTNESS
  if (whiteBrightness > MAX_BRIGHTNESS) whiteBrightness = MAX_BRIGHTNESS;

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
  printf("%s increased to %d / %d\n", "White", whiteBrightness, MAX_BRIGHTNESS);

  if (whiteBrightness == MIN_BRIGHTNESS)
  {
    white_min_time = TIM3->CNT;
    if ((white_half_time != 0) && button_held) printf("Swept half time to min time in %f seconds\n", (double)(white_min_time - white_half_time) / TIM3_CNT_PER_SEC);
    if ((white_max_time  != 0) && button_held) printf("Swept max time to min time in %f seconds\n",  (double)(white_min_time - white_max_time) / TIM3_CNT_PER_SEC);
  }
  else if (whiteBrightness == HALF_BRIGHTNESS)
  {
    white_half_time = TIM3->CNT;
    if ((white_min_time != 0) && button_held) printf("Swept min time to half time in %f seconds\n", (double)(white_half_time - white_min_time) / TIM3_CNT_PER_SEC);
    if ((white_max_time != 0) && button_held) printf("Swept max time to half time in %f seconds\n", (double)(white_half_time - white_max_time) / TIM3_CNT_PER_SEC);
  }
  else if (whiteBrightness == MAX_BRIGHTNESS)
  {
    white_max_time = TIM3->CNT;
    if ((white_min_time  != 0) && button_held) printf("Swept min time to max time in %f seconds\n",  (double)(white_max_time - white_min_time) / TIM3_CNT_PER_SEC);
    if ((white_half_time != 0) && button_held) printf("Swept half time to max time in %f seconds\n", (double)(white_max_time - white_half_time) / TIM3_CNT_PER_SEC);
  }
#endif /* ENABLE_UART_DEBUGGING */

  // set whiteBrightness
  setWhitePWM();

  return;
}

void increaseIRBrightness( uint8_t button_held )
{
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
  // set this to ensure that when we sweep we are using the exact value from when we start
  if (irBrightness == MIN_BRIGHTNESS)
  {
    ir_min_time  = TIM3->CNT;
    ir_half_time = 0;
    ir_max_time  = 0;
  }
  else if (irBrightness == HALF_BRIGHTNESS)
  {
    ir_half_time = TIM3->CNT;
  }
  else if (irBrightness == MAX_BRIGHTNESS)
  {
    ir_min_time  = 0;
    ir_half_time = 0;
    ir_max_time  = TIM3->CNT;
  }
#endif /* ENABLE_UART_DEBUGGING */
  // increase irBrightness
  if (button_held == BUTTON_PRESSED) irBrightness += HOLD_BRIGHTNESS_JUMP;
  else                               irBrightness++;

  // prevent from going above MAX_BRIGHTNESS
  if (irBrightness > MAX_BRIGHTNESS) irBrightness = MAX_BRIGHTNESS;

#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
  printf("%s increased to %d / %d\n", "IR", irBrightness, MAX_BRIGHTNESS);

  if (irBrightness == MIN_BRIGHTNESS)
  {
    ir_min_time = TIM3->CNT;
    if ((ir_half_time != 0) && button_held) printf("Swept half time to min time in %f seconds\n", (double)(ir_min_time - ir_half_time) / TIM3_CNT_PER_SEC);
    if ((ir_max_time  != 0) && button_held) printf("Swept max time to min time in %f seconds\n",  (double)(ir_min_time - ir_max_time) / TIM3_CNT_PER_SEC);
  }
  else if (irBrightness == HALF_BRIGHTNESS)
  {
    ir_half_time = TIM3->CNT;
    if ((ir_min_time != 0) && button_held) printf("Swept min time to half time in %f seconds\n", (double)(ir_half_time - ir_min_time) / TIM3_CNT_PER_SEC);
    if ((ir_max_time != 0) && button_held) printf("Swept max time to half time in %f seconds\n", (double)(ir_half_time - ir_max_time) / TIM3_CNT_PER_SEC);
  }
  else if (irBrightness == MAX_BRIGHTNESS)
  {
    ir_max_time = TIM3->CNT;
    if ((ir_min_time  != 0) && button_held) printf("Swept min time to max time in %f seconds\n",  (double)(ir_max_time - ir_min_time) / TIM3_CNT_PER_SEC);
    if ((ir_half_time != 0) && button_held) printf("Swept half time to max time in %f seconds\n", (double)(ir_max_time - ir_half_time) / TIM3_CNT_PER_SEC);
  }
#endif /* ENABLE_UART_DEBUGGING */

  // set irBrightness
  setIRPWM();

  return;
}

// set White PWM
void setWhitePWM( void )
{
  uint32_t pulse_width = getWhitePWM();
  setPW11(pulse_width);
  startPWM11();
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
  printf("Turn on %s to %u / %u \n", "White", pulse_width, PW_PERIOD);
#endif /* ENABLE_UART_DEBUGGING */
}

// set IR PWM
void setIRPWM( void )
{
  uint32_t pulse_width = getIRPWM();
  setPW12(pulse_width);
  startPWM12();
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
  printf("Turn on %s to %u / %u\n", "IR", pulse_width, PW_PERIOD);
#endif /* ENABLE_UART_DEBUGGING */
  return;
}

// set White PWM
void turnOffWhitePWM( void )
{
  stopPWM11();
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
  printf("Turn off %s\n", "White");
#endif /* ENABLE_UART_DEBUGGING */
}

// set IR PWM
void turnOffIRPWM( void )
{
  stopPWM12();
#ifdef ENABLE_UART_DEBUGGING /* tracing enabled */
  printf("Turn off %s\n", "IR");
#endif /* ENABLE_UART_DEBUGGING */
  return;
}

int8_t getWhiteBrightness( void )
{
  return (whiteBrightness);
}

int8_t getIRBrightness( void )
{
  return (irBrightness);
}

void setWhiteBrightness( int8_t brightness )
{
  if      (brightness > MAX_BRIGHTNESS) whiteBrightness = MAX_BRIGHTNESS;
  else if (brightness < MIN_BRIGHTNESS) whiteBrightness = MIN_BRIGHTNESS;
  else                                  whiteBrightness = brightness;
  return;
}

void setIRBrightness( int8_t brightness )
{
  if      (brightness > MAX_BRIGHTNESS) irBrightness = MAX_BRIGHTNESS;
  else if (brightness < MIN_BRIGHTNESS) irBrightness = MIN_BRIGHTNESS;
  else                                  irBrightness = brightness;
  return;
}

uint8_t getWhitePWM( void )
{
  uint8_t pwm;
  TemperatureRange_e temperature_range = get_temperature_range();

  switch(temperature_range)
  {
    case Cool:
      pwm = whitePWM[whiteBrightness];
      break;
    case Warm:
      pwm = (uint8_t)(whitePWM[whiteBrightness] * WARM_PWM_RATIO + 0.5f);
      break;
    case Hot:
      pwm = (uint8_t)(whitePWM[whiteBrightness] * HOT_PWM_RATIO + 0.5f);
      break;
    default:
      pwm = 0;
      break;
  }

  return (pwm);
}

uint8_t getIRPWM( void )
{
  uint8_t pwm;
  TemperatureRange_e temperature_range = get_temperature_range();

  switch(temperature_range)
  {
    case Cool:
      pwm = irPWM[irBrightness];
      break;
    case Warm:
      pwm = (uint8_t)(irPWM[irBrightness] * WARM_PWM_RATIO + 0.5f);
      break;
    case Hot:
      pwm = (uint8_t)(irPWM[irBrightness] * HOT_PWM_RATIO + 0.5f);
      break;
    default:
      pwm = 0;
      break;
  }

  return (pwm);
}
