#include <unity_fixture.h>      /* UNITY */
#include <string.h>             /* CUT */
#include "com.h"                /* CUT */
#include "stm32l412xx-bsp.h"    /* CUT */

extern void fillFram ( void );

extern uint8_t receivePacket [ RECEIVE_PACKET_SIZE ];
extern uint8_t serialBuffer [ UINT16_MAX ];
extern int16_t thermistor_value_dC;
extern uint16_t current_value_dA;
extern uint16_t voltage_value_dV;
extern uint16_t last_len;

// rounds logs so that last log is appropriate number
// log num is 0 based so 5000 / 500 has 0-9
// 5001 / 500 has 0-10 because we need to read one more log
const uint8_t last_temp_log_num = ( uint8_t ) ( TEMPERATURE_LOG_SPACE /
                                  MAX_LOG_SIZE
                                  - 0.001f ); // 19
const uint8_t last_curr_log_num = ( uint8_t ) ( CURRENT_LOG_SPACE / MAX_LOG_SIZE
                                  - 0.001f ); // 19
const uint8_t last_volt_log_num = ( uint8_t ) ( VOLTAGE_LOG_SPACE / MAX_LOG_SIZE
                                  - 0.001f ); // 19

TEST_GROUP ( Com );

TEST_SETUP ( Com )
{
    /* executed before *every* non-skipped test */
    memset ( receivePacket, 0, RECEIVE_PACKET_SIZE );
    fillFram();
}

TEST_TEAR_DOWN ( Com )
{
    /* executed after *every* non-skipped and non-failing test */
}

/* start com tests */
TEST ( Com, checkLine )
{
    receivePacket [ 0 ] = CMD_HDR_CHAR;
    receivePacket [ 1 ] = RECORDED_TEMP_LOGS;
    receivePacket [ 2 ] = ( uint8_t ) rand();

    uint8_t expected_response [ 2 ] = { receivePacket [ 1 ], receivePacket [ 2 ] };

    uint8_t response_received [ 2 ] = {0, 0};

    checkLine ( response_received );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_response, response_received, 2 );
}

TEST ( Com, checkLineNoResponse )
{
    receivePacket [ 0 ] = RSP_HDR_CHAR;
    receivePacket [ 1 ] = RECORDED_CURR_LOGS;
    receivePacket [ 2 ] = ( uint8_t ) rand();

    uint8_t expected_response [ 2 ] = { 0, 0 }; // Not CMD_HDR_CHAR so no response expected

    uint8_t response_received [ 2 ] = { 0, 0 };

    checkLine ( response_received );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_response, response_received, 2 );
}

TEST ( Com, sendLiveLogs )
{
    thermistor_value_dC = rand() % 1150;
    current_value_dA = rand() % 660;
    voltage_value_dV = rand() % 280;

    uint8_t expected_pk [ LIVE_PACKET_SIZE ] = { RSP_HDR_CHAR, LIVE_LOGS, 0 };
    numToCharArray (&expected_pk [ HEADER_PACKET_SIZE ], thermistor_value_dC );
    numToCharArray (&expected_pk [ HEADER_PACKET_SIZE + TEMPERATURE_LOG_SIZE ],
                    current_value_dA );
    numToCharArray (&expected_pk [ HEADER_PACKET_SIZE + TEMPERATURE_LOG_SIZE +
                                   CURRENT_LOG_SIZE ], voltage_value_dV );

    sendLiveLogs();

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer, LIVE_PACKET_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( LIVE_PACKET_SIZE, last_len );
}

TEST ( Com, sendRecordedLogsTemp )
{
    uint8_t type = RECORDED_TEMP_LOGS;
    uint8_t num = 0;
    uint32_t address = STARTING_TEMPERATURE_ADDRESS;

    uint8_t expected_pk [ HEADER_PACKET_SIZE + MAX_LOG_SIZE ] = { RSP_HDR_CHAR, type, num };

    ReadLog ( address, ( char* ) & expected_pk [ HEADER_PACKET_SIZE ],
              MAX_LOG_SIZE );

    sendRecordedLogs ( type, num );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer,
                                    HEADER_PACKET_SIZE + MAX_LOG_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( HEADER_PACKET_SIZE + MAX_LOG_SIZE, last_len );
}

TEST ( Com, sendRecordedLogsTempNumLast )
{
    // Should return no logs
    uint8_t type = RECORDED_TEMP_LOGS;
    uint8_t num = last_temp_log_num; // 19 Reaches end of logs
    uint32_t address = STARTING_TEMPERATURE_ADDRESS + num * MAX_LOG_SIZE;

    uint8_t expected_pk [ HEADER_PACKET_SIZE + MAX_LOG_SIZE ] = { RSP_HDR_CHAR, type, 0xFF };

    ReadLog ( address, ( char* ) &expected_pk [ HEADER_PACKET_SIZE ],
              MAX_LOG_SIZE );

    sendRecordedLogs ( type, num );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer,
                                    HEADER_PACKET_SIZE + MAX_LOG_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( HEADER_PACKET_SIZE + MAX_LOG_SIZE, last_len );
}

TEST ( Com, sendRecordedLogsTempNumSecondLast )
{
    // Should return no logs
    uint8_t type = RECORDED_TEMP_LOGS;
    uint8_t num = last_temp_log_num - 1; // 18 Doesn't reach end of logs
    uint32_t address = STARTING_TEMPERATURE_ADDRESS + num * MAX_LOG_SIZE;

    uint8_t expected_pk [ HEADER_PACKET_SIZE + MAX_LOG_SIZE ] = { RSP_HDR_CHAR, type, num };

    ReadLog ( address, ( char* ) &expected_pk [ HEADER_PACKET_SIZE ],
              MAX_LOG_SIZE );

    sendRecordedLogs ( type, num );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer,
                                    HEADER_PACKET_SIZE + MAX_LOG_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( HEADER_PACKET_SIZE + MAX_LOG_SIZE, last_len );
}

TEST ( Com, sendRecordedLogsTempNumPastLast )
{
    // Should return no logs
    uint8_t type = RECORDED_TEMP_LOGS;
    uint8_t num = last_temp_log_num + 1; // 20 Past end of logs

    uint8_t expected_pk [ HEADER_PACKET_SIZE + MAX_LOG_SIZE ] = { RSP_HDR_CHAR, type, 0xFF };

    sendRecordedLogs ( type, num );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer, HEADER_PACKET_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( HEADER_PACKET_SIZE, last_len );
}

TEST ( Com, sendRecordedLogsCurr )
{
    uint8_t type = RECORDED_CURR_LOGS;
    uint8_t num = 0;
    uint32_t address = STARTING_CURRENT_ADDRESS;

    uint8_t expected_pk [ HEADER_PACKET_SIZE + MAX_LOG_SIZE ] = { RSP_HDR_CHAR, type, num };

    ReadLog ( address, ( char* ) & expected_pk [ HEADER_PACKET_SIZE ],
              MAX_LOG_SIZE );

    sendRecordedLogs ( type, num );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer,
                                    HEADER_PACKET_SIZE + MAX_LOG_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( HEADER_PACKET_SIZE + MAX_LOG_SIZE, last_len );
}

TEST ( Com, sendRecordedLogsCurrNumLast )
{
    // Should return no logs
    uint8_t type = RECORDED_CURR_LOGS;
    uint8_t num = last_curr_log_num; // 19 Reaches end of logs
    uint32_t address = STARTING_CURRENT_ADDRESS + num * MAX_LOG_SIZE;

    uint8_t expected_pk [ HEADER_PACKET_SIZE + MAX_LOG_SIZE ] = { RSP_HDR_CHAR, type, 0xFF };

    ReadLog ( address, ( char* ) &expected_pk [ HEADER_PACKET_SIZE ],
              MAX_LOG_SIZE );

    sendRecordedLogs ( type, num );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer,
                                    HEADER_PACKET_SIZE + MAX_LOG_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( HEADER_PACKET_SIZE + MAX_LOG_SIZE, last_len );
}

TEST ( Com, sendRecordedLogsCurrNumSecondLast )
{
    // Should return no logs
    uint8_t type = RECORDED_CURR_LOGS;
    uint8_t num = last_curr_log_num - 1; // 18 Doesn't reach end of logs
    uint32_t address = STARTING_CURRENT_ADDRESS + num * MAX_LOG_SIZE;

    uint8_t expected_pk [ HEADER_PACKET_SIZE + MAX_LOG_SIZE ] = { RSP_HDR_CHAR, type, num };

    ReadLog ( address, ( char* ) &expected_pk [ HEADER_PACKET_SIZE ],
              MAX_LOG_SIZE );

    sendRecordedLogs ( type, num );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer,
                                    HEADER_PACKET_SIZE + MAX_LOG_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( HEADER_PACKET_SIZE + MAX_LOG_SIZE, last_len );
}

TEST ( Com, sendRecordedLogsCurrNumPastLast )
{
    // Should return no logs
    uint8_t type = RECORDED_CURR_LOGS;
    uint8_t num = last_curr_log_num + 1; // 20 Past end of logs

    uint8_t expected_pk [ HEADER_PACKET_SIZE + MAX_LOG_SIZE ] = { RSP_HDR_CHAR, type, 0xFF };

    sendRecordedLogs ( type, num );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer, HEADER_PACKET_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( HEADER_PACKET_SIZE, last_len );
}

TEST ( Com, sendRecordedLogsVolt )
{
    uint8_t type = RECORDED_VOLT_LOGS;
    uint8_t num = 0;
    uint32_t address = STARTING_VOLTAGE_ADDRESS;

    uint8_t expected_pk [ HEADER_PACKET_SIZE + MAX_LOG_SIZE ] = { RSP_HDR_CHAR, type, num };

    ReadLog ( address, ( char* ) & expected_pk [ HEADER_PACKET_SIZE ],
              MAX_LOG_SIZE );

    sendRecordedLogs ( type, num );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer,
                                    HEADER_PACKET_SIZE + MAX_LOG_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( HEADER_PACKET_SIZE + MAX_LOG_SIZE, last_len );
}

TEST ( Com, sendRecordedLogsVoltNumLast )
{
    // Should return no logs
    uint8_t type = RECORDED_VOLT_LOGS;
    uint8_t num = last_volt_log_num; // 19 Reaches end of logs
    uint32_t address = STARTING_VOLTAGE_ADDRESS + num * MAX_LOG_SIZE;

    uint8_t expected_pk [ HEADER_PACKET_SIZE + MAX_LOG_SIZE ] = { RSP_HDR_CHAR, type, 0xFF };

    ReadLog ( address, ( char* ) &expected_pk [ HEADER_PACKET_SIZE ],
              MAX_LOG_SIZE );

    sendRecordedLogs ( type, num );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer,
                                    HEADER_PACKET_SIZE + MAX_LOG_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( HEADER_PACKET_SIZE + MAX_LOG_SIZE, last_len );
}

TEST ( Com, sendRecordedLogsVoltNumSecondLast )
{
    // Should return no logs
    uint8_t type = RECORDED_VOLT_LOGS;
    uint8_t num = last_volt_log_num - 1; // 18 Doesn't reach end of logs
    uint32_t address = STARTING_VOLTAGE_ADDRESS + num * MAX_LOG_SIZE;

    uint8_t expected_pk [ HEADER_PACKET_SIZE + MAX_LOG_SIZE ] = { RSP_HDR_CHAR, type, num };

    ReadLog ( address, ( char* ) &expected_pk [ HEADER_PACKET_SIZE ],
              MAX_LOG_SIZE );

    sendRecordedLogs ( type, num );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer,
                                    HEADER_PACKET_SIZE + MAX_LOG_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( HEADER_PACKET_SIZE + MAX_LOG_SIZE, last_len );
}

TEST ( Com, sendRecordedLogsVoltNumPastLast )
{
    // Should return no logs
    uint8_t type = RECORDED_VOLT_LOGS;
    uint8_t num = last_volt_log_num + 1; // 20 Past end of logs

    uint8_t expected_pk [ HEADER_PACKET_SIZE + MAX_LOG_SIZE ] = { RSP_HDR_CHAR, type, 0xFF };

    sendRecordedLogs ( type, num );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer, HEADER_PACKET_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( HEADER_PACKET_SIZE, last_len );
}

TEST ( Com, sendRecordedLogsElse )
{
    uint8_t type = LIVE_LOGS;
    uint8_t num = ( uint8_t ) rand();

    uint8_t expected_pk [ HEADER_PACKET_SIZE ] = { RSP_HDR_CHAR, type, 0xFF };

    sendRecordedLogs ( type, num );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer, HEADER_PACKET_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( HEADER_PACKET_SIZE, last_len );
}

TEST ( Com, sendRecordedLogsNum01 )
{
    uint8_t type = RECORDED_TEMP_LOGS;
    uint8_t num = 1;
    uint32_t address = STARTING_TEMPERATURE_ADDRESS + num * MAX_LOG_SIZE;

    uint8_t expected_pk [ HEADER_PACKET_SIZE + MAX_LOG_SIZE ] = { RSP_HDR_CHAR, type, num };

    ReadLog ( address, ( char* ) & expected_pk [ HEADER_PACKET_SIZE ],
              MAX_LOG_SIZE );

    sendRecordedLogs ( type, num );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer,
                                    HEADER_PACKET_SIZE + MAX_LOG_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( HEADER_PACKET_SIZE + MAX_LOG_SIZE, last_len );
}

TEST ( Com, sendRecordedLogsNumFE )
{
    // Should return no logs
    uint8_t type = RECORDED_TEMP_LOGS;
    uint8_t num = 0xFE;
    uint32_t address = STARTING_TEMPERATURE_ADDRESS + num * MAX_LOG_SIZE;

    uint8_t expected_pk [ HEADER_PACKET_SIZE + MAX_LOG_SIZE ] = { RSP_HDR_CHAR, type, 0xFF };

    ReadLog ( address, ( char* ) & expected_pk [ HEADER_PACKET_SIZE ],
              0 );

    sendRecordedLogs ( type, num );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer, HEADER_PACKET_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( HEADER_PACKET_SIZE, last_len );
}

TEST ( Com, sendRecordedLogsNumFF )
{
    // Should return no logs
    uint8_t type = RECORDED_TEMP_LOGS;
    uint8_t num = 0xFF;
    uint32_t address = STARTING_TEMPERATURE_ADDRESS + num * MAX_LOG_SIZE;

    uint8_t expected_pk [ HEADER_PACKET_SIZE + MAX_LOG_SIZE ] = { RSP_HDR_CHAR, type, 0xFF };

    ReadLog ( address, ( char* ) & expected_pk [ HEADER_PACKET_SIZE ],
              0 );

    sendRecordedLogs ( type, num );

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( expected_pk, serialBuffer, HEADER_PACKET_SIZE );
    TEST_ASSERT_EQUAL_UINT16 ( HEADER_PACKET_SIZE, last_len );
}

/* end com tests */

TEST_GROUP_RUNNER ( Com )
{
    RUN_TEST_CASE ( Com, checkLine );
    RUN_TEST_CASE ( Com, checkLineNoResponse );
    RUN_TEST_CASE ( Com, sendLiveLogs );
    RUN_TEST_CASE ( Com, sendRecordedLogsTemp );
    RUN_TEST_CASE ( Com, sendRecordedLogsTempNumLast );
    RUN_TEST_CASE ( Com, sendRecordedLogsTempNumSecondLast );
    RUN_TEST_CASE ( Com, sendRecordedLogsTempNumPastLast );
    RUN_TEST_CASE ( Com, sendRecordedLogsCurr );
    RUN_TEST_CASE ( Com, sendRecordedLogsCurrNumLast );
    RUN_TEST_CASE ( Com, sendRecordedLogsCurrNumSecondLast );
    RUN_TEST_CASE ( Com, sendRecordedLogsCurrNumPastLast );
    RUN_TEST_CASE ( Com, sendRecordedLogsVolt );
    RUN_TEST_CASE ( Com, sendRecordedLogsVoltNumLast );
    RUN_TEST_CASE ( Com, sendRecordedLogsVoltNumSecondLast );
    RUN_TEST_CASE ( Com, sendRecordedLogsVoltNumPastLast );
    RUN_TEST_CASE ( Com, sendRecordedLogsElse );
    RUN_TEST_CASE ( Com, sendRecordedLogsNum01 );
    RUN_TEST_CASE ( Com, sendRecordedLogsNumFE );
    RUN_TEST_CASE ( Com, sendRecordedLogsNumFF );
}
