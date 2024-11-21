#include <string.h>

#include "unity_fixture.h"          /* UNITY */
#include "temperature_handler.h"    /* CUT */
#include "logger.h"                 /* CUT */
#include "stm32l412xx-bsp.h"        /* CUT */

const uint16_t HeatingWarmTherm_dC = 1000u;
const uint16_t HeatingHotTherm_dC = 1200u;
const uint16_t CoolingCoolTherm_dC = 800u;
const uint16_t CoolingWarmTherm_dC = 1000u;

extern void initFram ( void );

extern uint32_t address; // last written to address
extern uint16_t thermistor_value;
extern const float dC_to_thermistor;

TEST_GROUP ( Temperature_Handler );

TEST_SETUP ( Temperature_Handler )
{
    /* executed before *every* non-skipped test */
    initFram();
}

TEST_TEAR_DOWN ( Temperature_Handler )
{
    /* executed after *every* non-skipped and non-failing test */
    thermistor_value = ( uint16_t ) ( 500 * dC_to_thermistor +
                                      0.5f ); // return to default value
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
    TEST_ASSERT_EQUAL_INT32 (-500, GetTemperature() ); // Default value
}

TEST ( Temperature_Handler, GetDefaultTemperatureRange )
{
    TEST_ASSERT ( GetTemperatureRange() == TempCool );
}

TEST ( Temperature_Handler, GetTemperature )
{
    thermistor_value = ( uint16_t ) ( 700 * dC_to_thermistor + 0.5f );
    TEST_ASSERT_EQUAL_INT32 ( 200, GetTemperature() );

    thermistor_value = ( uint16_t ) ( ( HeatingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT_EQUAL_INT32 ( HeatingWarmTherm_dC, GetTemperature() );

    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT_EQUAL_INT32 ( HeatingHotTherm_dC, GetTemperature() );

    thermistor_value = ( uint16_t ) ( ( CoolingCoolTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT_EQUAL_INT32 ( CoolingCoolTherm_dC, GetTemperature() );

    thermistor_value = ( uint16_t ) ( ( CoolingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT_EQUAL_INT32 ( CoolingWarmTherm_dC, GetTemperature() );

    thermistor_value = ( uint16_t ) ( 1500 * dC_to_thermistor + 0.5f );
    TEST_ASSERT_EQUAL_INT32 ( 1000, GetTemperature() );

    thermistor_value = ( uint16_t ) ( 700 * dC_to_thermistor + 0.5f );
    TEST_ASSERT_EQUAL_INT32 ( 200, GetTemperature() );

    thermistor_value = ( uint16_t ) ( 1000 * dC_to_thermistor + 0.5f );
    TEST_ASSERT_EQUAL_INT32 ( 500, GetTemperature() );

    thermistor_value = ( uint16_t ) ( 1200 * dC_to_thermistor + 0.5f );
    TEST_ASSERT_EQUAL_INT32 ( 700, GetTemperature() );

    thermistor_value = ( uint16_t ) ( 1500 * dC_to_thermistor + 0.5f );
    TEST_ASSERT_EQUAL_INT32 ( 1000, GetTemperature() );
}

TEST ( Temperature_Handler, TemperatureRangeNormalStepping )
{
    thermistor_value = ( uint16_t ) ( ( HeatingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempHot );

    thermistor_value = ( uint16_t ) ( ( CoolingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value = ( uint16_t ) ( ( CoolingCoolTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempCool );
}

TEST ( Temperature_Handler, TemperatureRangeCoolTesting )
{
    thermistor_value = ( uint16_t ) ( ( HeatingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value = ( uint16_t ) ( ( CoolingCoolTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempCool );

    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempHot );
}

TEST ( Temperature_Handler, TemperatureRangeWarmTesting )
{
    thermistor_value = ( uint16_t ) ( ( HeatingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value = ( uint16_t ) ( ( CoolingCoolTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempCool );

    thermistor_value = ( uint16_t ) ( ( HeatingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempHot );

    thermistor_value = ( uint16_t ) ( ( CoolingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );
}

TEST ( Temperature_Handler, TemperatureRangeHotTesting )
{
    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempHot );

    thermistor_value = ( uint16_t ) ( ( CoolingCoolTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempCool );

    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempHot );

    thermistor_value = ( uint16_t ) ( ( CoolingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );
}

TEST ( Temperature_Handler, TemperatureRangeCoolBoundaryTesting )
{
    thermistor_value = ( ( uint16_t ) ( ( HeatingWarmTherm_dC + 500 ) *
                                        dC_to_thermistor + 0.5f ) - 10 );
    TEST_ASSERT ( GetTemperatureRange() == TempCool );

    thermistor_value = ( uint16_t ) ( ( HeatingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value = ( uint16_t ) ( ( CoolingCoolTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempCool );

    thermistor_value = ( ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                        dC_to_thermistor + 0.5f ) - 10 );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value = ( uint16_t ) ( ( CoolingCoolTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempCool );

    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempHot );
}

TEST ( Temperature_Handler, TemperatureRangeWarmBoundaryTesting )
{
    thermistor_value = ( uint16_t ) ( ( HeatingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value = ( ( uint16_t ) ( ( CoolingCoolTherm_dC + 500 ) *
                                        dC_to_thermistor + 0.5f ) + 10 );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value = ( uint16_t ) ( ( CoolingCoolTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempCool );

    thermistor_value = ( uint16_t ) ( ( HeatingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value = ( ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                        dC_to_thermistor + 0.5f ) - 10 );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempHot );
}

TEST ( Temperature_Handler, TemperatureRangeHotBoundaryTesting )
{
    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempHot );

    thermistor_value = ( ( uint16_t ) ( ( CoolingWarmTherm_dC + 500 ) *
                                        dC_to_thermistor + 0.5f ) + 10 );
    TEST_ASSERT ( GetTemperatureRange() == TempHot );

    thermistor_value = ( uint16_t ) ( ( CoolingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempHot );

    thermistor_value = ( ( uint16_t ) ( ( CoolingCoolTherm_dC + 500 ) *
                                        dC_to_thermistor + 0.5f ) + 10 );
    TEST_ASSERT ( GetTemperatureRange() == TempWarm );

    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempHot );

    thermistor_value = ( uint16_t ) ( ( CoolingCoolTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    TEST_ASSERT ( GetTemperatureRange() == TempCool );
}

TEST ( Temperature_Handler, CoolToWarmPrintout )
{

    thermistor_value = ( uint16_t ) ( ( HeatingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    GetTemperatureRange();

    char* expected = "Temp Cool->Warm\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, CoolToHotPrintout )
{

    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    GetTemperatureRange();

    char* expected = "Temp Cool->Hot\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, CoolToCoolNoPrintout )
{

    thermistor_value = ( uint16_t ) ( ( CoolingCoolTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    GetTemperatureRange();

    char* expected = "";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, WarmToCoolPrintout )
{

    thermistor_value = ( uint16_t ) ( ( HeatingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    GetTemperatureRange();

    thermistor_value = ( uint16_t ) ( ( CoolingCoolTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    GetTemperatureRange();

    char* expected = "Temp Warm->Cool\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, WarmToHotPrintout )
{

    thermistor_value = ( uint16_t ) ( ( HeatingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    GetTemperatureRange();

    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    GetTemperatureRange();

    char* expected = "Temp Warm->Hot\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, WarmToWarmNoPrintout )
{

    thermistor_value = ( uint16_t ) ( ( HeatingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    GetTemperatureRange();

    thermistor_value = ( uint16_t ) ( ( HeatingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    GetTemperatureRange();
    thermistor_value = ( uint16_t ) ( ( CoolingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    GetTemperatureRange();

    char* expected = "";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, HotToCoolPrintout )
{

    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    GetTemperatureRange();

    thermistor_value = ( uint16_t ) ( ( CoolingCoolTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    GetTemperatureRange();

    char* expected = "Temp Hot->Cool\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, HotToWarmPrintout )
{

    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    GetTemperatureRange();

    thermistor_value = ( uint16_t ) ( ( CoolingWarmTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    GetTemperatureRange();

    char* expected = "Temp Hot->Warm\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Temperature_Handler, HotToHotNoPrintout )
{

    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    GetTemperatureRange();

    thermistor_value = ( uint16_t ) ( ( HeatingHotTherm_dC + 500 ) *
                                      dC_to_thermistor + 0.5f );
    GetTemperatureRange();

    char* expected = "";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
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
}
