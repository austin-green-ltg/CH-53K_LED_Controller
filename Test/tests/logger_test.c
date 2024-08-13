#include <string.h>

#include "unity_fixture.h"      /* UNITY */
#include "logger.h"             /* CUT */
#include "stm32l412xx-bsp.h"    /* CUT */

extern FILE* file_ptr;
const char* const file_name = "testFile.txt";
char* string;

TEST_GROUP(Logger);

TEST_SETUP(Logger)
{
    /* executed before *every* non-skipped test */
    string = (char*)calloc(100 , sizeof(char));
}

TEST_TEAR_DOWN(Logger)
{
    /* executed after *every* non-skipped and non-failing test */
    fclose(file_ptr);   // close file
    remove(file_name);  // delete file
    free(string);
}

/* start Logger tests */
TEST(Logger, WriteRead)
{
    file_ptr = fopen(file_name, "w");

    const char* const expected = "test string";

    LogString(expected, 0);
    fclose(file_ptr);

    file_ptr = fopen(file_name, "r");

    ReadLog(0, string, strlen(expected) );
    TEST_ASSERT_EQUAL_STRING(expected, string);
}

TEST(Logger, WriteSpace)
{
    file_ptr = fopen(file_name, "w");

    const char* const expected = " ";

    LogString(expected, 0);
    fclose(file_ptr);

    file_ptr = fopen(file_name, "r");

    ReadLog(0, string, strlen(expected) );
    TEST_ASSERT_EQUAL_STRING(expected, string);
}

TEST(Logger, WriteEmpty)
{
    file_ptr = fopen(file_name, "w");

    const char* const expected = "";

    LogString(expected, 0);
    fclose(file_ptr);

    file_ptr = fopen(file_name, "r");

    ReadLog(0, string, strlen(expected) );
    TEST_ASSERT_EQUAL_STRING(expected, string);
}

TEST(Logger, WriteMultiple)
{
    file_ptr = fopen(file_name, "w");

    const char* const expected  = "test string";
    const char* const expected2 = "test2";

    LogString(expected, 0);
    LogString(expected2, 0);
    fclose(file_ptr);

    file_ptr = fopen(file_name, "r");

    ReadLog(0, string, strlen(expected) + strlen(expected2) );
    TEST_ASSERT_EQUAL_STRING("test stringtest2", string);
}

TEST(Logger, WriteBegging)
{
    file_ptr = fopen(file_name, "w");

    const char* const expected  = "test string";
    const char* const expected2 = "test2";

    LogString(expected, 0);
    LogString(expected2, 1);
    fclose(file_ptr);

    file_ptr = fopen(file_name, "r");

    ReadLog(0, string, strlen(expected2) );
    TEST_ASSERT_EQUAL_STRING(expected2, string);
}

TEST(Logger, WriteRestart)
{
    file_ptr = fopen(file_name, "w");

    const char* const expected  = "test string";
    const char* const expected2 = "test2";

    LogString(expected, 0);
    LogString(expected2, 1);
    LogString(expected, 0);
    fclose(file_ptr);

    file_ptr = fopen(file_name, "r");

    ReadLog(0, string, strlen(expected) + strlen(expected2) );
    TEST_ASSERT_EQUAL_STRING("test2test string", string);
}

TEST(Logger, WriteNumber)
{
    file_ptr = fopen(file_name, "w");

    const int32_t expected = 1;

    LogNumber(expected, 0);
    fclose(file_ptr);

    file_ptr = fopen(file_name, "r");

    ReadLog(0, string, 1 );
    TEST_ASSERT_EQUAL_STRING("1", string);
}

TEST(Logger, WriteNumberZero)
{
    file_ptr = fopen(file_name, "w");

    const int32_t expected = 0;

    LogNumber(expected, 0);
    fclose(file_ptr);

    file_ptr = fopen(file_name, "r");

    ReadLog(0, string, 1 );
    TEST_ASSERT_EQUAL_STRING("0", string);
}

TEST(Logger, WriteNumberNegative)
{
    file_ptr = fopen(file_name, "w");

    const int32_t expected = -1;

    LogNumber(expected, 0);
    fclose(file_ptr);

    file_ptr = fopen(file_name, "r");

    ReadLog(0, string, 2 );
    TEST_ASSERT_EQUAL_STRING("-1", string);
}

TEST(Logger, WriteNumberMax)
{
    file_ptr = fopen(file_name, "w");

    const int32_t expected = 0x7fffffff;

    LogNumber(expected, 0);
    fclose(file_ptr);

    file_ptr = fopen(file_name, "r");

    ReadLog(0, string, 32 );
    TEST_ASSERT_EQUAL_STRING("2147483647", string);
}

TEST(Logger, WriteNumberMin)
{
    file_ptr = fopen(file_name, "w");

    const int32_t expected = 0x80000000;

    LogNumber(expected, 0);
    fclose(file_ptr);

    file_ptr = fopen(file_name, "r");

    ReadLog(0, string, 33 );
    TEST_ASSERT_EQUAL_STRING("-2147483648", string);
}

TEST(Logger, ReadEmpty)
{
    file_ptr = fopen(file_name, "r");
    const char* const expected = "";

    ReadLog(0, string, 20 );
    TEST_ASSERT_EQUAL_STRING(expected, string);
}

TEST(Logger, ReadLessThanWrite)
{
    file_ptr = fopen(file_name, "w");

    const char* const expected = "test string";

    LogString(expected, 0);
    fclose(file_ptr);

    file_ptr = fopen(file_name, "r");

    ReadLog(0, string, 6 );
    TEST_ASSERT_EQUAL_STRING("test s", string);
}

TEST(Logger, ReadMoreThanWrite)
{
    file_ptr = fopen(file_name, "w");

    const char* const expected = "test string";

    LogString(expected, 0);
    fclose(file_ptr);

    file_ptr = fopen(file_name, "r");

    ReadLog(0, string, strlen(expected) + 10 );
    TEST_ASSERT_EQUAL_STRING(expected, string);
}

TEST(Logger, ReadPosition)
{
    file_ptr = fopen(file_name, "w");

    const char* const expected = "test string";

    LogString(expected, 0);
    fclose(file_ptr);

    file_ptr = fopen(file_name, "r");

    ReadLog(3, string, strlen(expected) );
    TEST_ASSERT_EQUAL_STRING("t string", string);
}

TEST(Logger, ReadPositionPastWrite)
{
    file_ptr = fopen(file_name, "w");

    const char* const expected = "test string";

    LogString(expected, 0);
    fclose(file_ptr);

    file_ptr = fopen(file_name, "r");

    ReadLog(20, string, strlen(expected) );
    TEST_ASSERT_EQUAL_STRING("", string);
}

/* end Logger tests */

TEST_GROUP_RUNNER(Logger)
{
    RUN_TEST_CASE(Logger, WriteRead);
    RUN_TEST_CASE(Logger, WriteSpace);
    RUN_TEST_CASE(Logger, WriteEmpty);
    RUN_TEST_CASE(Logger, WriteMultiple);
    RUN_TEST_CASE(Logger, WriteBegging);
    RUN_TEST_CASE(Logger, WriteNumber);
    RUN_TEST_CASE(Logger, WriteNumberZero);
    RUN_TEST_CASE(Logger, WriteNumberNegative);
    RUN_TEST_CASE(Logger, WriteNumberMax);
    RUN_TEST_CASE(Logger, WriteNumberMin);
    RUN_TEST_CASE(Logger, ReadEmpty);
    RUN_TEST_CASE(Logger, ReadLessThanWrite);
    RUN_TEST_CASE(Logger, ReadMoreThanWrite);
    RUN_TEST_CASE(Logger, ReadPosition);
    RUN_TEST_CASE(Logger, ReadPositionPastWrite);
}