#include <string.h>

#include "unity_fixture.h"          /* UNITY */
#include "temperature_handler.h"    /* CUT */
#include "logger.h"                 /* CUT */
#include "stm32l412xx-bsp.h"        /* CUT */

const uint16_t HeatingWarmTherm = 1000u;
const uint16_t HeatingHotTherm  = 1200u;
const uint16_t CoolingCoolTherm = 800u;
const uint16_t CoolingWarmTherm = 1000u;

extern void initFram( void );

extern uint32_t address; // last written to address
extern int32_t thermistor_value_dC;

TEST_GROUP(Temperature_Handler);

TEST_SETUP(Temperature_Handler)
{
    /* executed before *every* non-skipped test */
    initFram();
}

TEST_TEAR_DOWN(Temperature_Handler)
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
TEST(Temperature_Handler, GetDefaultTemperature)
{
    TEST_ASSERT_EQUAL_INT32(0, GetTemperature()); // Default value
}
TEST(Temperature_Handler, GetDefaultTemperatureRange)
{
    TEST_ASSERT(GetTemperatureRange() == TempCool);
}
TEST(Temperature_Handler, GetTemperature)
{
    thermistor_value_dC = 700;
    TEST_ASSERT_EQUAL_INT32(700, GetTemperature());

    thermistor_value_dC = HeatingWarmTherm;
    TEST_ASSERT_EQUAL_INT32(HeatingWarmTherm, GetTemperature());

    thermistor_value_dC = HeatingHotTherm;
    TEST_ASSERT_EQUAL_INT32(HeatingHotTherm, GetTemperature());

    thermistor_value_dC = CoolingCoolTherm;
    TEST_ASSERT_EQUAL_INT32(CoolingCoolTherm, GetTemperature());

    thermistor_value_dC = CoolingWarmTherm;
    TEST_ASSERT_EQUAL_INT32(CoolingWarmTherm, GetTemperature());

    thermistor_value_dC = 1500;
    TEST_ASSERT_EQUAL_INT32(1500, GetTemperature());

    thermistor_value_dC = -700;
    TEST_ASSERT_EQUAL_INT32(-700, GetTemperature());

    thermistor_value_dC = -1000;
    TEST_ASSERT_EQUAL_INT32(-1000, GetTemperature());

    thermistor_value_dC = -1200;
    TEST_ASSERT_EQUAL_INT32(-1200, GetTemperature());

    thermistor_value_dC = -1500;
    TEST_ASSERT_EQUAL_INT32(-1500, GetTemperature());
}
TEST(Temperature_Handler, TemperatureRangeNormalStepping)
{
    thermistor_value_dC = HeatingWarmTherm;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value_dC = HeatingHotTherm;
    TEST_ASSERT(GetTemperatureRange() == TempHot);

    thermistor_value_dC = CoolingWarmTherm;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value_dC = CoolingCoolTherm;
    TEST_ASSERT(GetTemperatureRange() == TempCool);
    }
    TEST(Temperature_Handler, TemperatureRangeCoolTesting)
    {
    thermistor_value_dC = HeatingWarmTherm;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value_dC = CoolingCoolTherm;
    TEST_ASSERT(GetTemperatureRange() == TempCool);

    thermistor_value_dC = HeatingHotTherm;
    TEST_ASSERT(GetTemperatureRange() == TempHot);
}
TEST(Temperature_Handler, TemperatureRangeWarmTesting)
{
    thermistor_value_dC = HeatingWarmTherm;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value_dC = CoolingCoolTherm;
    TEST_ASSERT(GetTemperatureRange() == TempCool);

    thermistor_value_dC = HeatingWarmTherm;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value_dC = HeatingHotTherm;
    TEST_ASSERT(GetTemperatureRange() == TempHot);

    thermistor_value_dC = CoolingWarmTherm;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);
}
TEST(Temperature_Handler, TemperatureRangeHotTesting)
{
    thermistor_value_dC = HeatingHotTherm;
    TEST_ASSERT(GetTemperatureRange() == TempHot);

    thermistor_value_dC = CoolingCoolTherm;
    TEST_ASSERT(GetTemperatureRange() == TempCool);

    thermistor_value_dC = HeatingHotTherm;
    TEST_ASSERT(GetTemperatureRange() == TempHot);

    thermistor_value_dC = CoolingWarmTherm;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);
}
TEST(Temperature_Handler, TemperatureRangeCoolBoundaryTesting)
{
    thermistor_value_dC = (HeatingWarmTherm - 10);
    TEST_ASSERT(GetTemperatureRange() == TempCool);

    thermistor_value_dC = HeatingWarmTherm;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value_dC = CoolingCoolTherm;
    TEST_ASSERT(GetTemperatureRange() == TempCool);

    thermistor_value_dC = (HeatingHotTherm - 10);
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value_dC = CoolingCoolTherm;
    TEST_ASSERT(GetTemperatureRange() == TempCool);

    thermistor_value_dC = HeatingHotTherm;
    TEST_ASSERT(GetTemperatureRange() == TempHot);
}
TEST(Temperature_Handler, TemperatureRangeWarmBoundaryTesting)
{
    thermistor_value_dC = HeatingWarmTherm;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value_dC = (CoolingCoolTherm + 10);
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value_dC = CoolingCoolTherm;
    TEST_ASSERT(GetTemperatureRange() == TempCool);

    thermistor_value_dC = HeatingWarmTherm;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value_dC = (HeatingHotTherm - 10);
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value_dC = HeatingHotTherm;
    TEST_ASSERT(GetTemperatureRange() == TempHot);
}
TEST(Temperature_Handler, TemperatureRangeHotBoundaryTesting)
{
    thermistor_value_dC = HeatingHotTherm;
    TEST_ASSERT(GetTemperatureRange() == TempHot);

    thermistor_value_dC = (CoolingWarmTherm + 10);
    TEST_ASSERT(GetTemperatureRange() == TempHot);

    thermistor_value_dC = CoolingWarmTherm;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value_dC = HeatingHotTherm;
    TEST_ASSERT(GetTemperatureRange() == TempHot);

    thermistor_value_dC = (CoolingCoolTherm + 10);
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value_dC = HeatingHotTherm;
    TEST_ASSERT(GetTemperatureRange() == TempHot);

    thermistor_value_dC = CoolingCoolTherm;
    TEST_ASSERT(GetTemperatureRange() == TempCool);
}

TEST(Temperature_Handler, CoolToWarmPrintout)
{

    thermistor_value_dC = HeatingWarmTherm;
    GetTemperatureRange();

    char* expected = "Temp Cool->Warm\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}

TEST(Temperature_Handler, CoolToHotPrintout)
{

    thermistor_value_dC = HeatingHotTherm;
    GetTemperatureRange();

    char* expected = "Temp Cool->Hot\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}

TEST(Temperature_Handler, CoolToCoolNoPrintout)
{

    thermistor_value_dC = CoolingCoolTherm;
    GetTemperatureRange();

    char* expected = "";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}

TEST(Temperature_Handler, WarmToCoolPrintout)
{

    thermistor_value_dC = HeatingWarmTherm;
    GetTemperatureRange();

    thermistor_value_dC = CoolingCoolTherm;
    GetTemperatureRange();

    char* expected = "Temp Warm->Cool\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}

TEST(Temperature_Handler, WarmToHotPrintout)
{

    thermistor_value_dC = HeatingWarmTherm;
    GetTemperatureRange();

    thermistor_value_dC = HeatingHotTherm;
    GetTemperatureRange();

    char* expected = "Temp Warm->Hot\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}

TEST(Temperature_Handler, WarmToWarmNoPrintout)
{

    thermistor_value_dC = HeatingWarmTherm;
    GetTemperatureRange();

    thermistor_value_dC = HeatingWarmTherm;
    GetTemperatureRange();
    thermistor_value_dC = CoolingWarmTherm;
    GetTemperatureRange();

    char* expected = "";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}

TEST(Temperature_Handler, HotToCoolPrintout)
{

    thermistor_value_dC = HeatingHotTherm;
    GetTemperatureRange();

    thermistor_value_dC = CoolingCoolTherm;
    GetTemperatureRange();

    char* expected = "Temp Hot->Cool\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}

TEST(Temperature_Handler, HotToWarmPrintout)
{

    thermistor_value_dC = HeatingHotTherm;
    GetTemperatureRange();

    thermistor_value_dC = CoolingWarmTherm;
    GetTemperatureRange();

    char* expected = "Temp Hot->Warm\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}

TEST(Temperature_Handler, HotToHotNoPrintout)
{

    thermistor_value_dC = HeatingHotTherm;
    GetTemperatureRange();

    thermistor_value_dC = HeatingHotTherm;
    GetTemperatureRange();

    char* expected = "";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}
/* end temperature_handler tests */


TEST_GROUP_RUNNER(Temperature_Handler)
{
    RUN_TEST_CASE(Temperature_Handler, GetDefaultTemperature);
    RUN_TEST_CASE(Temperature_Handler, GetDefaultTemperatureRange);
    RUN_TEST_CASE(Temperature_Handler, GetTemperature);
    RUN_TEST_CASE(Temperature_Handler, TemperatureRangeNormalStepping);
    RUN_TEST_CASE(Temperature_Handler, TemperatureRangeCoolTesting);
    RUN_TEST_CASE(Temperature_Handler, TemperatureRangeWarmTesting);
    RUN_TEST_CASE(Temperature_Handler, TemperatureRangeHotTesting);
    RUN_TEST_CASE(Temperature_Handler, TemperatureRangeCoolBoundaryTesting);
    RUN_TEST_CASE(Temperature_Handler, TemperatureRangeWarmBoundaryTesting);
    RUN_TEST_CASE(Temperature_Handler, TemperatureRangeHotBoundaryTesting);
    RUN_TEST_CASE(Temperature_Handler, CoolToWarmPrintout);
    RUN_TEST_CASE(Temperature_Handler, CoolToHotPrintout);
    RUN_TEST_CASE(Temperature_Handler, CoolToCoolNoPrintout);
    RUN_TEST_CASE(Temperature_Handler, WarmToCoolPrintout);
    RUN_TEST_CASE(Temperature_Handler, WarmToHotPrintout);
    RUN_TEST_CASE(Temperature_Handler, WarmToWarmNoPrintout);
    RUN_TEST_CASE(Temperature_Handler, HotToCoolPrintout);
    RUN_TEST_CASE(Temperature_Handler, HotToWarmPrintout);
    RUN_TEST_CASE(Temperature_Handler, HotToHotNoPrintout);
}
