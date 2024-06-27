/* Private includes ----------------------------------------------------------*/
#include "pwm_handler.h"
#include "bsp.h"
#include "my_printf.h"

/* Private define ------------------------------------------------------------*/
// todo make these const
#define BRIGHTNESS_STEPS (50)
#define MAX_BRIGHTNESS   (BRIGHTNESS_STEPS - 1)
#define MIN_BRIGHTNESS   (0)
#define HALF_BRIGHTNESS  ((uint8_t)(BRIGHTNESS_STEPS + 0.5f) / 2)

#define HOLD_BRIGHTNESS_JUMP (3)

#define TOGGLE_DELAY_MS     (250)
#define LOWER_SWEEP_TIME_MS (3375)                                          // time from 50%->0%
#define UPPER_SWEEP_TIME_MS (4000)                                          // time from 50%->100%
#define TOTAL_SWEEP_TIME_MS (7375)                                          // time from 0%->100%
#define AVG_SWEEP_TIME_MS   (TOTAL_SWEEP_TIME_MS / 2)                       // 3687.5
#define LOWER_STEP_TIME_MS  (LOWER_SWEEP_TIME_MS / (BRIGHTNESS_STEPS / 2))  // 135, time lower per step
#define UPPER_STEP_TIME_MS  (UPPER_SWEEP_TIME_MS / (BRIGHTNESS_STEPS / 2))  // 160, time upper per step
#define AVG_STEP_TIME_MS    ((UPPER_STEP_TIME_MS + LOWER_STEP_TIME_MS) / 2) // 147.5
#define AVG_STEP_DIFF_MS    (AVG_STEP_TIME_MS    - LOWER_STEP_TIME_MS)      // 12.5, distance between lower step time and average step time
#define LOW_STEP_TIME_MS    (LOWER_STEP_TIME_MS  - AVG_STEP_DIFF_MS)        // 122.5
#define HIGH_STEP_TIME_MS   (UPPER_STEP_TIME_MS  + AVG_STEP_DIFF_MS)        // 172.5

#define LED_PWM_OFF  (0)
#define PW_PERIOD    (255)            // Period of PWM timer
#define MIN_WHITE_PW (PW_PERIOD / 10) // relative pulse width
#define MAX_WHITE_PW (PW_PERIOD)      // relative pulse width
#define MIN_IR_PW    (PW_PERIOD / 10) // relative pulse width
#define MAX_IR_PW    (PW_PERIOD)      // relative pulse width

#define BUTTON_PRESSED (1)

/* Private variables ---------------------------------------------------------*/
static uint8_t whitePWM [ BRIGHTNESS_STEPS ]; // pulse width out of MAX_WHITE_PW
static uint8_t irPWM    [ BRIGHTNESS_STEPS ]; // pulse width out of MAX_IR_PW
// static const uint8_t whitePWM = {26, 30, 35, 40, 44, 49, 54, 58, 63, 68,
                                 // 72, 77, 82, 86, 91, 96, 100, 105, 110, 114,
                                 // 119, 124, 129, 133, 138, 143, 147, 152, 157, 161,
                                 // 166, 171, 175, 180, 185, 189, 194, 199, 203, 208,
                                 // 213, 218, 222, 227, 232, 236, 241, 246, 250, 255};
// static const uint8_t irPWM    = {26, 30, 35, 40, 44, 49, 54, 58, 63, 68,
                                 // 72, 77, 82, 86, 91, 96, 100, 105, 110, 114,
                                 // 119, 124, 129, 133, 138, 143, 147, 152, 157, 161,
                                 // 166, 171, 175, 180, 185, 189, 194, 199, 203, 208,
                                 // 213, 218, 222, 227, 232, 236, 241, 246, 250, 255};
static int8_t  whiteBrightness = 0;
static int8_t  irBrightness = 0;

// Init whitePWM var
// i = [MIN_WHITE_PW, MAX_WHITE_PW]
void initWhitePWM(void)
{
  whiteBrightness = HALF_BRIGHTNESS;
  const float range      = MAX_WHITE_PW - MIN_WHITE_PW;
  const float incPerStep = range / (BRIGHTNESS_STEPS - 1);
  for (uint8_t i = 0; i < BRIGHTNESS_STEPS; i++)
  {
    whitePWM[i] = (uint8_t)(i * incPerStep + 0.5) + MIN_WHITE_PW;
  }
  MY_PRINTF("Init %s PWM\n", "White");
  return;
}

// Init irPWM var
// i = [MIN_IR_PW, MAX_IR_PW]
void initIRPWM(void)
{
  irBrightness = HALF_BRIGHTNESS;
  const float range      = MAX_IR_PW - MIN_IR_PW;
  const float incPerStep = range / (BRIGHTNESS_STEPS - 1);
  for (uint8_t i = 0; i < BRIGHTNESS_STEPS; i++)
  {
    irPWM[i] = (uint8_t)(i * incPerStep + 0.5) + MIN_IR_PW;
  }
  MY_PRINTF("Init %s PWM\n", "IR");
  return;
}

void decreaseWhiteBrightness( uint8_t button_held )
{
  // decrease whiteBrightness
  if (button_held == BUTTON_PRESSED) whiteBrightness -= HOLD_BRIGHTNESS_JUMP;
  else                               whiteBrightness--;

  // prevent from going below MIN_BRIGHTNESS
  if (whiteBrightness < MIN_BRIGHTNESS) whiteBrightness = MIN_BRIGHTNESS;

  MY_PRINTF("%s decreased to %d / %d\n", "White", whiteBrightness, MAX_BRIGHTNESS);

  // set whiteBrightness
  setWhitePWM();

  return;
}

void decreaseIRBrightness( uint8_t button_held )
{
  // decrease irBrightness
  if (button_held == BUTTON_PRESSED) irBrightness -= HOLD_BRIGHTNESS_JUMP;
  else                               irBrightness--;

  // prevent from going below MIN_BRIGHTNESS
  if (irBrightness < MIN_BRIGHTNESS) irBrightness = MIN_BRIGHTNESS;

  MY_PRINTF("%s decreased to %d / %d\n", "IR", irBrightness, MAX_BRIGHTNESS);

  // set irBrightness
  setIRPWM();

  return;
}

void increaseWhiteBrightness( uint8_t button_held )
{
  // increase whiteBrightness
  if (button_held == BUTTON_PRESSED) whiteBrightness += HOLD_BRIGHTNESS_JUMP;
  else                               whiteBrightness++;

  // prevent from going above MAX_BRIGHTNESS
  if (whiteBrightness > MAX_BRIGHTNESS) whiteBrightness = MAX_BRIGHTNESS;

  MY_PRINTF("%s increased to %d / %d\n", "White", whiteBrightness, MAX_BRIGHTNESS);

  // set whiteBrightness
  setWhitePWM();

  return;
}

void increaseIRBrightness( uint8_t button_held )
{
  // increase irBrightness
  if (button_held == BUTTON_PRESSED) irBrightness += HOLD_BRIGHTNESS_JUMP;
  else                               irBrightness++;

  // prevent from going above MAX_BRIGHTNESS
  if (irBrightness > MAX_BRIGHTNESS) irBrightness = MAX_BRIGHTNESS;

  MY_PRINTF("%s increased to %d / %d\n", "IR", irBrightness, MAX_BRIGHTNESS);

  // set irBrightness
  setIRPWM();

  return;
}

// set White PWM
void setWhitePWM( void )
{
  uint32_t pulse_width = whitePWM[whiteBrightness];
  setPW11(pulse_width);
  startPWM11();
  MY_PRINTF("Turn on %s to %u / %u \n", "White", pulse_width, PW_PERIOD);
}

// set IR PWM
void setIRPWM( void )
{
  uint32_t pulse_width = irPWM[irBrightness];
  setPW12(pulse_width);
  startPWM12();
  MY_PRINTF("Turn on %s to %u / %u\n", "IR", pulse_width, PW_PERIOD);
  return;
}

// set White PWM
void turnOffWhitePWM( void )
{
  stopPWM11();
  MY_PRINTF("Turn off %s\n", "White");
}

// set IR PWM
void turnOffIRPWM( void )
{
  stopPWM12();
  MY_PRINTF("Turn off %s\n", "IR");
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
  whiteBrightness = brightness;
  return;
}

void setIRBrightness( int8_t brightness )
{
  irBrightness = brightness;
  return;
}

uint8_t getWhitePWM( void )
{
  return (whitePWM[whiteBrightness]);
}

uint8_t getIRPWM( void )
{
  return (irPWM[irBrightness]);
}
