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
uint8_t writeProtect = 0;
uint8_t chipEnable = 0;
uint8_t writeEnable = 0;
uint8_t statusRegister = 0;
uint8_t readStatusRegisterSignal = 0;

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


typedef enum {

  OC_WREN  = 6 ,  /**< set write enable latch */
  OC_WRDI  = 4 ,  /**< write disable          */
  OC_RDSR  = 5 ,  /**< read status register   */
  OC_WRSR  = 1 ,  /**< write status register  */
  OC_READ  = 3 ,  /**< read memory data       */
  OC_WRITE = 2 ,  /**< write memory data      */

} OPCODE_COMMANDS ;

void enableWriteProtect( void )
{
    writeProtect = 1;
}

void disableWriteProtect( void )
{
    writeProtect = 0;
}

void enableChipSelect( void )
{
    chipEnable = 1;
}

void disableChipSelect( void )
{
    chipEnable = 0;
}

#define FRAM_SIZE 0xFFFF
static uint8_t framMem[FRAM_SIZE];
uint32_t address = 0;
void initFram( void );
void initFram( void )
{
    for (uint32_t i = 0; i < FRAM_SIZE; i++) framMem[i] = 0;
}

void transferData( const unsigned char* const txData, const uint32_t bytes )
{
    if ((txData[0] == OC_WRDI || txData[0] == OC_WREN) && bytes == 1)
    {
        writeEnable = (txData[0] == OC_WRDI) ? 0 : 1;
    }
    // Write Status Register
    else if (txData[0] == OC_WRSR && bytes == 2)
    {
        statusRegister = txData[1];
    }
    // Signal Read Status Register
    else if (txData[0] == OC_RDSR && bytes == 2)
    {
        readStatusRegisterSignal = 1;
    }
    else if ((txData[0] == OC_READ || txData[0] == OC_WRITE) && bytes == 3)
    {
        address = (txData[1] << 8) + txData[2];
    }
    else
    {
        for (uint32_t i = address; i < bytes + address; i++)
        {
            framMem[i] = txData[i - address];
        }
    }
}

void receiveData( unsigned char* rxData, const uint32_t bytes )
{
    if (readStatusRegisterSignal)
    {
        rxData[1] = statusRegister;
        readStatusRegisterSignal = 0;
    }
    else
    {
        for (uint32_t i = address; i < bytes + address; i++)
        {
            rxData[i - address] = framMem[i];
        }
    }
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
