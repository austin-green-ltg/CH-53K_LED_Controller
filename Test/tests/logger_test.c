#include <string.h>

#include "unity_fixture.h"      /* UNITY */
#include "logger.h"             /* CUT */
#include "stm32l412xx-bsp.h"    /* CUT */

extern void initFram ( void );

extern uint32_t address; // last written to address
char* string;

TEST_GROUP ( Logger );

TEST_SETUP ( Logger )
{
    /* executed before *every* non-skipped test */
    string = ( char* ) calloc ( 100, sizeof ( char ) );
    // initFram();
}

TEST_TEAR_DOWN ( Logger )
{
    /* executed after *every* non-skipped and non-failing test */
    free ( string );
}

/* start Logger tests */
TEST ( Logger, WriteRead )
{
    const char* const expected = "test string";

    LogString ( expected, 0 );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );
}

TEST ( Logger, WriteSpace )
{
    const char* const expected = " ";

    LogString ( expected, 0 );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );
}

TEST ( Logger, WriteEmpty )
{
    const char* const expected = "";

    LogString ( expected, 0 );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );
}

TEST ( Logger, WriteMultiple )
{
    const char* const expected = "wm-test string";
    const char* const expected2 = "wm-test2";

    LogString ( expected, 0 );
    LogString ( expected2, 0 );

    // reverse ReadLog address because we wrote two strings
    ReadLog ( address - strlen ( expected ) - 1, string,
              strlen ( expected ) + strlen ( expected2 ) + 2 );
    TEST_ASSERT_EQUAL_UINT8_ARRAY ( "wm-test string\0wm-test2", string,
                                    strlen ( expected ) + strlen ( expected2 ) + 2 );
}

TEST ( Logger, WriteBegging )
{
    const char* const expected = "wb-test string";
    const char* const expected2 = "wb-test2";

    LogString ( expected, 0 );
    LogString ( expected2, 1 );

    ReadLog ( address, string, strlen ( expected2 ) );
    TEST_ASSERT_EQUAL_STRING ( expected2, string );
}

TEST ( Logger, WriteRestart )
{
    const char* const expected = "wr-test string";
    const char* const expected2 = "wr-test2";

    LogString ( expected, 0 );
    LogString ( expected2, 1 );
    LogString ( expected, 0 );

    ReadLog ( address - strlen ( expected2 ) - 1, string,
              strlen ( expected ) + strlen ( expected2 ) + 2 );
    TEST_ASSERT_EQUAL_UINT8_ARRAY ( "wr-test2\0wr-test string", string,
                                    strlen ( expected ) + strlen ( expected2 ) + 2 );
}

TEST ( Logger, WriteNumber )
{
    const int32_t expected = 1;

    LogNumber ( expected, 0 );

    ReadLog ( address, string, 1 );
    TEST_ASSERT_EQUAL_STRING ( "1", string );
}

TEST ( Logger, WriteNumberZero )
{
    const int32_t expected = 0;

    LogNumber ( expected, 0 );

    ReadLog ( address, string, 1 );
    TEST_ASSERT_EQUAL_STRING ( "0", string );
}

TEST ( Logger, WriteNumberNegative )
{
    const int32_t expected = -1;

    LogNumber ( expected, 0 );

    ReadLog ( address, string, 2 );
    TEST_ASSERT_EQUAL_STRING ( "-1", string );
}

TEST ( Logger, WriteNumberMax )
{
    const int32_t expected = 0x7fffffff;

    LogNumber ( expected, 0 );

    ReadLog ( address, string, 32 );
    TEST_ASSERT_EQUAL_STRING ( "2147483647", string );
}

TEST ( Logger, WriteNumberMin )
{
    const int32_t expected = 0x80000000;

    LogNumber ( expected, 0 );

    ReadLog ( address, string, 33 );
    TEST_ASSERT_EQUAL_STRING ( "-2147483648", string );
}

TEST ( Logger, ReadEmpty )
{
    const char* const expected = "";

    initFram();

    ReadLog ( address, string, 20 );
    TEST_ASSERT_EQUAL_STRING ( expected, string );
}

TEST ( Logger, ReadLessThanWrite )
{
    const char* const expected = "test string";

    LogString ( expected, 0 );

    ReadLog ( address, string, 6 );
    TEST_ASSERT_EQUAL_STRING ( "test s", string );
}

TEST ( Logger, ReadMoreThanWrite )
{
    const char* const expected = "test string";

    LogString ( expected, 0 );

    ReadLog ( address, string, strlen ( expected ) + 10 );
    TEST_ASSERT_EQUAL_STRING ( expected, string );
}

TEST ( Logger, ReadPosition )
{
    const char* const expected = "test string";

    LogString ( expected, 0 );

    ReadLog ( address + 3, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( "t string", string );
}

TEST ( Logger, ReadPositionPastWrite )
{
    const char* const expected = "test string";

    LogString ( expected, 0 );

    ReadLog ( address + 20, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( "", string );
}

/* end Logger tests */

TEST_GROUP_RUNNER ( Logger )
{
    RUN_TEST_CASE ( Logger, WriteRead );
    RUN_TEST_CASE ( Logger, WriteSpace );
    RUN_TEST_CASE ( Logger, WriteEmpty );
    RUN_TEST_CASE ( Logger, WriteMultiple );
    RUN_TEST_CASE ( Logger, WriteBegging );
    RUN_TEST_CASE ( Logger, WriteRestart );
    RUN_TEST_CASE ( Logger, WriteNumber );
    RUN_TEST_CASE ( Logger, WriteNumberZero );
    RUN_TEST_CASE ( Logger, WriteNumberNegative );
    RUN_TEST_CASE ( Logger, WriteNumberMax );
    RUN_TEST_CASE ( Logger, WriteNumberMin );
    RUN_TEST_CASE ( Logger, ReadEmpty );
    RUN_TEST_CASE ( Logger, ReadLessThanWrite );
    RUN_TEST_CASE ( Logger, ReadMoreThanWrite );
    RUN_TEST_CASE ( Logger, ReadPosition );
    RUN_TEST_CASE ( Logger, ReadPositionPastWrite );
}
