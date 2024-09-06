#include <string.h>

#include "unity_fixture.h"      /* UNITY */
#include "current_handler.h"    /* CUT */
#include "logger.h"             /* CUT */
#include "stm32l412xx-bsp.h"    /* CUT */

const uint16_t CurrentNormalValue   = 30u;
const uint16_t CurrentHighValue     = 35u;
const uint16_t CurrentErrorValue    = 40u;

extern void initFram( void );

extern uint32_t address; // last written to address
extern uint16_t current_value_dA;

TEST_GROUP(Current_Handler);

TEST_SETUP(Current_Handler)
{
    /* executed before *every* non-skipped test */
    initFram();
}

TEST_TEAR_DOWN(Current_Handler)
{
    /* executed after *every* non-skipped and non-failing test */
    current_value_dA = CurrentNormalValue; // return to default value
    GetCurrentRange(); // set current range
}

/* start Current_Handler tests */
TEST(Current_Handler, GetDefaultCurrent)
{
    TEST_ASSERT_EQUAL_UINT16(0u, GetCurrent()); // Default value
}
TEST(Current_Handler, GetDefaultCurrentRange)
{
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());
}
TEST(Current_Handler, GetCurrent)
{
    current_value_dA = 10u;
    TEST_ASSERT_EQUAL_UINT16(10u, GetCurrent());
    current_value_dA = 25u;
    TEST_ASSERT_EQUAL_UINT16(25u, GetCurrent());

    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_UINT16(CurrentHighValue, GetCurrent());

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_UINT16(CurrentErrorValue, GetCurrent());
}
TEST(Current_Handler, CurrentRangeNormalStepping)
{
    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());

    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_dA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());
}
TEST(Current_Handler, CurrentRangeNormalTesting)
{
    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_dA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());
}
TEST(Current_Handler, CurrentRangeHighTesting)
{
    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_dA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());

    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());

    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());
}
TEST(Current_Handler, CurrentRangeErrorTesting)
{
    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());

    current_value_dA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());

    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());
}
TEST(Current_Handler, CurrentRangeNormalBoundaryTesting)
{
    current_value_dA = (CurrentHighValue - 1);
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());

    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_dA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());

    current_value_dA = (CurrentErrorValue - 1);
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_dA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());
}
TEST(Current_Handler, CurrentRangeHighBoundaryTesting)
{
    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_dA = (CurrentHighValue - 1);
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());

    current_value_dA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_dA = (CurrentErrorValue - 1);
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());
}
TEST(Current_Handler, CurrentRangeErrorBoundaryTesting)
{
    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());

    current_value_dA = (CurrentErrorValue - 1);
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());

    current_value_dA = (CurrentHighValue - 1);
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());

    current_value_dA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());
}

TEST(Current_Handler, NormalToHighPrintout)
{

    current_value_dA = CurrentHighValue;
    GetCurrentRange();

    char* expected = "High Current 35 dA\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}

TEST(Current_Handler, NormalToErrorPrintout)
{

    current_value_dA = CurrentErrorValue;
    GetCurrentRange();

    char* expected = "Error Current 40 dA\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}

TEST(Current_Handler, NormalToNormalNoPrintout)
{

    current_value_dA = CurrentNormalValue;
    GetCurrentRange();

    char* expected = "";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}

TEST(Current_Handler, HighToNormalPrintout)
{

    current_value_dA = CurrentHighValue;
    GetCurrentRange();

    current_value_dA = CurrentNormalValue;
    GetCurrentRange();

    char* expected = "Normal Current 30 dA\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}

TEST(Current_Handler, HighToErrorPrintout)
{

    current_value_dA = CurrentHighValue;
    GetCurrentRange();

    current_value_dA = CurrentErrorValue;
    GetCurrentRange();

    char* expected = "Error Current 40 dA\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}

TEST(Current_Handler, HighToHighNoPrintout)
{

    current_value_dA = CurrentHighValue;
    GetCurrentRange();

    current_value_dA = CurrentHighValue;
    GetCurrentRange();
    current_value_dA = CurrentHighValue;
    GetCurrentRange();

    char* expected = "";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}

TEST(Current_Handler, ErrorToNormalPrintout)
{

    current_value_dA = CurrentErrorValue;
    GetCurrentRange();

    current_value_dA = CurrentNormalValue;
    GetCurrentRange();

    char* expected = "Normal Current 30 dA\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}

TEST(Current_Handler, ErrorToHighPrintout)
{

    current_value_dA = CurrentErrorValue;
    GetCurrentRange();

    current_value_dA = CurrentHighValue;
    GetCurrentRange();

    char* expected = "High Current 35 dA\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}

TEST(Current_Handler, ErrorToErrorNoPrintout)
{

    current_value_dA = CurrentErrorValue;
    GetCurrentRange();

    current_value_dA = CurrentErrorValue;
    GetCurrentRange();

    char* expected = "";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(address, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    free(string);
}
/* end Current_Handler tests */


TEST_GROUP_RUNNER(Current_Handler)
{
    RUN_TEST_CASE(Current_Handler, GetDefaultCurrent);
    RUN_TEST_CASE(Current_Handler, GetDefaultCurrentRange);
    RUN_TEST_CASE(Current_Handler, GetCurrent);
    RUN_TEST_CASE(Current_Handler, CurrentRangeNormalStepping);
    RUN_TEST_CASE(Current_Handler, CurrentRangeNormalTesting);
    RUN_TEST_CASE(Current_Handler, CurrentRangeHighTesting);
    RUN_TEST_CASE(Current_Handler, CurrentRangeErrorTesting);
    RUN_TEST_CASE(Current_Handler, CurrentRangeNormalBoundaryTesting);
    RUN_TEST_CASE(Current_Handler, CurrentRangeHighBoundaryTesting);
    RUN_TEST_CASE(Current_Handler, CurrentRangeErrorBoundaryTesting);
    RUN_TEST_CASE(Current_Handler, NormalToHighPrintout);
    RUN_TEST_CASE(Current_Handler, NormalToErrorPrintout);
    RUN_TEST_CASE(Current_Handler, NormalToNormalNoPrintout);
    RUN_TEST_CASE(Current_Handler, HighToNormalPrintout);
    RUN_TEST_CASE(Current_Handler, HighToErrorPrintout);
    RUN_TEST_CASE(Current_Handler, HighToHighNoPrintout);
    RUN_TEST_CASE(Current_Handler, ErrorToNormalPrintout);
    RUN_TEST_CASE(Current_Handler, ErrorToHighPrintout);
    RUN_TEST_CASE(Current_Handler, ErrorToErrorNoPrintout);
}
