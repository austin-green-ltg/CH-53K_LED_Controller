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
    RECORDED_VOLT_LOGS = 4,
    INFO = UINT8_MAX - 1
};

//static void sendInfo (uint8_t info)
//{
//
// uint8_t pk[3] = {RSP_HDR_CHAR, INFO, info};
//
// CDC_Transmit_FS ( pk, HEADER_PACKET_SIZE );
//
// return;
//}

void checkLine ( uint8_t* response_received )
{
    if ( receivePacket [ 0 ] == CMD_HDR_CHAR )
    {
        response_received[0] = receivePacket[1];
        response_received[1] = receivePacket[2];
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
    uint16_t packet_size = 0;
    uint16_t log_size = 0;
    uint32_t total_sent = num * MAX_LOG_SIZE;
    uint8_t log_num = 0xFF;
    uint32_t address = 0;

    switch ( type )
    {
        case RECORDED_TEMP_LOGS:

            address = STARTING_TEMPERATURE_ADDRESS + total_sent;

            if ( ( total_sent + MAX_LOG_SIZE ) < TEMPERATURE_LOG_SPACE )
            {
                // full log able to be sent
                log_size = MAX_LOG_SIZE;

                log_num = num;

            }
            else
            {
                // last of logs
                log_size = TEMPERATURE_LOG_SPACE - total_sent;
            }

            break;

        case RECORDED_CURR_LOGS:

            address = STARTING_CURRENT_ADDRESS + total_sent;

            if ( ( total_sent + MAX_LOG_SIZE ) < CURRENT_LOG_SPACE )
            {
                // full log able to be sent
                log_size = MAX_LOG_SIZE;

                log_num = num;
            }
            else
            {
                // last of logs
                log_size = CURRENT_LOG_SPACE - total_sent;
            }

            break;

        case RECORDED_VOLT_LOGS:

            address = STARTING_VOLTAGE_ADDRESS + total_sent;

            if ( ( total_sent + MAX_LOG_SIZE ) < VOLTAGE_LOG_SPACE )
            {
                // full log able to be sent
                log_size = MAX_LOG_SIZE;
                log_num = num;
            }
            else
            {
                // last of logs
                log_size = VOLTAGE_LOG_SPACE - total_sent;
            }

            break;

        default:

            break;
    }

    packet_size = HEADER_PACKET_SIZE + log_size;
    pk = ( uint8_t* ) malloc ( packet_size );

    pk [ 0 ] = RSP_HDR_CHAR;
    pk [ 1 ] = type;
    pk [ 2 ] = log_num;

    ReadLog ( address, ( char* ) &pk [ HEADER_PACKET_SIZE ],
              log_size );

    CDC_Transmit_FS ( pk, packet_size );

    free ( pk );

    // Not ideal, but prevents timeouts and only used occasionally
    HAL_Delay ( 100 );

    return;
}
