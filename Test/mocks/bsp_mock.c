#include <stdio.h>

#include "bsp.h"

pwmStruct pwm_white;
pwmStruct pwm_ir;
timerStruct timer;
GPIO_PinState toggle_pin = 0;
GPIO_PinState dim_pin = 0;
GPIO_PinState bright_pin = 0;
int32_t thermistor_value = 0;
FILE* file_ptr;

GPIO_PinState readTogglePin( void )
{
  return toggle_pin;
}

GPIO_PinState readDimPin( void )
{
  return dim_pin;
}

GPIO_PinState readBrightPin( void )
{
  return bright_pin;
}

void startPWM11( void )
{
  pwm_white.is_running = 1;
  return;
}

void startPWM12( void )
{
  pwm_ir.is_running = 1;
  return;
}

void stopPWM11( void )
{
  pwm_white.is_running = 0;
  return;
}

void stopPWM12( void )
{
  pwm_ir.is_running = 0;
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
  timer.is_running = 1;
  timer.time = 0;
}

void restartTIM2( void )
{
  timer.is_running = 1;
  timer.time = 0;
}

uint32_t getTIM2Cnt( void )
{
  return timer.time;
}

int32_t getThermistorValue( void )
{
  // assumes value is mC, i.e. 1000 = 1 C
  return thermistor_value;
}

void writeMem( const uint32_t address, const char* const string, const uint32_t bytes )
{
  if (address == 0) rewind(file_ptr);

  fwrite(string, sizeof(char), bytes, file_ptr);
}
void readMem(  const uint32_t address, char* string, const uint32_t bytes )
{
  fseek(file_ptr, address, SEEK_SET);

  fread(string, sizeof(char), bytes, file_ptr);
}

void sendUARTChar(char c)
{
  (void)c;
  return;
}