#include "com.h"
#include "usbd_cdc_if.h"
#include "temperature_handler.h"
#include "current_handler.h"
#include "voltage_handler.h"
#include "logger.h"

#define RECEIVE_PACKET_SIZE (2)
// #define RECORDED_PACKET_SIZE (2 + TEMPERATURE_LOG_SPACE + CURRENT_LOG_SPACE + VOLTAGE_LOG_SPACE) // 902
#define RECORDED_PACKET_SIZE (3) // 902
#define LIVE_PACKET_SIZE (8)

#define CMD_HDR_CHAR    ('C')  /* command header character */
#define RSP_HDR_CHAR    ('R')  /* response header character */

uint8_t receivePacket [ RECEIVE_PACKET_SIZE ];

enum
{
    LIVE_LOGS = 0,
    RECORDED_LOGS = 1
};

uint8_t checkLine ( void )
{
    uint8_t response_received = 0;

    if ( receivePacket [ 0 ] == CMD_HDR_CHAR &&
            receivePacket [ 1 ] == RECORDED_LOGS )
    {
        response_received = 1;
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

    pk [ 2 ] = ( temperature & 0x00FF );
    pk [ 3 ] = ( temperature & 0xFF00 ) >> 0x8;
    pk [ 4 ] = ( current & 0x00FF );
    pk [ 5 ] = ( current & 0xFF00 ) >> 0x8;
    pk [ 6 ] = ( voltage & 0x00FF );
    pk [ 7 ] = ( voltage & 0xFF00 ) >> 0x8;

    CDC_Transmit_FS ( pk, LIVE_PACKET_SIZE );

    return;
}

void sendRecordedLogs ( void )
{
    uint8_t pk [ RECORDED_PACKET_SIZE ] = {RSP_HDR_CHAR, RECORDED_LOGS, '3'};
    CDC_Transmit_FS ( pk, RECORDED_PACKET_SIZE );

    return;
}
