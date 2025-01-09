#include "com.h"
#include "usbd_cdc_if.h"
#include "temperature_handler.h"
#include "current_handler.h"
#include "voltage_handler.h"
#include "logger.h"

#define HEADER_PACKET_SIZE          (3)
#define RECEIVE_PACKET_SIZE         (HEADER_PACKET_SIZE)
#define RECORDED_PACKET_SIZE        (HEADER_PACKET_SIZE)
#define RECORDED_TEMP_PACKET_SIZE   (HEADER_PACKET_SIZE + TEMPERATURE_LOG_SPACE)   // 203
#define RECORDED_CURR_PACKET_SIZE   (HEADER_PACKET_SIZE + CURRENT_LOG_SPACE)       // 203
#define RECORDED_VOLT_PACKET_SIZE   (HEADER_PACKET_SIZE + VOLTAGE_LOG_SPACE)       // 203
#define LIVE_PACKET_SIZE            (HEADER_PACKET_SIZE + TEMPERATURE_LOG_SIZE + CURRENT_LOG_SIZE + VOLTAGE_LOG_SIZE) // 9

#define MAX_LOG_SIZE (500) // Bytes

#define CMD_HDR_CHAR    ('C')  /* command header character */
#define RSP_HDR_CHAR    ('R')  /* response header character */

uint8_t receivePacket [ RECEIVE_PACKET_SIZE ];

enum
{
    LIVE_LOGS = 0,
    RECORDED_LOGS = 1,
    RECORDED_TEMP_LOGS = 2,
    RECORDED_CURR_LOGS = 3,
    RECORDED_VOLT_LOGS = 4
};

void checkLine ( uint8_t* response_received )
{
    memset ( response_received, '\0', 2 );

    if ( receivePacket [ 0 ] == CMD_HDR_CHAR )
    {
        memcpy ( response_received, &receivePacket [ 1 ], 2 );
    }

    memset ( receivePacket, '\0', 3 );
}

void sendLiveLogs ( void )
{
    uint8_t pk [ LIVE_PACKET_SIZE ] = {RSP_HDR_CHAR, LIVE_LOGS, 0};
    int16_t temperature = 0;
    uint16_t voltage = 0;
    uint16_t current = 0;

    temperature = GetTemperature();
    current = GetCurrent();
    voltage = GetVoltage();

    numToCharArray (&pk [ HEADER_PACKET_SIZE ], temperature );
    numToCharArray (&pk [ HEADER_PACKET_SIZE + TEMPERATURE_LOG_SIZE ], current );
    numToCharArray (&pk [ HEADER_PACKET_SIZE + TEMPERATURE_LOG_SIZE +
                          CURRENT_LOG_SIZE ], voltage );

    CDC_Transmit_FS ( pk, LIVE_PACKET_SIZE );

    return;
}

void sendRecordedLogs ( uint8_t type, uint8_t num )
{
    uint8_t* pk;
    uint16_t size = 0;

    (void) num;

    switch ( type )
    {
        case RECORDED_TEMP_LOGS:
            size = RECORDED_TEMP_PACKET_SIZE;

            pk = ( uint8_t* ) malloc ( size );
            pk [ 0 ] = RSP_HDR_CHAR;
            pk [ 1 ] = RECORDED_TEMP_LOGS;
            pk [ 2 ] = 0xFF;

            ReadLog ( STARTING_TEMPERATURE_ADDRESS, ( char* ) &pk [ HEADER_PACKET_SIZE ],
                      TEMPERATURE_LOG_SPACE );

            break;

        case RECORDED_CURR_LOGS:
            size = RECORDED_CURR_PACKET_SIZE;

            pk = ( uint8_t* ) malloc ( size );
            pk [ 0 ] = RSP_HDR_CHAR;
            pk [ 1 ] = RECORDED_CURR_LOGS;
            pk [ 2 ] = 0xFF;

            ReadLog ( STARTING_CURRENT_ADDRESS, ( char* ) &pk [ HEADER_PACKET_SIZE ],
                      CURRENT_LOG_SPACE );

            break;

        case RECORDED_VOLT_LOGS:
            size = RECORDED_VOLT_PACKET_SIZE;

            pk = ( uint8_t* ) malloc ( size );
            pk [ 0 ] = RSP_HDR_CHAR;
            pk [ 1 ] = RECORDED_VOLT_LOGS;
            pk [ 2 ] = 0xFF;

            ReadLog ( STARTING_VOLTAGE_ADDRESS, ( char* ) &pk [ HEADER_PACKET_SIZE ],
                      VOLTAGE_LOG_SPACE );

            break;

        default:
            size = RECORDED_PACKET_SIZE;

            pk = ( uint8_t* ) malloc ( size );
            pk [ 0 ] = RSP_HDR_CHAR;
            pk [ 1 ] = RECORDED_LOGS;
            pk [ 2 ] = 0xFF;

            break;
    }

    CDC_Transmit_FS ( pk, size );

    free ( pk );

    return;
}
