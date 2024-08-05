#include <string.h>

#include "unity_fixture.h"      /* UNITY */
#include "current_handler.h"    /* CUT */
#include "logger.h"             /* CUT */
#include "bsp.h"                /* CUT */

const uint16_t CurrentNormalValue   = 3000u;
const uint16_t CurrentHighValue     = 3500u;
const uint16_t CurrentErrorValue    = 4000u;

extern uint16_t current_value_mA;

TEST_GROUP(Current_Handler);

TEST_SETUP(Current_Handler)
{
    /* executed before *every* non-skipped test */
}

TEST_TEAR_DOWN(Current_Handler)
{
    /* executed after *every* non-skipped and non-failing test */
    current_value_mA = CurrentNormalValue; // return to default value
    GetCurrentRange(); // set current range
}

/* start Current_Handler tests */
TEST(Current_Handler, GetDefaultCurrent)
{
    TEST_ASSERT_EQUAL_UINT16(0u, GetCurrent_mA()); // Default value
}
TEST(Current_Handler, GetDefaultCurrentRange)
{
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());
}
TEST(Current_Handler, GetCurrent)
{
    current_value_mA = 1000u;
    TEST_ASSERT_EQUAL_UINT16(1000u, GetCurrent_mA());
    current_value_mA = 2523u;
    TEST_ASSERT_EQUAL_UINT16(2523u, GetCurrent_mA());

    current_value_mA = CurrentHighValue;
    TEST_ASSERT_EQUAL_UINT16(CurrentHighValue, GetCurrent_mA());

    current_value_mA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_UINT16(CurrentErrorValue, GetCurrent_mA());
}
TEST(Current_Handler, CurrentRangeNormalStepping)
{
    current_value_mA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_mA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());

    current_value_mA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_mA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());
}
TEST(Current_Handler, CurrentRangeNormalTesting)
{
    current_value_mA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_mA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());

    current_value_mA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());
}
TEST(Current_Handler, CurrentRangeHighTesting)
{
    current_value_mA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_mA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());

    current_value_mA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_mA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());

    current_value_mA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());
}
TEST(Current_Handler, CurrentRangeErrorTesting)
{
    current_value_mA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());

    current_value_mA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());

    current_value_mA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());

    current_value_mA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());
}
TEST(Current_Handler, CurrentRangeNormalBoundaryTesting)
{
    current_value_mA = (CurrentHighValue - 1);
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());

    current_value_mA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_mA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());

    current_value_mA = (CurrentErrorValue - 1);
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_mA = CurrentNormalValue;
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());

    current_value_mA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());
}
TEST(Current_Handler, CurrentRangeHighBoundaryTesting)
{
    current_value_mA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_mA = (CurrentHighValue - 1);
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());

    current_value_mA = CurrentHighValue;
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_mA = (CurrentErrorValue - 1);
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_mA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());
}
TEST(Current_Handler, CurrentRangeErrorBoundaryTesting)
{
    current_value_mA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());

    current_value_mA = (CurrentErrorValue - 1);
    TEST_ASSERT_EQUAL_INT(CurrentHigh, GetCurrentRange());

    current_value_mA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());

    current_value_mA = (CurrentHighValue - 1);
    TEST_ASSERT_EQUAL_INT(CurrentNormal, GetCurrentRange());

    current_value_mA = CurrentErrorValue;
    TEST_ASSERT_EQUAL_INT(CurrentError, GetCurrentRange());
}

TEST(Current_Handler, NormalToHighPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    current_value_mA = CurrentHighValue;
    GetCurrentRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "High Current 3500 mA\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(0, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Current_Handler, NormalToErrorPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    current_value_mA = CurrentErrorValue;
    GetCurrentRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Error Current 4000 mA\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(0, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Current_Handler, NormalToNormalNoPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    current_value_mA = CurrentNormalValue;
    GetCurrentRange();

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

TEST(Current_Handler, HighToNormalPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    current_value_mA = CurrentHighValue;
    GetCurrentRange();

    current_value_mA = CurrentNormalValue;
    GetCurrentRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Normal Current 3000 mA\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("High Current 3500 mA\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Current_Handler, HighToErrorPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    current_value_mA = CurrentHighValue;
    GetCurrentRange();

    current_value_mA = CurrentErrorValue;
    GetCurrentRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Error Current 4000 mA\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("High Current 3500 mA\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Current_Handler, HighToHighNoPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    current_value_mA = CurrentHighValue;
    GetCurrentRange();

    current_value_mA = CurrentHighValue;
    GetCurrentRange();
    current_value_mA = CurrentHighValue;
    GetCurrentRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("High Current 3500 mA\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Current_Handler, ErrorToNormalPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    current_value_mA = CurrentErrorValue;
    GetCurrentRange();

    current_value_mA = CurrentNormalValue;
    GetCurrentRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Normal Current 3000 mA\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Error Current 4000 mA\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Current_Handler, ErrorToHighPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    current_value_mA = CurrentErrorValue;
    GetCurrentRange();

    current_value_mA = CurrentHighValue;
    GetCurrentRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "High Current 3500 mA\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Error Current 4000 mA\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Current_Handler, ErrorToErrorNoPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    current_value_mA = CurrentErrorValue;
    GetCurrentRange();

    current_value_mA = CurrentErrorValue;
    GetCurrentRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Error Current 4000 mA\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
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