#include <string.h>

#include "bsp.h"
#include "logger.h"

static uint32_t tail_pointer = 0;

void Logger_Create( void )
{
  tail_pointer = 0;
}
void Logger_Destroy( void )
{
  tail_pointer = 0;
}

void logString( const char* const string, uint8_t write_beginning )
{
  uint32_t write_bytes = strlen(string);
  uint32_t address = tail_pointer;
  tail_pointer += write_bytes;
  if (write_beginning)
  {
    address = 0;
    tail_pointer = 0;
  }

  if (string == NULL) return;
  writeMem( address, string, write_bytes );
}
void readLog(  const uint32_t address, char* string, const uint32_t bytes )
{
  uint32_t read_bytes = bytes;
  if (string == NULL) return;
  if (bytes == 0)
  {
    string = "";
    return;
  }
  readMem( address, string, read_bytes );
}