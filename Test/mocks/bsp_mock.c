#include <stdio.h>

#include "stm32l412xx-bsp.h"

PwmStruct pwm_vis;
PwmStruct pwm_ir;
TimerStruct timer;
TimerStruct logTimer;
GPIO_PinState on_off_pin = 0;
GPIO_PinState toggle_pin = 0;
GPIO_PinState dim_pin = 0;
GPIO_PinState bright_pin = 0;
uint16_t thermistor_value_dC = 0;
int16_t current_value_dA = 0;
int16_t voltage_value_dV = 280;
uint8_t writeProtect = 0;
uint8_t chipEnable = 0;
uint8_t writeEnable = 0;
uint8_t statusRegister = 0;
uint8_t readStatusRegisterSignal = 0;

GPIO_PinState ReadOnOffPin( void )
{
    return on_off_pin;
}

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
    pwm_vis.is_running = 1;
    return;
}

void StartPWM12( void )
{
    pwm_ir.is_running = 1;
    return;
}

void StopPWM11( void )
{
    pwm_vis.is_running = 0;
    return;
}

void StopPWM12( void )
{
    pwm_ir.is_running = 0;
    return;
}

void SetPW11( uint32_t pulse_width )
{
    pwm_vis.pulse_width = pulse_width;
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

extern const float Tim2ClkKhz;
uint32_t GetTIM2Cnt( void )
{
    return (uint32_t)(timer.time * Tim2ClkKhz);
}

void StartTIM6( void )
{
    logTimer.is_running = 1;
    logTimer.time = 0;
}

void RestartTIM6( void )
{
    logTimer.is_running = 1;
    logTimer.time = 0;
}

extern const float Tim6ClkKhz;
uint32_t GetTIM6Cnt( void )
{
    return (uint32_t)(logTimer.time * Tim6ClkKhz);
}

uint16_t GetThermistorValue( void )
{
    // value is in dC
    // to get to raw, add 500 to get mV
    // then multiply by 0xFFF / 3300 to get raw value
    return (uint16_t)((thermistor_value_dC + 500.0f) * MV_TO_RAW + 0.5f) ;
}

uint16_t GetCurrentValue( void )
{
    // value is in dA
    // to get to raw, multiply by 100 to get mA
    // divide by 2 to get mV
    // then multiply by 0xFFF / 3300 to get raw value
    return (uint16_t)((current_value_dA * 100.0f / 2.0f) * MV_TO_RAW + 0.5f);
}

uint16_t GetVoltageValue( void )
{
    const float voltage_divider = 15.0f / 264.0f;

    // value is in dC
    // to get to raw, multiply by 100 to get mV
    // divide by voltage divider value to get mV
    // then multiply by 0xFFF / 3300 to get raw value
    return (uint16_t)((voltage_value_dV * 100.0f * voltage_divider) * MV_TO_RAW + 0.5f);
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
    address = 0;
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
