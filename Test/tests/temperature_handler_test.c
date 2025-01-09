#include <string.h>

#include <unity_fixture.h>          /* UNITY */
#include "temperature_handler.h"    /* CUT */
#include "logger.h"                 /* CUT */
#include "stm32l412xx-bsp.h"        /* CUT */

const int16_t HeatingWarmTherm_dC = 1000u;
const int16_t HeatingHotTherm_dC = 1200u;
const int16_t CoolingCoolTherm_dC = 800u;
const int16_t CoolingWarmTherm_dC = 1000u;

extern void initFram ( void );

extern uint32_t address; // last written to address
extern int16_t thermistor_value_dC;

static uint32_t numTemperatureLogs = 0;

TEST_GROUP ( Temperature_Handler );

TEST_SETUP ( Temperature_Handler )
{
    /* executed before *every* non-skipped test */
    initFram();
}

TEST_TEAR_DOWN ( Temperature_Handler )
{
    /* executed after *every* non-skipped and non-failing test */
    thermistor_value_dC = 0; // return to default value
    GetTemperatureRange(); // set temperature range
}

/* TESTS */
/*
    * Check get values
    * Check get range
*/

/* start temperature_handler tests */
TEST ( Temperature_Handler, GetDefaultTemperature )
{
    TEST_ASSERT_EQUAL_INT32 ( 0, GetTemperature() ); // Default value
}

TEST ( Temperature_Handler, GetDefaultTemperatureRange )
{
    TEST_ASSERT ( GetTemperatureRange() == TempCool );
}

TEST ( Temperature_Handler, GetTemperature )
{
    // Max
    thermistor_value_dC = 2800;
    TEST_ASSERT_EQUAL_INT32 ( 2800, GetTemperature() );

    // Min
    thermistor_value_dC = -500;
    TEST_ASSERT_EQUAL_INT32 (-500, GetTemperature() );

    thermistor_value_dC = 200;
    TEST_ASSERT_EQUAL_INT32 ( 200, GetTemperature() );

    thermistor_value_dC = HeatingWarmTherm_dC;
    TEST_ASSERT_EQUAL_INT32 ( HeatingWarmTherm_dC, GetTemperature() );

    thermistor_value_dC = HeatingHotTherm_dC;
    TEST_ASSERT_EQUAL_INT32 ( HeatingHotTherm_dC, GetTemperature() );

    thermistor_value_dC = CoolingCoolTherm_dC;
    TEST_ASSERT_EQUAL_INT32 ( CoolingCoolTherm_dC, GetTemperature() );

    thermistor_value_dC = CoolingWarmTherm_dC;
    TEST_ASSERT_EQUAL_INT32 ( CoolingWarmTherm_dC, GetTemperature() );

    thermistor_value_dC = 1500;
    TEST_ASSERT_EQUAL_INT32 ( 1500, GetTemperature() );

    thermistor_value_dC = 700;
    TEST_ASSERT_EQUAL_INT32 ( 700, GetTemperature() );

    thermistor_value_dC = 1000;
    TEST_ASSERT_EQUAL_INT32 ( 1000, GetTemperature() );

    thermistor_value_dC = 1200;
    TEST_ASSERT_EQUAL_INT32 ( 1200, GetTemperature() );

    thermistor_value_dC = 1500;
    TEST_ASSERT_EQUAL_INT32 ( 1500, GetTemperature() );
}

TEST ( Temperature_Handler, TemperatureRangeNormalStepping )
{
    thermistor_value_dC = HeatingWarmTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value_dC = HeatingHotTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempHot );

    thermistor_value_dC = CoolingWarmTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value_dC = CoolingCoolTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempCool );
}

TEST ( Temperature_Handler, TemperatureRangeCoolTesting )
{
    thermistor_value_dC = HeatingWarmTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value_dC = CoolingCoolTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempCool );

    thermistor_value_dC = HeatingHotTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempHot );
}

TEST ( Temperature_Handler, TemperatureRangeWarmTesting )
{
    thermistor_value_dC = HeatingWarmTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value_dC = CoolingCoolTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempCool );

    thermistor_value_dC = HeatingWarmTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value_dC = HeatingHotTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempHot );

    thermistor_value_dC = CoolingWarmTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );
}

TEST ( Temperature_Handler, TemperatureRangeHotTesting )
{
    thermistor_value_dC = HeatingHotTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempHot );

    thermistor_value_dC = CoolingCoolTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempCool );

    thermistor_value_dC = HeatingHotTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempHot );

    thermistor_value_dC = CoolingWarmTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );
}

TEST ( Temperature_Handler, TemperatureRangeCoolBoundaryTesting )
{
    thermistor_value_dC = ( HeatingWarmTherm_dC - 10 );
    TEST_ASSERT ( GetTemperatureRange() == TempCool );

    thermistor_value_dC = HeatingWarmTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value_dC = CoolingCoolTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempCool );

    thermistor_value_dC = ( HeatingHotTherm_dC - 10 );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value_dC = CoolingCoolTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempCool );

    thermistor_value_dC = HeatingHotTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempHot );
}

TEST ( Temperature_Handler, TemperatureRangeWarmBoundaryTesting )
{
    thermistor_value_dC = HeatingWarmTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value_dC = ( CoolingCoolTherm_dC + 10 );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value_dC = CoolingCoolTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempCool );

    thermistor_value_dC = HeatingWarmTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value_dC = ( HeatingHotTherm_dC - 10 );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value_dC = HeatingHotTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempHot );
}

TEST ( Temperature_Handler, TemperatureRangeHotBoundaryTesting )
{
    thermistor_value_dC = HeatingHotTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempHot );

    thermistor_value_dC = ( CoolingWarmTherm_dC + 10 );
    TEST_ASSERT ( GetTemperatureRange() == TempHot );

    thermistor_value_dC = CoolingWarmTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value_dC = HeatingHotTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempHot );

    thermistor_value_dC = ( CoolingCoolTherm_dC + 10 );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value_dC = HeatingHotTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempHot );

    thermistor_value_dC = CoolingCoolTherm_dC;
    TEST_ASSERT ( GetTemperatureRange() == TempCool );
}

TEST ( Temperature_Handler, CoolToWarmPrintout )
{

    thermistor_value_dC = HeatingWarmTherm_dC;
    GetTemperatureRange();

    char* expected = "Temp Cool->Warm\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, CoolToHotPrintout )
{

    thermistor_value_dC = HeatingHotTherm_dC;
    GetTemperatureRange();

    char* expected = "Temp Cool->Hot\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, CoolToCoolNoPrintout )
{

    thermistor_value_dC = CoolingCoolTherm_dC;
    GetTemperatureRange();

    char* expected = "";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, WarmToCoolPrintout )
{

    thermistor_value_dC = HeatingWarmTherm_dC;
    GetTemperatureRange();

    thermistor_value_dC = CoolingCoolTherm_dC;
    GetTemperatureRange();

    char* expected = "Temp Warm->Cool\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, WarmToHotPrintout )
{

    thermistor_value_dC = HeatingWarmTherm_dC;
    GetTemperatureRange();

    thermistor_value_dC = HeatingHotTherm_dC;
    GetTemperatureRange();

    char* expected = "Temp Warm->Hot\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, WarmToWarmNoPrintout )
{

    thermistor_value_dC = HeatingWarmTherm_dC;
    GetTemperatureRange();

    thermistor_value_dC = HeatingWarmTherm_dC;
    GetTemperatureRange();
    thermistor_value_dC = CoolingWarmTherm_dC;
    GetTemperatureRange();

    char* expected = "";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, HotToCoolPrintout )
{

    thermistor_value_dC = HeatingHotTherm_dC;
    GetTemperatureRange();

    thermistor_value_dC = CoolingCoolTherm_dC;
    GetTemperatureRange();

    char* expected = "Temp Hot->Cool\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, HotToWarmPrintout )
{

    thermistor_value_dC = HeatingHotTherm_dC;
    GetTemperatureRange();

    thermistor_value_dC = CoolingWarmTherm_dC;
    GetTemperatureRange();

    char* expected = "Temp Hot->Warm\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, HotToHotNoPrintout )
{

    thermistor_value_dC = HeatingHotTherm_dC;
    GetTemperatureRange();

    thermistor_value_dC = HeatingHotTherm_dC;
    GetTemperatureRange();

    char* expected = "";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, ( const uint32_t ) strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, LogTemperature )
{
    char* string = ( char* ) calloc ( TEMPERATURE_LOG_SIZE, sizeof ( char ) );

    thermistor_value_dC = 10;
    int16_t expected = thermistor_value_dC;
    int16_t actual = 0;

    LogTemperature();

    ReadLog ( STARTING_TEMPERATURE_ADDRESS + numTemperatureLogs *
              TEMPERATURE_LOG_SIZE, string,
              TEMPERATURE_LOG_SIZE );

    numFromCharArray ( string, actual );
    TEST_ASSERT_EQUAL_INT16 ( expected, actual );

    numTemperatureLogs++;

    if ( numTemperatureLogs >= TOTAL_TEMPERATURE_LOGS )
    {
        numTemperatureLogs = 0;
    }

    free ( string );
}

TEST ( Temperature_Handler, findTemperatureLogEOL )
{
    char* string = ( char* ) calloc ( TEMPERATURE_LOG_SIZE, sizeof ( char ) );
    int16_t int_str = 0;

    // Log Temperature
    LogTemperature();

    numTemperatureLogs++;

    if ( numTemperatureLogs >= TOTAL_TEMPERATURE_LOGS )
    {
        numTemperatureLogs = 0;
    }


    // Read EOL
    ReadLog ( STARTING_TEMPERATURE_ADDRESS + numTemperatureLogs *
              TEMPERATURE_LOG_SIZE, string,
              TEMPERATURE_LOG_SIZE );

    // Verify it is EOL
    numFromCharArray ( string, int_str );

    TEST_ASSERT_EQUAL_INT16 ( eol_int_const, int_str );

    // FindEOL Function
    findTemperatureLogEOL();

    // Write Log
    LogTemperature();

    numTemperatureLogs++;

    if ( numTemperatureLogs >= TOTAL_TEMPERATURE_LOGS )
    {
        numTemperatureLogs = 0;
    }

    // Read EOL at next
    ReadLog ( STARTING_TEMPERATURE_ADDRESS + numTemperatureLogs *
              TEMPERATURE_LOG_SIZE, string,
              TEMPERATURE_LOG_SIZE );

    // Verify it is EOL
    numFromCharArray ( string, int_str );

    TEST_ASSERT_EQUAL_INT16 ( eol_int_const, int_str );

}

TEST ( Temperature_Handler, LogTemperatureAgain )
{
    char* string = ( char* ) calloc ( TEMPERATURE_LOG_SIZE, sizeof ( char ) );

    thermistor_value_dC = 1;
    int16_t expected = thermistor_value_dC;
    int16_t actual = 0;

    LogTemperature();

    ReadLog ( STARTING_TEMPERATURE_ADDRESS + numTemperatureLogs *
              TEMPERATURE_LOG_SIZE, string,
              TEMPERATURE_LOG_SIZE );

    numFromCharArray ( string, actual );
    TEST_ASSERT_EQUAL_INT16 ( expected, actual );

    numTemperatureLogs++;

    if ( numTemperatureLogs >= TOTAL_TEMPERATURE_LOGS )
    {
        numTemperatureLogs = 0;
    }

    free ( string );
}

TEST ( Temperature_Handler, LogFiftyTemperatures )
{
    char* string = ( char* ) calloc ( TEMPERATURE_LOG_SIZE, sizeof ( char ) );

    const uint8_t logs_to_write = 50;

    for ( uint8_t i = 0; i < logs_to_write; i++ )
    {
        thermistor_value_dC = i;
        LogTemperature();
    }

    for ( uint16_t i = 0; i < logs_to_write; i++ )
    {
        int16_t expected = i;
        int16_t actual = 0;
        ReadLog ( STARTING_TEMPERATURE_ADDRESS + numTemperatureLogs *
                  TEMPERATURE_LOG_SIZE, string,
                  TEMPERATURE_LOG_SIZE );

        numFromCharArray ( string, actual );
        TEST_ASSERT_EQUAL_INT16 ( expected, actual );

        numTemperatureLogs++;

        if ( numTemperatureLogs >= TOTAL_TEMPERATURE_LOGS )
        {
            numTemperatureLogs = 0;
        }
    }

    free ( string );
}

TEST ( Temperature_Handler, ReadWholeLog )
{
    // This just ensures that we don't crash if we write the entire thing

    char* string = ( char* ) calloc ( TEMPERATURE_LOG_SIZE, sizeof ( char ) );

    const uint8_t logs_to_read = TOTAL_TEMPERATURE_LOGS;

    for ( uint8_t i = 0; i < logs_to_read; i++ )
    {
        ReadLog ( STARTING_TEMPERATURE_ADDRESS + numTemperatureLogs *
                  TEMPERATURE_LOG_SIZE, string,
                  TEMPERATURE_LOG_SIZE );
        numTemperatureLogs++;

        if ( numTemperatureLogs >= TOTAL_TEMPERATURE_LOGS )
        {
            numTemperatureLogs = 0;
        }
    }

    free ( string );
}

TEST ( Temperature_Handler, WriteLogOverflow )
{
    char* string = ( char* ) calloc ( TEMPERATURE_LOG_SIZE, sizeof ( char ) );

    // Fill Log Up
    while ( numTemperatureLogs < TOTAL_TEMPERATURE_LOGS )
    {
        LogTemperature();
        numTemperatureLogs++;
    }

    numTemperatureLogs = 0;

    // Write One More
    thermistor_value_dC = 27;
    int16_t expected = thermistor_value_dC;
    int16_t actual = 0;

    LogTemperature();

    // Make sure that last log was written at STARTING_TEMPERATURE_ADDRESS
    ReadLog ( STARTING_TEMPERATURE_ADDRESS, string,
              TEMPERATURE_LOG_SIZE );

    numFromCharArray ( string, actual );
    TEST_ASSERT_EQUAL_INT16 ( expected, actual );

    numTemperatureLogs++;

    if ( numTemperatureLogs > TOTAL_TEMPERATURE_LOGS )
    {
        numTemperatureLogs = 0;
    }

    free ( string );

}

TEST ( Temperature_Handler, findTemperatureLogEOLLast )
{
    char* string = ( char* ) calloc ( TEMPERATURE_LOG_SIZE, sizeof ( char ) );
    int16_t int_str = 0;

    // Fill Log Up Minus Last Spot
    while ( numTemperatureLogs < TOTAL_TEMPERATURE_LOGS - 1 )
    {
        LogTemperature();
        numTemperatureLogs++;
    }

    const uint32_t last_entry = STARTING_TEMPERATURE_ADDRESS + TEMPERATURE_LOG_SPACE
                                - TEMPERATURE_LOG_SIZE;

    // Read EOL
    ReadLog ( last_entry, string,
              TEMPERATURE_LOG_SIZE );

    // Verify it is EOL
    numFromCharArray ( string, int_str );

    TEST_ASSERT_EQUAL_INT16 ( eol_int_const, int_str );

    // FindEOL Function
    findTemperatureLogEOL();

    // Write Log
    LogTemperature();

    // EOL should now be at beginning
    numTemperatureLogs = 0;

    // Read EOL at next
    ReadLog ( STARTING_TEMPERATURE_ADDRESS + numTemperatureLogs *
              TEMPERATURE_LOG_SIZE, string,
              TEMPERATURE_LOG_SIZE );

    // Verify it is EOL
    numFromCharArray ( string, int_str );

    TEST_ASSERT_EQUAL_INT16 ( eol_int_const, int_str );

}

TEST ( Temperature_Handler, findTemperatureLogEOLFirst )
{
    char* string = ( char* ) calloc ( TEMPERATURE_LOG_SIZE, sizeof ( char ) );
    int16_t int_str = 0;

    // Fill Log Up Minus Last Spot
    while ( numTemperatureLogs < TOTAL_TEMPERATURE_LOGS )
    {
        LogTemperature();
        numTemperatureLogs++;
    }

    // EOL should now be at beginning
    numTemperatureLogs = 0;

    // Read EOL
    ReadLog ( STARTING_TEMPERATURE_ADDRESS, string,
              TEMPERATURE_LOG_SIZE );

    // Verify it is EOL
    numFromCharArray ( string, int_str );

    TEST_ASSERT_EQUAL_INT16 ( eol_int_const, int_str );

    // FindEOL Function
    findTemperatureLogEOL();

    // Write Log
    LogTemperature();

    numTemperatureLogs++;

    if ( numTemperatureLogs > TOTAL_TEMPERATURE_LOGS )
    {
        numTemperatureLogs = 0;
    }

    // Read EOL at next
    ReadLog ( STARTING_TEMPERATURE_ADDRESS + numTemperatureLogs *
              TEMPERATURE_LOG_SIZE, string,
              TEMPERATURE_LOG_SIZE );

    // Verify it is EOL
    numFromCharArray ( string, int_str );

    TEST_ASSERT_EQUAL_INT16 ( eol_int_const, int_str );
}

/* end temperature_handler tests */


TEST_GROUP_RUNNER ( Temperature_Handler )
{
    RUN_TEST_CASE ( Temperature_Handler, GetDefaultTemperature );
    RUN_TEST_CASE ( Temperature_Handler, GetDefaultTemperatureRange );
    RUN_TEST_CASE ( Temperature_Handler, GetTemperature );
    RUN_TEST_CASE ( Temperature_Handler, TemperatureRangeNormalStepping );
    RUN_TEST_CASE ( Temperature_Handler, TemperatureRangeCoolTesting );
    RUN_TEST_CASE ( Temperature_Handler, TemperatureRangeWarmTesting );
    RUN_TEST_CASE ( Temperature_Handler, TemperatureRangeHotTesting );
    RUN_TEST_CASE ( Temperature_Handler, TemperatureRangeCoolBoundaryTesting );
    RUN_TEST_CASE ( Temperature_Handler, TemperatureRangeWarmBoundaryTesting );
    RUN_TEST_CASE ( Temperature_Handler, TemperatureRangeHotBoundaryTesting );
    RUN_TEST_CASE ( Temperature_Handler, CoolToWarmPrintout );
    RUN_TEST_CASE ( Temperature_Handler, CoolToHotPrintout );
    RUN_TEST_CASE ( Temperature_Handler, CoolToCoolNoPrintout );
    RUN_TEST_CASE ( Temperature_Handler, WarmToCoolPrintout );
    RUN_TEST_CASE ( Temperature_Handler, WarmToHotPrintout );
    RUN_TEST_CASE ( Temperature_Handler, WarmToWarmNoPrintout );
    RUN_TEST_CASE ( Temperature_Handler, HotToCoolPrintout );
    RUN_TEST_CASE ( Temperature_Handler, HotToWarmPrintout );
    RUN_TEST_CASE ( Temperature_Handler, HotToHotNoPrintout );
    RUN_TEST_CASE ( Temperature_Handler, LogTemperature );
    RUN_TEST_CASE ( Temperature_Handler, findTemperatureLogEOL );
    RUN_TEST_CASE ( Temperature_Handler, LogTemperatureAgain );
    RUN_TEST_CASE ( Temperature_Handler, LogFiftyTemperatures );
    RUN_TEST_CASE ( Temperature_Handler, ReadWholeLog );
    RUN_TEST_CASE ( Temperature_Handler, WriteLogOverflow );
    RUN_TEST_CASE ( Temperature_Handler, findTemperatureLogEOLLast );
    RUN_TEST_CASE ( Temperature_Handler, findTemperatureLogEOLFirst );
}
