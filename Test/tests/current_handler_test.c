#include <string.h>

#include <unity_fixture.h>      /* UNITY */
#include "current_handler.h"    /* CUT */
#include "logger.h"             /* CUT */
#include "stm32l412xx-bsp.h"    /* CUT */

const uint16_t CurrentNormalValue = 30u;
const uint16_t CurrentHighValue = 35u;
const uint16_t CurrentErrorValue = 40u;

extern void initFram ( void );

extern uint32_t address; // last written to address
extern uint16_t current_value_dA;

static uint32_t numCurrentLogs = 0;

TEST_GROUP ( Current_Handler );

TEST_SETUP ( Current_Handler )
{
    /* executed before *every* non-skipped test */
    initFram();
}

TEST_TEAR_DOWN ( Current_Handler )
{
    /* executed after *every* non-skipped and non-failing test */
    current_value_dA = CurrentNormalValue; // return to default value
    GetCurrentRange(); // set current range
}

/* start Current_Handler tests */
TEST ( Current_Handler, GetDefaultCurrent )
{
    TEST_ASSERT_EQUAL_UINT16 ( 0u, GetCurrent() ); // Default value
}

TEST ( Current_Handler, GetDefaultCurrentRange )
{
    TEST_ASSERT_EQUAL_INT ( CurrentNormal, GetCurrentRange() );
}

TEST ( Current_Handler, GetCurrent )
{
    // Max
    current_value_dA = 66;
    TEST_ASSERT_EQUAL_INT32 ( 66, GetCurrent() );

    // Min
    current_value_dA = 0;
    TEST_ASSERT_EQUAL_INT32 ( 0, GetCurrent() );

    current_value_dA = 10u;
    TEST_ASSERT_EQUAL_UINT16 ( 10u, GetCurrent() );
    current_value_dA = 25u;
    TEST_ASSERT_EQUAL_UINT16 ( 25u, GetCurrent() );

    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_UINT16 ( CurrentHighValue, GetCurrent() );

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_UINT16 ( CurrentErrorValue, GetCurrent() );
}

TEST ( Current_Handler, CurrentRangeNormalStepping )
{
    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT ( CurrentHigh, GetCurrentRange() );

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT ( CurrentError, GetCurrentRange() );

    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT ( CurrentHigh, GetCurrentRange() );

    current_value_dA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT ( CurrentNormal, GetCurrentRange() );
}

TEST ( Current_Handler, CurrentRangeNormalTesting )
{
    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT ( CurrentHigh, GetCurrentRange() );

    current_value_dA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT ( CurrentNormal, GetCurrentRange() );

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT ( CurrentError, GetCurrentRange() );
}

TEST ( Current_Handler, CurrentRangeHighTesting )
{
    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT ( CurrentHigh, GetCurrentRange() );

    current_value_dA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT ( CurrentNormal, GetCurrentRange() );

    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT ( CurrentHigh, GetCurrentRange() );

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT ( CurrentError, GetCurrentRange() );

    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT ( CurrentHigh, GetCurrentRange() );
}

TEST ( Current_Handler, CurrentRangeErrorTesting )
{
    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT ( CurrentError, GetCurrentRange() );

    current_value_dA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT ( CurrentNormal, GetCurrentRange() );

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT ( CurrentError, GetCurrentRange() );

    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT ( CurrentHigh, GetCurrentRange() );
}

TEST ( Current_Handler, CurrentRangeNormalBoundaryTesting )
{
    current_value_dA = ( CurrentHighValue - 1 );
    TEST_ASSERT_EQUAL_INT ( CurrentNormal, GetCurrentRange() );

    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT ( CurrentHigh, GetCurrentRange() );

    current_value_dA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT ( CurrentNormal, GetCurrentRange() );

    current_value_dA = ( CurrentErrorValue - 1 );
    TEST_ASSERT_EQUAL_INT ( CurrentHigh, GetCurrentRange() );

    current_value_dA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT ( CurrentNormal, GetCurrentRange() );

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT ( CurrentError, GetCurrentRange() );
}

TEST ( Current_Handler, CurrentRangeHighBoundaryTesting )
{
    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT ( CurrentHigh, GetCurrentRange() );

    current_value_dA = ( CurrentHighValue - 1 );
    TEST_ASSERT_EQUAL_INT ( CurrentNormal, GetCurrentRange() );

    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT ( CurrentHigh, GetCurrentRange() );

    current_value_dA = ( CurrentErrorValue - 1 );
    TEST_ASSERT_EQUAL_INT ( CurrentHigh, GetCurrentRange() );

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT ( CurrentError, GetCurrentRange() );
}

TEST ( Current_Handler, CurrentRangeErrorBoundaryTesting )
{
    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT ( CurrentError, GetCurrentRange() );

    current_value_dA = ( CurrentErrorValue - 1 );
    TEST_ASSERT_EQUAL_INT ( CurrentHigh, GetCurrentRange() );

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT ( CurrentError, GetCurrentRange() );

    current_value_dA = ( CurrentHighValue - 1 );
    TEST_ASSERT_EQUAL_INT ( CurrentNormal, GetCurrentRange() );

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT ( CurrentError, GetCurrentRange() );
}

TEST ( Current_Handler, NormalToHighPrintout )
{

    current_value_dA = CurrentHighValue;
    GetCurrentRange();

    char* expected = "High Current 35 dA\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Current_Handler, NormalToErrorPrintout )
{

    current_value_dA = CurrentErrorValue;
    GetCurrentRange();

    char* expected = "Error Current 40 dA\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Current_Handler, NormalToNormalNoPrintout )
{

    current_value_dA = CurrentNormalValue;
    GetCurrentRange();

    char* expected = "";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Current_Handler, HighToNormalPrintout )
{

    current_value_dA = CurrentHighValue;
    GetCurrentRange();

    current_value_dA = CurrentNormalValue;
    GetCurrentRange();

    char* expected = "Normal Current 30 dA\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Current_Handler, HighToErrorPrintout )
{

    current_value_dA = CurrentHighValue;
    GetCurrentRange();

    current_value_dA = CurrentErrorValue;
    GetCurrentRange();

    char* expected = "Error Current 40 dA\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Current_Handler, HighToHighNoPrintout )
{

    current_value_dA = CurrentHighValue;
    GetCurrentRange();

    current_value_dA = CurrentHighValue;
    GetCurrentRange();
    current_value_dA = CurrentHighValue;
    GetCurrentRange();

    char* expected = "";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Current_Handler, ErrorToNormalPrintout )
{

    current_value_dA = CurrentErrorValue;
    GetCurrentRange();

    current_value_dA = CurrentNormalValue;
    GetCurrentRange();

    char* expected = "Normal Current 30 dA\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Current_Handler, ErrorToHighPrintout )
{

    current_value_dA = CurrentErrorValue;
    GetCurrentRange();

    current_value_dA = CurrentHighValue;
    GetCurrentRange();

    char* expected = "High Current 35 dA\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Current_Handler, ErrorToErrorNoPrintout )
{

    current_value_dA = CurrentErrorValue;
    GetCurrentRange();

    current_value_dA = CurrentErrorValue;
    GetCurrentRange();

    char* expected = "";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Current_Handler, LogCurrent )
{
    char* string = ( char* ) calloc ( CURRENT_LOG_SIZE, sizeof ( char ) );

    current_value_dA = 10;
    int16_t expected = current_value_dA;
    int16_t actual = 0;

    LogCurrent();

    ReadLog ( STARTING_CURRENT_ADDRESS + numCurrentLogs * CURRENT_LOG_SIZE, string,
              CURRENT_LOG_SIZE );

    numFromCharArray ( string, actual );
    TEST_ASSERT_EQUAL_INT16 ( expected, actual );

    numCurrentLogs++;

    if ( numCurrentLogs >= TOTAL_CURRENT_LOGS )
    {
        numCurrentLogs = 0;
    }

    free ( string );
}

TEST ( Current_Handler, findCurrentLogEOL )
{
    char* string = ( char* ) calloc ( CURRENT_LOG_SIZE, sizeof ( char ) );
    int16_t int_str = 0;

    // Log Current
    LogCurrent();

    numCurrentLogs++;

    if ( numCurrentLogs >= TOTAL_CURRENT_LOGS )
    {
        numCurrentLogs = 0;
    }


    // Read EOL
    ReadLog ( STARTING_CURRENT_ADDRESS + numCurrentLogs *
              CURRENT_LOG_SIZE, string,
              CURRENT_LOG_SIZE );

    // Verify it is EOL
    numFromCharArray ( string, int_str );

    TEST_ASSERT_EQUAL_INT16 ( eol_uint_const, int_str );

    // FindEOL Function
    findCurrentLogEOL();

    // Write Log
    LogCurrent();

    numCurrentLogs++;

    if ( numCurrentLogs >= TOTAL_CURRENT_LOGS )
    {
        numCurrentLogs = 0;
    }

    // Read EOL at next
    ReadLog ( STARTING_CURRENT_ADDRESS + numCurrentLogs *
              CURRENT_LOG_SIZE, string,
              CURRENT_LOG_SIZE );

    // Verify it is EOL
    numFromCharArray ( string, int_str );

    TEST_ASSERT_EQUAL_INT16 ( eol_uint_const, int_str );

}

TEST ( Current_Handler, LogCurrentAgain )
{
    char* string = ( char* ) calloc ( CURRENT_LOG_SIZE, sizeof ( char ) );

    current_value_dA = 1;
    int16_t expected = current_value_dA;
    int16_t actual = 0;

    LogCurrent();

    ReadLog ( STARTING_CURRENT_ADDRESS + numCurrentLogs * CURRENT_LOG_SIZE, string,
              CURRENT_LOG_SIZE );

    numFromCharArray ( string, actual );
    TEST_ASSERT_EQUAL_INT16 ( expected, actual );

    numCurrentLogs++;

    if ( numCurrentLogs >= TOTAL_CURRENT_LOGS )
    {
        numCurrentLogs = 0;
    }

    free ( string );
}

TEST ( Current_Handler, LogFiftyCurrents )
{
    char* string = ( char* ) calloc ( CURRENT_LOG_SIZE, sizeof ( char ) );

    const uint8_t logs_to_write = 50;

    for ( uint8_t i = 0; i < logs_to_write; i++ )
    {
        current_value_dA = i;
        LogCurrent();
    }

    for ( uint16_t i = 0; i < logs_to_write; i++ )
    {
        int16_t expected = i;
        int16_t actual = 0;
        ReadLog ( STARTING_CURRENT_ADDRESS + numCurrentLogs * CURRENT_LOG_SIZE, string,
                  CURRENT_LOG_SIZE );

        numFromCharArray ( string, actual );
        TEST_ASSERT_EQUAL_INT16 ( expected, actual );

        numCurrentLogs++;

        if ( numCurrentLogs >= TOTAL_CURRENT_LOGS )
        {
            numCurrentLogs = 0;
        }
    }

    free ( string );
}

TEST ( Current_Handler, ReadWholeLog )
{
    // This just ensures that we don't crash if we write the entire thing

    char* string = ( char* ) calloc ( CURRENT_LOG_SIZE, sizeof ( char ) );

    const uint8_t logs_to_read = TOTAL_CURRENT_LOGS;

    for ( uint8_t i = 0; i < logs_to_read; i++ )
    {
        ReadLog ( STARTING_CURRENT_ADDRESS + numCurrentLogs * CURRENT_LOG_SIZE, string,
                  CURRENT_LOG_SIZE );
        numCurrentLogs++;

        if ( numCurrentLogs >= TOTAL_CURRENT_LOGS )
        {
            numCurrentLogs = 0;
        }
    }

    free ( string );
}

TEST ( Current_Handler, WriteLogOverflow )
{
    char* string = ( char* ) calloc ( CURRENT_LOG_SIZE, sizeof ( char ) );

    // Fill Log Up
    while ( numCurrentLogs < TOTAL_CURRENT_LOGS )
    {
        LogCurrent();
        numCurrentLogs++;
    }

    numCurrentLogs = 0;

    // Write One More
    current_value_dA = 27;
    int16_t expected = current_value_dA;
    int16_t actual = 0;

    LogCurrent();

    // Make sure that last log was written at STARTING_CURRENT_ADDRESS
    ReadLog ( STARTING_CURRENT_ADDRESS, string,
              CURRENT_LOG_SIZE );

    numFromCharArray ( string, actual );
    TEST_ASSERT_EQUAL_INT16 ( expected, actual );

    numCurrentLogs++;

    if ( numCurrentLogs > TOTAL_CURRENT_LOGS )
    {
        numCurrentLogs = 0;
    }

    free ( string );

}

TEST ( Current_Handler, findCurrentLogEOLLast )
{
    char* string = ( char* ) calloc ( CURRENT_LOG_SIZE, sizeof ( char ) );
    int16_t int_str = 0;

    // Fill Log Up Minus Last Spot
    while ( numCurrentLogs < TOTAL_CURRENT_LOGS - 1 )
    {
        LogCurrent();
        numCurrentLogs++;
    }

    const uint32_t last_entry = STARTING_CURRENT_ADDRESS + CURRENT_LOG_SPACE
                                - CURRENT_LOG_SIZE;

    // Read EOL
    ReadLog ( last_entry, string,
              CURRENT_LOG_SIZE );

    // Verify it is EOL
    numFromCharArray ( string, int_str );

    TEST_ASSERT_EQUAL_INT16 ( eol_uint_const, int_str );

    // FindEOL Function
    findCurrentLogEOL();

    // Write Log
    LogCurrent();

    // EOL should now be at beginning
    numCurrentLogs = 0;

    // Read EOL at next
    ReadLog ( STARTING_CURRENT_ADDRESS + numCurrentLogs *
              CURRENT_LOG_SIZE, string,
              CURRENT_LOG_SIZE );

    // Verify it is EOL
    numFromCharArray ( string, int_str );

    TEST_ASSERT_EQUAL_INT16 ( eol_uint_const, int_str );

}

TEST ( Current_Handler, findCurrentLogEOLFirst )
{
    char* string = ( char* ) calloc ( CURRENT_LOG_SIZE, sizeof ( char ) );
    int16_t int_str = 0;

    // Fill Log Up Minus Last Spot
    while ( numCurrentLogs < TOTAL_CURRENT_LOGS )
    {
        LogCurrent();
        numCurrentLogs++;
    }

    // EOL should now be at beginning
    numCurrentLogs = 0;

    // Read EOL
    ReadLog ( STARTING_CURRENT_ADDRESS, string,
              CURRENT_LOG_SIZE );

    // Verify it is EOL
    numFromCharArray ( string, int_str );

    TEST_ASSERT_EQUAL_INT16 ( eol_uint_const, int_str );

    // FindEOL Function
    findCurrentLogEOL();

    // Write Log
    LogCurrent();

    numCurrentLogs++;

    if ( numCurrentLogs > TOTAL_CURRENT_LOGS )
    {
        numCurrentLogs = 0;
    }

    // Read EOL at next
    ReadLog ( STARTING_CURRENT_ADDRESS + numCurrentLogs *
              CURRENT_LOG_SIZE, string,
              CURRENT_LOG_SIZE );

    // Verify it is EOL
    numFromCharArray ( string, int_str );

    TEST_ASSERT_EQUAL_INT16 ( eol_uint_const, int_str );
}

/* end Current_Handler tests */


TEST_GROUP_RUNNER ( Current_Handler )
{
    RUN_TEST_CASE ( Current_Handler, GetDefaultCurrent );
    RUN_TEST_CASE ( Current_Handler, GetDefaultCurrentRange );
    RUN_TEST_CASE ( Current_Handler, GetCurrent );
    RUN_TEST_CASE ( Current_Handler, CurrentRangeNormalStepping );
    RUN_TEST_CASE ( Current_Handler, CurrentRangeNormalTesting );
    RUN_TEST_CASE ( Current_Handler, CurrentRangeHighTesting );
    RUN_TEST_CASE ( Current_Handler, CurrentRangeErrorTesting );
    RUN_TEST_CASE ( Current_Handler, CurrentRangeNormalBoundaryTesting );
    RUN_TEST_CASE ( Current_Handler, CurrentRangeHighBoundaryTesting );
    RUN_TEST_CASE ( Current_Handler, CurrentRangeErrorBoundaryTesting );
    RUN_TEST_CASE ( Current_Handler, NormalToHighPrintout );
    RUN_TEST_CASE ( Current_Handler, NormalToErrorPrintout );
    RUN_TEST_CASE ( Current_Handler, NormalToNormalNoPrintout );
    RUN_TEST_CASE ( Current_Handler, HighToNormalPrintout );
    RUN_TEST_CASE ( Current_Handler, HighToErrorPrintout );
    RUN_TEST_CASE ( Current_Handler, HighToHighNoPrintout );
    RUN_TEST_CASE ( Current_Handler, ErrorToNormalPrintout );
    RUN_TEST_CASE ( Current_Handler, ErrorToHighPrintout );
    RUN_TEST_CASE ( Current_Handler, ErrorToErrorNoPrintout );
    RUN_TEST_CASE ( Current_Handler, LogCurrent );
    RUN_TEST_CASE ( Current_Handler, findCurrentLogEOL );
    RUN_TEST_CASE ( Current_Handler, LogCurrentAgain );
    RUN_TEST_CASE ( Current_Handler, LogFiftyCurrents );
    RUN_TEST_CASE ( Current_Handler, ReadWholeLog );
    RUN_TEST_CASE ( Current_Handler, WriteLogOverflow );
    RUN_TEST_CASE ( Current_Handler, findCurrentLogEOLLast );
    RUN_TEST_CASE ( Current_Handler, findCurrentLogEOLFirst );
}
