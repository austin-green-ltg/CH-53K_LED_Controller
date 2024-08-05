#include <string.h>

#include "unity_fixture.h"      /* UNITY */
#include "thermistor_handler.h" /* CUT */
#include "logger.h"             /* CUT */
#include "bsp.h"                /* CUT */

#define HEATING_WARM_THERM (100000)
#define HEATING_HOT_THERM  (120000)
#define COOLING_COOL_THERM (80000)
#define COOLING_WARM_THERM (100000)

extern int32_t thermistor_value;

TEST_GROUP(Thermistor_Handler);

TEST_SETUP(Thermistor_Handler)
{
    /* executed before *every* non-skipped test */
}

TEST_TEAR_DOWN(Thermistor_Handler)
{
    /* executed after *every* non-skipped and non-failing test */
    thermistor_value = 0; // return to default value
    GetTemperatureRange(); // set thermistor range
}

/* TESTS */
/*
    * Check get values
    * Check get range
*/

/* start thermistor_handler tests */
TEST(Thermistor_Handler, GetDefaultTemperature)
{
    TEST_ASSERT_EQUAL_INT32(0, GetTemperature()); // Default value
}
TEST(Thermistor_Handler, GetDefaultTemperatureRange)
{
    TEST_ASSERT(GetTemperatureRange() == TempCool);
}
TEST(Thermistor_Handler, GetTemperature)
{
    thermistor_value = 70000;
    TEST_ASSERT_EQUAL_INT32(70000, GetTemperature());

    thermistor_value = HEATING_WARM_THERM;
    TEST_ASSERT_EQUAL_INT32(HEATING_WARM_THERM, GetTemperature());

    thermistor_value = HEATING_HOT_THERM;
    TEST_ASSERT_EQUAL_INT32(HEATING_HOT_THERM, GetTemperature());

    thermistor_value = COOLING_COOL_THERM;
    TEST_ASSERT_EQUAL_INT32(COOLING_COOL_THERM, GetTemperature());

    thermistor_value = COOLING_WARM_THERM;
    TEST_ASSERT_EQUAL_INT32(COOLING_WARM_THERM, GetTemperature());

    thermistor_value = 150000;
    TEST_ASSERT_EQUAL_INT32(150000, GetTemperature());

    thermistor_value = -70000;
    TEST_ASSERT_EQUAL_INT32(-70000, GetTemperature());

    thermistor_value = -100000;
    TEST_ASSERT_EQUAL_INT32(-100000, GetTemperature());

    thermistor_value = -120000;
    TEST_ASSERT_EQUAL_INT32(-120000, GetTemperature());

    thermistor_value = -150000;
    TEST_ASSERT_EQUAL_INT32(-150000, GetTemperature());
}
TEST(Thermistor_Handler, TemperatureRangeNormalStepping)
{
    thermistor_value = HEATING_WARM_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value = HEATING_HOT_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempHot);

    thermistor_value = COOLING_WARM_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value = COOLING_COOL_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempCool);
    }
    TEST(Thermistor_Handler, TemperatureRangeCoolTesting)
    {
    thermistor_value = HEATING_WARM_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value = COOLING_COOL_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempCool);

    thermistor_value = HEATING_HOT_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempHot);
}
TEST(Thermistor_Handler, TemperatureRangeWarmTesting)
{
    thermistor_value = HEATING_WARM_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value = COOLING_COOL_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempCool);

    thermistor_value = HEATING_WARM_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value = HEATING_HOT_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempHot);

    thermistor_value = COOLING_WARM_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);
}
TEST(Thermistor_Handler, TemperatureRangeHotTesting)
{
    thermistor_value = HEATING_HOT_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempHot);

    thermistor_value = COOLING_COOL_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempCool);

    thermistor_value = HEATING_HOT_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempHot);

    thermistor_value = COOLING_WARM_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);
}
TEST(Thermistor_Handler, TemperatureRangeCoolBoundaryTesting)
{
    thermistor_value = (HEATING_WARM_THERM - 1000);
    TEST_ASSERT(GetTemperatureRange() == TempCool);

    thermistor_value = HEATING_WARM_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value = COOLING_COOL_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempCool);

    thermistor_value = (HEATING_HOT_THERM - 1000);
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value = COOLING_COOL_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempCool);

    thermistor_value = HEATING_HOT_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempHot);
}
TEST(Thermistor_Handler, TemperatureRangeWarmBoundaryTesting)
{
    thermistor_value = HEATING_WARM_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value = (COOLING_COOL_THERM + 1000);
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value = COOLING_COOL_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempCool);

    thermistor_value = HEATING_WARM_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value = (HEATING_HOT_THERM - 1000);
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value = HEATING_HOT_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempHot);
}
TEST(Thermistor_Handler, TemperatureRangeHotBoundaryTesting)
{
    thermistor_value = HEATING_HOT_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempHot);

    thermistor_value = (COOLING_WARM_THERM + 1000);
    TEST_ASSERT(GetTemperatureRange() == TempHot);

    thermistor_value = COOLING_WARM_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value = HEATING_HOT_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempHot);

    thermistor_value = (COOLING_COOL_THERM + 1000);
    TEST_ASSERT(GetTemperatureRange() == TempWarm);

    thermistor_value = HEATING_HOT_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempHot);

    thermistor_value = COOLING_COOL_THERM;
    TEST_ASSERT(GetTemperatureRange() == TempCool);
}

TEST(Thermistor_Handler, CoolToWarmPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    thermistor_value = HEATING_WARM_THERM;
    GetTemperatureRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Temp Cool->Warm\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(0, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Thermistor_Handler, CoolToHotPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    thermistor_value = HEATING_HOT_THERM;
    GetTemperatureRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Temp Cool->Hot\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(0, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Thermistor_Handler, CoolToCoolNoPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    thermistor_value = COOLING_COOL_THERM;
    GetTemperatureRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(0, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Thermistor_Handler, WarmToCoolPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    thermistor_value = HEATING_WARM_THERM;
    GetTemperatureRange();

    thermistor_value = COOLING_COOL_THERM;
    GetTemperatureRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Temp Warm->Cool\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Temp Cool->Warm\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Thermistor_Handler, WarmToHotPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    thermistor_value = HEATING_WARM_THERM;
    GetTemperatureRange();

    thermistor_value = HEATING_HOT_THERM;
    GetTemperatureRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Temp Warm->Hot\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Temp Cool->Warm\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Thermistor_Handler, WarmToWarmNoPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    thermistor_value = HEATING_WARM_THERM;
    GetTemperatureRange();

    thermistor_value = HEATING_WARM_THERM;
    GetTemperatureRange();
    thermistor_value = COOLING_WARM_THERM;
    GetTemperatureRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Temp Cool->Warm\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Thermistor_Handler, HotToCoolPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    thermistor_value = HEATING_HOT_THERM;
    GetTemperatureRange();

    thermistor_value = COOLING_COOL_THERM;
    GetTemperatureRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Temp Hot->Cool\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Temp Cool->Hot\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Thermistor_Handler, HotToWarmPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    thermistor_value = HEATING_HOT_THERM;
    GetTemperatureRange();

    thermistor_value = COOLING_WARM_THERM;
    GetTemperatureRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Temp Hot->Warm\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Temp Cool->Hot\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Thermistor_Handler, HotToHotNoPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    thermistor_value = HEATING_HOT_THERM;
    GetTemperatureRange();

    thermistor_value = HEATING_HOT_THERM;
    GetTemperatureRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Temp Cool->Hot\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}
/* end thermistor_handler tests */


TEST_GROUP_RUNNER(Thermistor_Handler)
{
    RUN_TEST_CASE(Thermistor_Handler, GetDefaultTemperature);
    RUN_TEST_CASE(Thermistor_Handler, GetDefaultTemperatureRange);
    RUN_TEST_CASE(Thermistor_Handler, GetTemperature);
    RUN_TEST_CASE(Thermistor_Handler, TemperatureRangeNormalStepping);
    RUN_TEST_CASE(Thermistor_Handler, TemperatureRangeCoolTesting);
    RUN_TEST_CASE(Thermistor_Handler, TemperatureRangeWarmTesting);
    RUN_TEST_CASE(Thermistor_Handler, TemperatureRangeHotTesting);
    RUN_TEST_CASE(Thermistor_Handler, TemperatureRangeCoolBoundaryTesting);
    RUN_TEST_CASE(Thermistor_Handler, TemperatureRangeWarmBoundaryTesting);
    RUN_TEST_CASE(Thermistor_Handler, TemperatureRangeHotBoundaryTesting);
    RUN_TEST_CASE(Thermistor_Handler, CoolToWarmPrintout);
    RUN_TEST_CASE(Thermistor_Handler, CoolToHotPrintout);
    RUN_TEST_CASE(Thermistor_Handler, CoolToCoolNoPrintout);
    RUN_TEST_CASE(Thermistor_Handler, WarmToCoolPrintout);
    RUN_TEST_CASE(Thermistor_Handler, WarmToHotPrintout);
    RUN_TEST_CASE(Thermistor_Handler, WarmToWarmNoPrintout);
    RUN_TEST_CASE(Thermistor_Handler, HotToCoolPrintout);
    RUN_TEST_CASE(Thermistor_Handler, HotToWarmPrintout);
    RUN_TEST_CASE(Thermistor_Handler, HotToHotNoPrintout);
}