#ifndef INC_comh
#define INC_comh

#include <stdint.h>
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

enum
{
    LIVE_LOGS = 0,
    RECORDED_LOGS = 1,
    RECORDED_TEMP_LOGS = 2,
    RECORDED_CURR_LOGS = 3,
    RECORDED_VOLT_LOGS = 4,
    INFO = UINT8_MAX - 1
};

void checkLine ( uint8_t* response_received );

void sendLiveLogs ( void );

void sendRecordedLogs ( uint8_t type, uint8_t num );

#endif /* INC_comh */
