#include "com.h"
#include "usbd_cdc_if.h"
#include "temperature_handler.h"
#include "current_handler.h"
#include "voltage_handler.h"
#include "logger.h"

#define RECEIVE_PACKET_SIZE     (2)
#define RECORDED_PACKET_SIZE    (2)
#define RECORDED_TEMP_PACKET_SIZE   (2 + TEMPERATURE_LOG_SPACE)   // 302
#define RECORDED_CURR_PACKET_SIZE   (2 + CURRENT_LOG_SPACE)       // 302
#define RECORDED_VOLT_PACKET_SIZE   (2 + VOLTAGE_LOG_SPACE)       // 302
#define LIVE_PACKET_SIZE (8)

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

uint8_t checkLine ( void )
{
    uint8_t response_received = 0;

    if ( receivePacket [ 0 ] == CMD_HDR_CHAR )
    {
        if ( receivePacket [ 1 ] == RECORDED_LOGS )
        {
            response_received = RECORDED_LOGS;
        }
        else if ( receivePacket [ 1 ] == RECORDED_TEMP_LOGS )
        {
            response_received = RECORDED_TEMP_LOGS;
        }
        else if ( receivePacket [ 1 ] == RECORDED_CURR_LOGS )
        {
            response_received = RECORDED_CURR_LOGS;
        }
        else if ( receivePacket [ 1 ] == RECORDED_VOLT_LOGS )
        {
            response_received = RECORDED_VOLT_LOGS;
        }
    }

    memset ( receivePacket, '\0', 2 );

    return response_received;
}

void sendLiveLogs ( void )
{
    uint8_t pk [ LIVE_PACKET_SIZE ] = {RSP_HDR_CHAR, LIVE_LOGS};
    int16_t temperature = 0;
    uint16_t voltage = 0;
    uint16_t current = 0;

    temperature = GetTemperature();
    current = GetCurrent();
    voltage = GetVoltage();

    numToCharArray (&pk [ 2 ], temperature );
    numToCharArray (&pk [ 4 ], current );
    numToCharArray (&pk [ 6 ], voltage );

    CDC_Transmit_FS ( pk, LIVE_PACKET_SIZE );

    return;
}

void sendRecordedLogs ( uint8_t type )
{
    uint8_t* pk;
    uint16_t size = 0;

    switch ( type )
    {
        case RECORDED_TEMP_LOGS:
            size = RECORDED_TEMP_PACKET_SIZE;

            pk = ( uint8_t* ) malloc ( size );
            pk [ 0 ] = RSP_HDR_CHAR;
            pk [ 1 ] = RECORDED_TEMP_LOGS;

            ReadLog ( STARTING_TEMPERATURE_ADDRESS, ( char* ) &pk [ 2 ],
                      TEMPERATURE_LOG_SPACE );

            break;

        case RECORDED_CURR_LOGS:
            size = RECORDED_CURR_PACKET_SIZE;

            pk = ( uint8_t* ) malloc ( size );
            pk [ 0 ] = RSP_HDR_CHAR;
            pk [ 1 ] = RECORDED_CURR_LOGS;

            ReadLog ( STARTING_CURRENT_ADDRESS, ( char* ) &pk [ 2 ], CURRENT_LOG_SPACE );

            break;

        case RECORDED_VOLT_LOGS:
            size = RECORDED_VOLT_PACKET_SIZE;

            pk = ( uint8_t* ) malloc ( size );
            pk [ 0 ] = RSP_HDR_CHAR;
            pk [ 1 ] = RECORDED_VOLT_LOGS;

            ReadLog ( STARTING_VOLTAGE_ADDRESS, ( char* ) &pk [ 2 ], VOLTAGE_LOG_SPACE );

            break;

        default:
            size = RECORDED_PACKET_SIZE;

            pk = ( uint8_t* ) malloc ( size );
            pk [ 0 ] = RSP_HDR_CHAR;
            pk [ 1 ] = RECORDED_LOGS;

            break;
    }

    CDC_Transmit_FS ( pk, size );

    free ( pk );

    return;
}
