#include <stdio.h>

#include "stm32l412xx-bsp.h"

PwmStruct pwm;
TimerStruct timer;
GPIO_PinState toggle_pin = 0;
GPIO_PinState dim_pin = 0;
GPIO_PinState bright_pin = 0;
int16_t thermistor_value_dC = 0;
int16_t current_value_dA = 0;
int16_t voltage_value_dV = 280;
FILE* file_ptr;

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
    pwm.is_running = 1;
    return;
}

void StopPWM11( void )
{
    pwm.is_running = 0;
    return;
}

void SetPW11( uint32_t pulse_width )
{
    pwm.pulse_width = pulse_width;
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

int16_t GetThermistorValue( void )
{
    // assumes value is dC, i.e. 10 = 1 C
    return thermistor_value_dC;
}

int16_t GetCurrentValue( void )
{
    // assumes value is dA, i.e. 10 = 1 A
    return current_value_dA;
}

int16_t GetVoltageValue( void )
{
    // assumes value is dV, i.e. 10 = 1 V
    return voltage_value_dV;
}

void WriteMem( const uint32_t address, const char* const string, const uint32_t bytes )
{
    if (address == 0) rewind(file_ptr);

    fwrite(string, sizeof(char), bytes, file_ptr);
}
void ReadMem( const uint32_t address, char* string, const uint32_t bytes )
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
