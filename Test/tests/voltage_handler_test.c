#include <string.h>

#include "unity_fixture.h"      /* UNITY */
#include "voltage_handler.h"    /* CUT */
#include "logger.h"             /* CUT */
#include "stm32l412xx-bsp.h"    /* CUT */

const uint16_t VoltageErrorLowValue_dV = 240u;
const uint16_t VoltageLowValue_dV = 260u;
const uint16_t VoltageNormalValue_dV = 280u;
const uint16_t VoltageHighValue_dV = 300u;
const uint16_t VoltageErrorHighValue_dV = 320u;

extern void initFram ( void );

extern uint32_t address; // last written to address
extern uint16_t voltage_value_dV;

static uint32_t numVoltageLogs = 0;

TEST_GROUP ( Voltage_Handler );

TEST_SETUP ( Voltage_Handler )
{
    /* executed before *every* non-skipped test */
    initFram();
}

TEST_TEAR_DOWN ( Voltage_Handler )
{
    /* executed after *every* non-skipped and non-failing test */
    voltage_value_dV = VoltageNormalValue_dV; // return to default value
    GetVoltageRange(); // set voltage range
}

/* start Voltage_Handler tests */
TEST ( Voltage_Handler, GetDefaultVoltage )
{
    TEST_ASSERT_EQUAL_UINT16 ( VoltageNormalValue_dV,
                               GetVoltage() ); // Default value
}

TEST ( Voltage_Handler, GetDefaultVoltageRange )
{
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );
}

TEST ( Voltage_Handler, GetVoltage )
{
    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_UINT16 ( VoltageNormalValue_dV, GetVoltage() );
    voltage_value_dV = 292u;
    TEST_ASSERT_EQUAL_UINT16 ( 292u, GetVoltage() );

    voltage_value_dV = VoltageErrorLowValue_dV;
    TEST_ASSERT_EQUAL_UINT16 ( VoltageErrorLowValue_dV, GetVoltage() );

    voltage_value_dV = VoltageLowValue_dV;
    TEST_ASSERT_EQUAL_UINT16 ( VoltageLowValue_dV, GetVoltage() );

    voltage_value_dV = VoltageHighValue_dV;
    TEST_ASSERT_EQUAL_UINT16 ( VoltageHighValue_dV, GetVoltage() );

    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_UINT16 ( VoltageErrorHighValue_dV, GetVoltage() );
}

TEST ( Voltage_Handler, VoltageRangeNormalStepping )
{
    // VoltageNormal -> VoltageHigh
    voltage_value_dV = VoltageHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    // VoltageHigh -> Error VoltageHigh
    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorHigh, GetVoltageRange() );

    // Error VoltageHigh -> VoltageHigh
    voltage_value_dV = VoltageHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    // VoltageHigh -> VoltageNormal
    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    // VoltageNormal -> VoltageLow
    voltage_value_dV = VoltageLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageLow, GetVoltageRange() );

    // VoltageLow -> Error VoltageLow
    voltage_value_dV = VoltageErrorLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorLow, GetVoltageRange() );

    // Error VoltageLow -> VoltageLow
    voltage_value_dV = VoltageLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageLow, GetVoltageRange() );

    // VoltageLow -> VoltageNormal
    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );
}

TEST ( Voltage_Handler, VoltageRangeErrorLowTesting )
{
    voltage_value_dV = VoltageErrorLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorLow, GetVoltageRange() );

    // Error VoltageLow -> VoltageLow
    voltage_value_dV = VoltageLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageLow, GetVoltageRange() );

    voltage_value_dV = VoltageErrorLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorLow, GetVoltageRange() );

    // Error VoltageLow -> VoltageNormal
    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    voltage_value_dV = VoltageErrorLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorLow, GetVoltageRange() );

    // Error VoltageLow -> VoltageHigh
    voltage_value_dV = VoltageHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    voltage_value_dV = VoltageErrorLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorLow, GetVoltageRange() );

    // Error VoltageLow -> Error VoltageHigh
    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorHigh, GetVoltageRange() );
}

TEST ( Voltage_Handler, VoltageRangeLowTesting )
{
    voltage_value_dV = VoltageLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageLow, GetVoltageRange() );

    // VoltageLow -> Error VoltageLow
    voltage_value_dV = VoltageErrorLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorLow, GetVoltageRange() );

    voltage_value_dV = VoltageLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageLow, GetVoltageRange() );

    // VoltageLow -> VoltageNormal
    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    voltage_value_dV = VoltageLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageLow, GetVoltageRange() );

    // VoltageLow -> VoltageHigh
    voltage_value_dV = VoltageHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    voltage_value_dV = VoltageLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageLow, GetVoltageRange() );

    // VoltageLow -> Error VoltageHigh
    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorHigh, GetVoltageRange() );
}

TEST ( Voltage_Handler, VoltageRangeNormalTesting )
{
    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    // VoltageNormal -> Error VoltageLow
    voltage_value_dV = VoltageErrorLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorLow, GetVoltageRange() );

    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    // VoltageNormal -> VoltageLow
    voltage_value_dV = VoltageLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageLow, GetVoltageRange() );

    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    // VoltageNormal -> VoltageHigh
    voltage_value_dV = VoltageHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    // VoltageNormal -> Error VoltageHigh
    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorHigh, GetVoltageRange() );
}

TEST ( Voltage_Handler, VoltageRangeHighTesting )
{
    voltage_value_dV = VoltageHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    // VoltageHigh -> Error VoltageLow
    voltage_value_dV = VoltageErrorLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorLow, GetVoltageRange() );

    voltage_value_dV = VoltageHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    // VoltageHigh -> VoltageLow
    voltage_value_dV = VoltageLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageLow, GetVoltageRange() );

    voltage_value_dV = VoltageHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    // VoltageHigh -> VoltageNormal
    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    voltage_value_dV = VoltageHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    // VoltageHigh -> Error VoltageHigh
    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorHigh, GetVoltageRange() );
}

TEST ( Voltage_Handler, VoltageRangeErrorHighTesting )
{
    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorHigh, GetVoltageRange() );

    // Error VoltageHigh -> Error VoltageLow
    voltage_value_dV = VoltageErrorLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorLow, GetVoltageRange() );

    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorHigh, GetVoltageRange() );

    // Error VoltageHigh -> VoltageLow
    voltage_value_dV = VoltageLowValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageLow, GetVoltageRange() );

    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorHigh, GetVoltageRange() );

    // Error VoltageHigh -> VoltageNormal
    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorHigh, GetVoltageRange() );

    // Error VoltageHigh -> VoltageHigh
    voltage_value_dV = VoltageHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );
}

TEST ( Voltage_Handler, VoltageRangeErrorLowBoundaryTesting )
{
    voltage_value_dV = ( VoltageHighValue_dV - 1 );
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    voltage_value_dV = VoltageHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    voltage_value_dV = ( VoltageErrorHighValue_dV - 1 );
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorHigh, GetVoltageRange() );
}

TEST ( Voltage_Handler, VoltageRangeLowBoundaryTesting )
{
    voltage_value_dV = ( VoltageHighValue_dV - 1 );
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    voltage_value_dV = VoltageHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    voltage_value_dV = ( VoltageErrorHighValue_dV - 1 );
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorHigh, GetVoltageRange() );
}

TEST ( Voltage_Handler, VoltageRangeNormalBoundaryTesting )
{
    voltage_value_dV = ( VoltageHighValue_dV - 1 );
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    voltage_value_dV = VoltageHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    voltage_value_dV = ( VoltageErrorHighValue_dV - 1 );
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    voltage_value_dV = VoltageNormalValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorHigh, GetVoltageRange() );
}

TEST ( Voltage_Handler, VoltageRangeHighBoundaryTesting )
{
    voltage_value_dV = VoltageHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    voltage_value_dV = ( VoltageHighValue_dV - 1 );
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    voltage_value_dV = VoltageHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    voltage_value_dV = ( VoltageErrorHighValue_dV - 1 );
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorHigh, GetVoltageRange() );
}

TEST ( Voltage_Handler, VoltageRangeErrorHighBoundaryTesting )
{
    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorHigh, GetVoltageRange() );

    voltage_value_dV = ( VoltageErrorHighValue_dV - 1 );
    TEST_ASSERT_EQUAL_INT ( VoltageHigh, GetVoltageRange() );

    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorHigh, GetVoltageRange() );

    voltage_value_dV = ( VoltageHighValue_dV - 1 );
    TEST_ASSERT_EQUAL_INT ( VoltageNormal, GetVoltageRange() );

    voltage_value_dV = VoltageErrorHighValue_dV;
    TEST_ASSERT_EQUAL_INT ( VoltageErrorHigh, GetVoltageRange() );
}

TEST ( Voltage_Handler, NormalToErrorLowPrintout )
{

    voltage_value_dV = VoltageErrorLowValue_dV;
    GetVoltageRange();

    char* expected = "Error Low Voltage 240 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, NormalToLowPrintout )
{

    voltage_value_dV = VoltageLowValue_dV;
    GetVoltageRange();

    char* expected = "Low Voltage 260 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, NormalToHighPrintout )
{

    voltage_value_dV = VoltageHighValue_dV;
    GetVoltageRange();

    char* expected = "High Voltage 300 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, NormalToErrorHighPrintout )
{

    voltage_value_dV = VoltageErrorHighValue_dV;
    GetVoltageRange();

    char* expected = "Error High Voltage 320 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, NormalToNormalNoPrintout )
{

    voltage_value_dV = VoltageNormalValue_dV;
    GetVoltageRange();

    char* expected = "";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, ErrorLowToLowPrintout )
{

    voltage_value_dV = VoltageErrorLowValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageLowValue_dV;
    GetVoltageRange();

    char* expected = "Low Voltage 260 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, ErrorLowToNormalPrintout )
{

    voltage_value_dV = VoltageErrorLowValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageNormalValue_dV;
    GetVoltageRange();

    char* expected = "Normal Voltage 280 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, ErrorLowToHighPrintout )
{

    voltage_value_dV = VoltageErrorLowValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageHighValue_dV;
    GetVoltageRange();

    char* expected = "High Voltage 300 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, ErrorLowToErrorHighPrintout )
{

    voltage_value_dV = VoltageErrorLowValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageErrorHighValue_dV;
    GetVoltageRange();

    char* expected = "Error High Voltage 320 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, ErrorLowToErrorLowNoPrintout )
{

    voltage_value_dV = VoltageErrorLowValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageErrorLowValue_dV;
    GetVoltageRange();

    char* expected = "";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, LowToErrorLowPrintout )
{

    voltage_value_dV = VoltageLowValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageErrorLowValue_dV;
    GetVoltageRange();

    char* expected = "Error Low Voltage 240 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, LowToNormalPrintout )
{

    voltage_value_dV = VoltageLowValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageNormalValue_dV;
    GetVoltageRange();

    char* expected = "Normal Voltage 280 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, LowToHighPrintout )
{

    voltage_value_dV = VoltageLowValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageHighValue_dV;
    GetVoltageRange();

    char* expected = "High Voltage 300 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, LowToErrorHighPrintout )
{

    voltage_value_dV = VoltageLowValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageErrorHighValue_dV;
    GetVoltageRange();

    char* expected = "Error High Voltage 320 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, LowToLowNoPrintout )
{

    voltage_value_dV = VoltageLowValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageLowValue_dV;
    GetVoltageRange();

    char* expected = "";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, HighToErrorLowPrintout )
{

    voltage_value_dV = VoltageHighValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageErrorLowValue_dV;
    GetVoltageRange();

    char* expected = "Error Low Voltage 240 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, HighToLowPrintout )
{

    voltage_value_dV = VoltageHighValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageLowValue_dV;
    GetVoltageRange();

    char* expected = "Low Voltage 260 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, HighToNormalPrintout )
{

    voltage_value_dV = VoltageHighValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageNormalValue_dV;
    GetVoltageRange();

    char* expected = "Normal Voltage 280 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, HighToErrorHighPrintout )
{

    voltage_value_dV = VoltageHighValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageErrorHighValue_dV;
    GetVoltageRange();

    char* expected = "Error High Voltage 320 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, HighToHighNoPrintout )
{

    voltage_value_dV = VoltageHighValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageHighValue_dV;
    GetVoltageRange();
    voltage_value_dV = VoltageHighValue_dV;
    GetVoltageRange();

    char* expected = "";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, ErrorHighToErrorLowPrintout )
{

    voltage_value_dV = VoltageErrorHighValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageErrorLowValue_dV;
    GetVoltageRange();

    char* expected = "Error Low Voltage 240 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, ErrorHighToLowPrintout )
{

    voltage_value_dV = VoltageErrorHighValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageLowValue_dV;
    GetVoltageRange();

    char* expected = "Low Voltage 260 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, ErrorHighToNormalPrintout )
{

    voltage_value_dV = VoltageErrorHighValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageNormalValue_dV;
    GetVoltageRange();

    char* expected = "Normal Voltage 280 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, ErrorHighToHighPrintout )
{

    voltage_value_dV = VoltageErrorHighValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageHighValue_dV;
    GetVoltageRange();

    char* expected = "High Voltage 300 dV\n";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, ErrorHighToErrorHighNoPrintout )
{

    voltage_value_dV = VoltageErrorHighValue_dV;
    GetVoltageRange();

    voltage_value_dV = VoltageErrorHighValue_dV;
    GetVoltageRange();

    char* expected = "";
    char* string = ( char* ) calloc ( 100, sizeof ( char ) );

    ReadLog ( address, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    free ( string );
}

TEST ( Voltage_Handler, LogVoltage )
{
    char expected [ VOLTAGE_LOG_SIZE ] = "10";
    char* string = ( char* ) calloc ( VOLTAGE_LOG_SIZE, sizeof ( char ) );

    voltage_value_dV = 10;

    LogVoltage();

    ReadLog ( STARTING_VOLTAGE_ADDRESS + numVoltageLogs * VOLTAGE_LOG_SIZE, string,
              strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    numVoltageLogs++;

    if ( numVoltageLogs >= TOTAL_VOLTAGE_LOGS )
    {
        numVoltageLogs = 0;
    }

    free ( string );
}

TEST ( Voltage_Handler, LogVoltageAgain )
{
    char expected [ VOLTAGE_LOG_SIZE ] = "1";
    char* string = ( char* ) calloc ( VOLTAGE_LOG_SIZE, sizeof ( char ) );

    voltage_value_dV = 1;

    LogVoltage();

    ReadLog ( STARTING_VOLTAGE_ADDRESS + numVoltageLogs * VOLTAGE_LOG_SIZE, string,
              strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );

    numVoltageLogs++;

    if ( numVoltageLogs >= TOTAL_VOLTAGE_LOGS )
    {
        numVoltageLogs = 0;
    }

    free ( string );
}

TEST ( Voltage_Handler, LogFiftyVoltages )
{
    char expected [ VOLTAGE_LOG_SIZE ];
    char* string = ( char* ) calloc ( VOLTAGE_LOG_SIZE, sizeof ( char ) );

    const uint8_t logs_to_write = 50;

    for ( uint8_t i = 0; i < logs_to_write; i++ )
    {
        voltage_value_dV = i;
        LogVoltage();
    }

    for ( uint16_t i = 0; i < logs_to_write; i++ )
    {
        snprintf ( expected, sizeof ( expected ), "%hu", i );
        ReadLog ( STARTING_VOLTAGE_ADDRESS + numVoltageLogs * VOLTAGE_LOG_SIZE, string,
                  strlen ( expected ) );
        TEST_ASSERT_EQUAL_STRING ( expected, string );
        numVoltageLogs++;

        if ( numVoltageLogs >= TOTAL_VOLTAGE_LOGS )
        {
            numVoltageLogs = 0;
        }
    }

    free ( string );
}

TEST ( Voltage_Handler, ReadWholeLog )
{
    // This just ensures that we don't crash if we write the entire thing

    char* string = ( char* ) calloc ( VOLTAGE_LOG_SIZE, sizeof ( char ) );

    const uint8_t logs_to_read = TOTAL_VOLTAGE_LOGS;

    for ( uint8_t i = 0; i < logs_to_read; i++ )
    {
        ReadLog ( STARTING_VOLTAGE_ADDRESS + numVoltageLogs * VOLTAGE_LOG_SIZE, string,
                  VOLTAGE_LOG_SIZE );
        numVoltageLogs++;

        if ( numVoltageLogs >= TOTAL_VOLTAGE_LOGS )
        {
            numVoltageLogs = 0;
        }
    }

    free ( string );
}

TEST ( Voltage_Handler, WriteLogOverflow )
{
    char expected [ VOLTAGE_LOG_SIZE ];
    char* string = ( char* ) calloc ( VOLTAGE_LOG_SIZE, sizeof ( char ) );

    // Fill Log Up
    while ( numVoltageLogs < TOTAL_VOLTAGE_LOGS )
    {
        LogVoltage();
        numVoltageLogs++;
    }

    numVoltageLogs = 0;

    // Write One More
    voltage_value_dV = 27;
    snprintf ( expected, sizeof ( expected ), "%hu", voltage_value_dV );

    LogVoltage();

    // Make sure that last log was written at STARTING_VOLTAGE_ADDRESS
    ReadLog ( STARTING_VOLTAGE_ADDRESS, string, strlen ( expected ) );
    TEST_ASSERT_EQUAL_STRING ( expected, string );
    numVoltageLogs++;

    if ( numVoltageLogs > TOTAL_VOLTAGE_LOGS )
    {
        numVoltageLogs = 0;
    }

    free ( string );

}

/* end Voltage_Handler tests */


TEST_GROUP_RUNNER ( Voltage_Handler )
{
    RUN_TEST_CASE ( Voltage_Handler, GetDefaultVoltage );
    RUN_TEST_CASE ( Voltage_Handler, GetDefaultVoltageRange );
    RUN_TEST_CASE ( Voltage_Handler, GetVoltage );
    RUN_TEST_CASE ( Voltage_Handler, VoltageRangeNormalStepping );
    RUN_TEST_CASE ( Voltage_Handler, VoltageRangeErrorLowTesting );
    RUN_TEST_CASE ( Voltage_Handler, VoltageRangeLowTesting );
    RUN_TEST_CASE ( Voltage_Handler, VoltageRangeNormalTesting );
    RUN_TEST_CASE ( Voltage_Handler, VoltageRangeHighTesting );
    RUN_TEST_CASE ( Voltage_Handler, VoltageRangeErrorHighTesting );
    RUN_TEST_CASE ( Voltage_Handler, VoltageRangeErrorLowBoundaryTesting );
    RUN_TEST_CASE ( Voltage_Handler, VoltageRangeLowBoundaryTesting );
    RUN_TEST_CASE ( Voltage_Handler, VoltageRangeNormalBoundaryTesting );
    RUN_TEST_CASE ( Voltage_Handler, VoltageRangeHighBoundaryTesting );
    RUN_TEST_CASE ( Voltage_Handler, VoltageRangeErrorHighBoundaryTesting );
    RUN_TEST_CASE ( Voltage_Handler, NormalToErrorLowPrintout );
    RUN_TEST_CASE ( Voltage_Handler, NormalToLowPrintout );
    RUN_TEST_CASE ( Voltage_Handler, NormalToHighPrintout );
    RUN_TEST_CASE ( Voltage_Handler, NormalToErrorHighPrintout );
    RUN_TEST_CASE ( Voltage_Handler, NormalToNormalNoPrintout );
    RUN_TEST_CASE ( Voltage_Handler, ErrorLowToLowPrintout );
    RUN_TEST_CASE ( Voltage_Handler, ErrorLowToNormalPrintout );
    RUN_TEST_CASE ( Voltage_Handler, ErrorLowToHighPrintout );
    RUN_TEST_CASE ( Voltage_Handler, ErrorLowToErrorHighPrintout );
    RUN_TEST_CASE ( Voltage_Handler, ErrorLowToErrorLowNoPrintout );
    RUN_TEST_CASE ( Voltage_Handler, LowToErrorLowPrintout );
    RUN_TEST_CASE ( Voltage_Handler, LowToNormalPrintout );
    RUN_TEST_CASE ( Voltage_Handler, LowToHighPrintout );
    RUN_TEST_CASE ( Voltage_Handler, LowToErrorHighPrintout );
    RUN_TEST_CASE ( Voltage_Handler, LowToLowNoPrintout );
    RUN_TEST_CASE ( Voltage_Handler, HighToErrorLowPrintout );
    RUN_TEST_CASE ( Voltage_Handler, HighToLowPrintout );
    RUN_TEST_CASE ( Voltage_Handler, HighToNormalPrintout );
    RUN_TEST_CASE ( Voltage_Handler, HighToErrorHighPrintout );
    RUN_TEST_CASE ( Voltage_Handler, HighToHighNoPrintout );
    RUN_TEST_CASE ( Voltage_Handler, ErrorHighToErrorLowPrintout );
    RUN_TEST_CASE ( Voltage_Handler, ErrorHighToLowPrintout );
    RUN_TEST_CASE ( Voltage_Handler, ErrorHighToNormalPrintout );
    RUN_TEST_CASE ( Voltage_Handler, ErrorHighToHighPrintout );
    RUN_TEST_CASE ( Voltage_Handler, ErrorHighToErrorHighNoPrintout );
    RUN_TEST_CASE ( Voltage_Handler, LogVoltage );
    RUN_TEST_CASE ( Voltage_Handler, LogVoltageAgain );
    RUN_TEST_CASE ( Voltage_Handler, LogFiftyVoltages );
    RUN_TEST_CASE ( Voltage_Handler, ReadWholeLog );
    RUN_TEST_CASE ( Voltage_Handler, WriteLogOverflow );
}
