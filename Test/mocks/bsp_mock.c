#include <stdio.h>

#include "bsp.h"

PwmStruct pwm_white;
PwmStruct pwm_ir;
TimerStruct timer;
GPIO_PinState toggle_pin = 0;
GPIO_PinState dim_pin = 0;
GPIO_PinState bright_pin = 0;
int32_t thermistor_value = 0;
int32_t current_value_mA = 0;
int32_t voltage_value_mV = 28000;
FILE* file_ptr;

GPIO_PinState ReadTogglePin( void )
{
    return toggle_pin;
}

GPIO_PinState ReadDimPin( void )
{
    return dim_pin;
}

GPIO_PinState ReadBrightPin( void )
{
    return bright_pin;
}

void EnablePWM1( void )
{
    return;
}

void DisablePWM1( void )
{
    return;
}

void StartPWM11( void )
{
    pwm_white.is_running = 1;
    return;
}

void StartPWM12( void )
{
    pwm_ir.is_running = 1;
    return;
}

void StopPWM11( void )
{
    pwm_white.is_running = 0;
    return;
}

void StopPWM12( void )
{
    pwm_ir.is_running = 0;
    return;
}

void SetPW11( uint32_t pulse_width )
{
    pwm_white.pulse_width = pulse_width;
    return;
}

void SetPW12( uint32_t pulse_width )
{
    pwm_ir.pulse_width = pulse_width;
    return;
}

void StartTIM2( void )
{
    timer.is_running = 1;
    timer.time = 0;
}

void RestartTIM2( void )
{
    timer.is_running = 1;
    timer.time = 0;
}

uint32_t GetTIM2Cnt( void )
{
    return timer.time;
}

int32_t GetThermistorValue( void )
{
    // assumes value is mC, i.e. 1000 = 1 C
    return thermistor_value;
}

int32_t GetCurrentValue( void )
{
    // assumes value is mA, i.e. 1000 = 1 A
    return current_value_mA;
}

int32_t GetVoltageValue( void )
{
    // assumes value is mV, i.e. 1000 = 1 V
    return voltage_value_mV;
}

void WriteMem( const uint32_t address, const char* const string, const uint32_t bytes )
{
    if (address == 0) rewind(file_ptr);

    fwrite(string, sizeof(char), bytes, file_ptr);
}
void ReadMem(  const uint32_t address, char* string, const uint32_t bytes )
{
    fseek(file_ptr, address, SEEK_SET);

    fread(string, sizeof(char), bytes, file_ptr);
}

void sendUARTChar(char c)
{
    (void)c;
    return;
}

void Error_Handler(void)
{
    return;
}