#ifndef INC_loggerh
#define INC_loggerh

void Logger_Create( void );
void Logger_Destroy( void );

void logString( const char* const string, uint8_t write_beginning );
void readLog(  const uint32_t address, char* string, const uint32_t bytes );


#endif /* INC_loggerh */