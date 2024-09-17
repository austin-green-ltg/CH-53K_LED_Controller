#include <stdlib.h>         /* rand */
#include <string.h>         /* rand */

#include "unity_fixture.h"      /* UNITY */
#include "stm32l412xx-bsp.h"    /* CUT */
#include "fram.h"               /* CUT */

/* Defines */

/* Functions */
extern void initFram ( void );

/* Global Variables */
extern uint8_t writeProtect;
extern uint8_t chipEnable;
extern uint8_t writeEnable;
extern uint8_t statusRegister;

TEST_GROUP ( Fram );

TEST_SETUP ( Fram )
{
    /* executed before *every* non-skipped test */
    initFram();
}


TEST_TEAR_DOWN ( Fram )
{
    /* executed after *every* non-skipped and non-failing test */
}


/* start fram tests */
TEST ( Fram, WriteProtect )
{
    WRITE_PROTECT_STATE wps;

    wps = WPS_PROTECTED; /* write protected */

    framWriteProtect ( wps ) ;

    TEST_ASSERT_EQUAL_UINT8 ( 0, writeProtect );

    wps = WPS_WRITEABLE; /* write enabled   */

    framWriteProtect ( wps ) ;

    TEST_ASSERT_EQUAL_UINT8 ( 1, writeProtect );
}

TEST ( Fram, ChipSelect )
{
    CHIP_SELECT_STATE css;

    css = CSS_ASSERT; /* chip select disabled */

    framChipSelect ( css ) ;

    TEST_ASSERT_EQUAL_UINT8 ( 0, chipEnable );

    css = CSS_RELEASE; /* chip select enabled */

    framChipSelect ( css ) ;

    TEST_ASSERT_EQUAL_UINT8 ( 1, chipEnable );
}

TEST ( Fram, WriteDisable )
{
    framWriteDisable( ) ;

    TEST_ASSERT_EQUAL_UINT8 ( 0, writeEnable );
}

TEST ( Fram, WriteEnable )
{

    framWriteEnable( ) ;

    TEST_ASSERT_EQUAL_UINT8 ( 1, writeEnable );
}

TEST ( Fram, ReadSr )
{
    const unsigned char txchar [ 2 ] = {OC_WRSR, SR_WEL};
    unsigned char sr = 0;
    unsigned char expectedsr = SR_WEL;
    unsigned short len = 2;

    transferData ( txchar, len );

    framReadSr (&sr ) ;

    TEST_ASSERT_EQUAL_UINT8 ( expectedsr, sr );
}

TEST ( Fram, WriteSr )
{
    const unsigned char txchar [ 2 ] = {OC_RDSR, 0};
    unsigned char rxchar [ 2 ] = {0, 0};
    unsigned char sr = SR_WEL;
    unsigned char expectedsr = SR_WEL;
    unsigned short len = 2;

    framWriteSr ( sr ) ;

    transferData ( txchar, len );
    receiveData ( rxchar, len );

    TEST_ASSERT_EQUAL_UINT8 ( expectedsr, rxchar [ 1 ] );
}

TEST ( Fram, WRMemory )
{
    unsigned short addr = ( rand() & 0xFFFF );

    if ( addr > 0xFFFF - 64 )
    {
        addr = 0xFFFF - 64; // ensure we don't write off the end of fram
    }

    const unsigned char txsr [ 2 ] = {OC_WRSR, SR_WEL};
    unsigned char txchar [ 64 ];
    unsigned char rxchar [ 64 ];
    unsigned short len = 64;

    for ( int i = 0; i < len; i++ )
    {
        txchar [ i ] = rand();
        rxchar [ i ] = 0;
    }

    transferData ( txsr, 2 );

    framWriteMemory ( addr, txchar, len ) ;

    framReadMemory ( addr, rxchar, len ) ;

    TEST_ASSERT_EQUAL_UINT8_ARRAY ( txchar, rxchar, len );
}

TEST ( Fram, Test )
{

    uint8_t pass = framTest( ) ; /* test code to verify fram operation */

    TEST_ASSERT_TRUE ( pass );
}

/* end fram tests */


TEST_GROUP_RUNNER ( Fram )
{
    RUN_TEST_CASE ( Fram, WriteProtect );
    RUN_TEST_CASE ( Fram, ChipSelect );
    RUN_TEST_CASE ( Fram, WriteDisable );
    RUN_TEST_CASE ( Fram, WriteEnable );
    RUN_TEST_CASE ( Fram, ReadSr );
    RUN_TEST_CASE ( Fram, WriteSr );
    RUN_TEST_CASE ( Fram, WRMemory );
    RUN_TEST_CASE ( Fram, Test );
}
