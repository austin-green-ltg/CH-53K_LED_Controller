#include <string.h>

#include "unity_fixture.h"      /* UNITY */
#include "voltage_handler.h"    /* CUT */
#include "logger.h"             /* CUT */
#include "stm32l412xx-bsp.h"    /* CUT */

const uint16_t VoltageErrorLowValue     = 24000u;
const uint16_t VoltageLowValue          = 26000u;
const uint16_t VoltageNormalValue       = 28000u;
const uint16_t VoltageHighValue         = 30000u;
const uint16_t VoltageErrorHighValue    = 32000u;

extern uint16_t voltage_value_mV;

TEST_GROUP(Voltage_Handler);

TEST_SETUP(Voltage_Handler)
{
    /* executed before *every* non-skipped test */
}

TEST_TEAR_DOWN(Voltage_Handler)
{
    /* executed after *every* non-skipped and non-failing test */
    voltage_value_mV = VoltageNormalValue; // return to default value
    GetVoltageRange(); // set voltage range
}

/* start Voltage_Handler tests */
TEST(Voltage_Handler, GetDefaultVoltage)
{
    TEST_ASSERT_EQUAL_UINT16(VoltageNormalValue, GetVoltage()); // Default value
}
TEST(Voltage_Handler, GetDefaultVoltageRange)
{
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());
}
TEST(Voltage_Handler, GetVoltage)
{
    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_UINT16(VoltageNormalValue, GetVoltage());
    voltage_value_mV = 29245u;
    TEST_ASSERT_EQUAL_UINT16(29245u, GetVoltage());

    voltage_value_mV = VoltageErrorLowValue;
    TEST_ASSERT_EQUAL_UINT16(VoltageErrorLowValue, GetVoltage());

    voltage_value_mV = VoltageLowValue;
    TEST_ASSERT_EQUAL_UINT16(VoltageLowValue, GetVoltage());

    voltage_value_mV = VoltageHighValue;
    TEST_ASSERT_EQUAL_UINT16(VoltageHighValue, GetVoltage());

    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_UINT16(VoltageErrorHighValue, GetVoltage());
}
TEST(Voltage_Handler, VoltageRangeNormalStepping)
{
    // VoltageNormal -> VoltageHigh
    voltage_value_mV = VoltageHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    // VoltageHigh -> Error VoltageHigh
    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorHigh, GetVoltageRange());

    // Error VoltageHigh -> VoltageHigh
    voltage_value_mV = VoltageHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    // VoltageHigh -> VoltageNormal
    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    // VoltageNormal -> VoltageLow
    voltage_value_mV = VoltageLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageLow, GetVoltageRange());

    // VoltageLow -> Error VoltageLow
    voltage_value_mV = VoltageErrorLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorLow, GetVoltageRange());

    // Error VoltageLow -> VoltageLow
    voltage_value_mV = VoltageLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageLow, GetVoltageRange());

    // VoltageLow -> VoltageNormal
    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());
}
TEST(Voltage_Handler, VoltageRangeErrorLowTesting)
{
    voltage_value_mV = VoltageErrorLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorLow, GetVoltageRange());

    // Error VoltageLow -> VoltageLow
    voltage_value_mV = VoltageLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageLow, GetVoltageRange());

    voltage_value_mV = VoltageErrorLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorLow, GetVoltageRange());

    // Error VoltageLow -> VoltageNormal
    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    voltage_value_mV = VoltageErrorLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorLow, GetVoltageRange());

    // Error VoltageLow -> VoltageHigh
    voltage_value_mV = VoltageHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    voltage_value_mV = VoltageErrorLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorLow, GetVoltageRange());

    // Error VoltageLow -> Error VoltageHigh
    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorHigh, GetVoltageRange());
}
TEST(Voltage_Handler, VoltageRangeLowTesting)
{
    voltage_value_mV = VoltageLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageLow, GetVoltageRange());

    // VoltageLow -> Error VoltageLow
    voltage_value_mV = VoltageErrorLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorLow, GetVoltageRange());

    voltage_value_mV = VoltageLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageLow, GetVoltageRange());

    // VoltageLow -> VoltageNormal
    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    voltage_value_mV = VoltageLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageLow, GetVoltageRange());

    // VoltageLow -> VoltageHigh
    voltage_value_mV = VoltageHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    voltage_value_mV = VoltageLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageLow, GetVoltageRange());

    // VoltageLow -> Error VoltageHigh
    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorHigh, GetVoltageRange());
}
TEST(Voltage_Handler, VoltageRangeNormalTesting)
{
    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    // VoltageNormal -> Error VoltageLow
    voltage_value_mV = VoltageErrorLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorLow, GetVoltageRange());

    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    // VoltageNormal -> VoltageLow
    voltage_value_mV = VoltageLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageLow, GetVoltageRange());

    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    // VoltageNormal -> VoltageHigh
    voltage_value_mV = VoltageHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    // VoltageNormal -> Error VoltageHigh
    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorHigh, GetVoltageRange());
}
TEST(Voltage_Handler, VoltageRangeHighTesting)
{
    voltage_value_mV = VoltageHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    // VoltageHigh -> Error VoltageLow
    voltage_value_mV = VoltageErrorLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorLow, GetVoltageRange());

    voltage_value_mV = VoltageHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    // VoltageHigh -> VoltageLow
    voltage_value_mV = VoltageLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageLow, GetVoltageRange());

    voltage_value_mV = VoltageHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    // VoltageHigh -> VoltageNormal
    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    voltage_value_mV = VoltageHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    // VoltageHigh -> Error VoltageHigh
    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorHigh, GetVoltageRange());
}
TEST(Voltage_Handler, VoltageRangeErrorHighTesting)
{
    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorHigh, GetVoltageRange());

    // Error VoltageHigh -> Error VoltageLow
    voltage_value_mV = VoltageErrorLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorLow, GetVoltageRange());

    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorHigh, GetVoltageRange());

    // Error VoltageHigh -> VoltageLow
    voltage_value_mV = VoltageLowValue;
    TEST_ASSERT_EQUAL_INT(VoltageLow, GetVoltageRange());

    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorHigh, GetVoltageRange());

    // Error VoltageHigh -> VoltageNormal
    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorHigh, GetVoltageRange());

    // Error VoltageHigh -> VoltageHigh
    voltage_value_mV = VoltageHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());
}
TEST(Voltage_Handler, VoltageRangeErrorLowBoundaryTesting)
{
    voltage_value_mV = (VoltageHighValue - 1);
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    voltage_value_mV = VoltageHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    voltage_value_mV = (VoltageErrorHighValue - 1);
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorHigh, GetVoltageRange());
}
TEST(Voltage_Handler, VoltageRangeLowBoundaryTesting)
{
    voltage_value_mV = (VoltageHighValue - 1);
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    voltage_value_mV = VoltageHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    voltage_value_mV = (VoltageErrorHighValue - 1);
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorHigh, GetVoltageRange());
}
TEST(Voltage_Handler, VoltageRangeNormalBoundaryTesting)
{
    voltage_value_mV = (VoltageHighValue - 1);
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    voltage_value_mV = VoltageHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    voltage_value_mV = (VoltageErrorHighValue - 1);
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    voltage_value_mV = VoltageNormalValue;
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorHigh, GetVoltageRange());
}
TEST(Voltage_Handler, VoltageRangeHighBoundaryTesting)
{
    voltage_value_mV = VoltageHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    voltage_value_mV = (VoltageHighValue - 1);
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    voltage_value_mV = VoltageHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    voltage_value_mV = (VoltageErrorHighValue - 1);
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorHigh, GetVoltageRange());
}
TEST(Voltage_Handler, VoltageRangeErrorHighBoundaryTesting)
{
    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorHigh, GetVoltageRange());

    voltage_value_mV = (VoltageErrorHighValue - 1);
    TEST_ASSERT_EQUAL_INT(VoltageHigh, GetVoltageRange());

    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorHigh, GetVoltageRange());

    voltage_value_mV = (VoltageHighValue - 1);
    TEST_ASSERT_EQUAL_INT(VoltageNormal, GetVoltageRange());

    voltage_value_mV = VoltageErrorHighValue;
    TEST_ASSERT_EQUAL_INT(VoltageErrorHigh, GetVoltageRange());
}

TEST(Voltage_Handler, NormalToErrorLowPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageErrorLowValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Error Low Voltage 24000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(0, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, NormalToLowPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageLowValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Low Voltage 26000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(0, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, NormalToHighPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageHighValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "High Voltage 30000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(0, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, NormalToErrorHighPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageErrorHighValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Error High Voltage 32000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(0, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}
TEST(Voltage_Handler, NormalToNormalNoPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageNormalValue;
    GetVoltageRange();

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

TEST(Voltage_Handler, ErrorLowToLowPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageErrorLowValue;
    GetVoltageRange();

    voltage_value_mV = VoltageLowValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Low Voltage 26000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Error Low Voltage 24000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, ErrorLowToNormalPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageErrorLowValue;
    GetVoltageRange();

    voltage_value_mV = VoltageNormalValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Normal Voltage 28000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Error Low Voltage 24000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, ErrorLowToHighPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageErrorLowValue;
    GetVoltageRange();

    voltage_value_mV = VoltageHighValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "High Voltage 30000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Error Low Voltage 24000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, ErrorLowToErrorHighPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageErrorLowValue;
    GetVoltageRange();

    voltage_value_mV = VoltageErrorHighValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Error High Voltage 32000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Error Low Voltage 24000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, ErrorLowToErrorLowNoPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageErrorLowValue;
    GetVoltageRange();

    voltage_value_mV = VoltageErrorLowValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Error Low Voltage 24000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, LowToErrorLowPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageLowValue;
    GetVoltageRange();

    voltage_value_mV = VoltageErrorLowValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Error Low Voltage 24000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Low Voltage 26000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, LowToNormalPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageLowValue;
    GetVoltageRange();

    voltage_value_mV = VoltageNormalValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Normal Voltage 28000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Low Voltage 26000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, LowToHighPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageLowValue;
    GetVoltageRange();

    voltage_value_mV = VoltageHighValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "High Voltage 30000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Low Voltage 26000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, LowToErrorHighPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageLowValue;
    GetVoltageRange();

    voltage_value_mV = VoltageErrorHighValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Error High Voltage 32000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Low Voltage 26000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, LowToLowNoPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageLowValue;
    GetVoltageRange();

    voltage_value_mV = VoltageLowValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Low Voltage 26000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, HighToErrorLowPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageHighValue;
    GetVoltageRange();

    voltage_value_mV = VoltageErrorLowValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Error Low Voltage 24000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("High Voltage 30000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, HighToLowPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageHighValue;
    GetVoltageRange();

    voltage_value_mV = VoltageLowValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Low Voltage 26000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("High Voltage 30000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, HighToNormalPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageHighValue;
    GetVoltageRange();

    voltage_value_mV = VoltageNormalValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Normal Voltage 28000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("High Voltage 30000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, HighToErrorHighPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageHighValue;
    GetVoltageRange();

    voltage_value_mV = VoltageErrorHighValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Error High Voltage 32000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("High Voltage 30000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, HighToHighNoPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageHighValue;
    GetVoltageRange();

    voltage_value_mV = VoltageHighValue;
    GetVoltageRange();
    voltage_value_mV = VoltageHighValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("High Voltage 30000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, ErrorHighToErrorLowPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageErrorHighValue;
    GetVoltageRange();

    voltage_value_mV = VoltageErrorLowValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Error Low Voltage 24000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Error High Voltage 32000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, ErrorHighToLowPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageErrorHighValue;
    GetVoltageRange();

    voltage_value_mV = VoltageLowValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Low Voltage 26000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Error High Voltage 32000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, ErrorHighToNormalPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageErrorHighValue;
    GetVoltageRange();

    voltage_value_mV = VoltageNormalValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "Normal Voltage 28000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Error High Voltage 32000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, ErrorHighToHighPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageErrorHighValue;
    GetVoltageRange();

    voltage_value_mV = VoltageHighValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "High Voltage 30000 mV\n";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Error High Voltage 32000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}

TEST(Voltage_Handler, ErrorHighToErrorHighNoPrintout)
{
    extern FILE* file_ptr;
    file_ptr = fopen("testFile.txt", "w");

    voltage_value_mV = VoltageErrorHighValue;
    GetVoltageRange();

    voltage_value_mV = VoltageErrorHighValue;
    GetVoltageRange();

    fclose(file_ptr);

    file_ptr = fopen("testFile.txt", "r");

    char* expected = "";
    char* string = (char*)calloc(100 , sizeof(char));

    ReadLog(strlen("Error High Voltage 32000 mV\n") + 1, string,  strlen(expected));
    TEST_ASSERT_EQUAL_STRING(expected, string);

    fclose(file_ptr);        // close file
    remove("testFile.txt");  // delete file
    free(string);
}
/* end Voltage_Handler tests */


TEST_GROUP_RUNNER(Voltage_Handler)
{
    RUN_TEST_CASE(Voltage_Handler, GetDefaultVoltage);
    RUN_TEST_CASE(Voltage_Handler, GetDefaultVoltageRange);
    RUN_TEST_CASE(Voltage_Handler, GetVoltage);
    RUN_TEST_CASE(Voltage_Handler, VoltageRangeNormalStepping);
    RUN_TEST_CASE(Voltage_Handler, VoltageRangeErrorLowTesting);
    RUN_TEST_CASE(Voltage_Handler, VoltageRangeLowTesting);
    RUN_TEST_CASE(Voltage_Handler, VoltageRangeNormalTesting);
    RUN_TEST_CASE(Voltage_Handler, VoltageRangeHighTesting);
    RUN_TEST_CASE(Voltage_Handler, VoltageRangeErrorHighTesting);
    RUN_TEST_CASE(Voltage_Handler, VoltageRangeErrorLowBoundaryTesting);
    RUN_TEST_CASE(Voltage_Handler, VoltageRangeLowBoundaryTesting);
    RUN_TEST_CASE(Voltage_Handler, VoltageRangeNormalBoundaryTesting);
    RUN_TEST_CASE(Voltage_Handler, VoltageRangeHighBoundaryTesting);
    RUN_TEST_CASE(Voltage_Handler, VoltageRangeErrorHighBoundaryTesting);
    RUN_TEST_CASE(Voltage_Handler, NormalToErrorLowPrintout);
    RUN_TEST_CASE(Voltage_Handler, NormalToLowPrintout);
    RUN_TEST_CASE(Voltage_Handler, NormalToHighPrintout);
    RUN_TEST_CASE(Voltage_Handler, NormalToErrorHighPrintout);
    RUN_TEST_CASE(Voltage_Handler, NormalToNormalNoPrintout);
    RUN_TEST_CASE(Voltage_Handler, ErrorLowToLowPrintout);
    RUN_TEST_CASE(Voltage_Handler, ErrorLowToNormalPrintout);
    RUN_TEST_CASE(Voltage_Handler, ErrorLowToHighPrintout);
    RUN_TEST_CASE(Voltage_Handler, ErrorLowToErrorHighPrintout);
    RUN_TEST_CASE(Voltage_Handler, ErrorLowToErrorLowNoPrintout);
    RUN_TEST_CASE(Voltage_Handler, LowToErrorLowPrintout);
    RUN_TEST_CASE(Voltage_Handler, LowToNormalPrintout);
    RUN_TEST_CASE(Voltage_Handler, LowToHighPrintout);
    RUN_TEST_CASE(Voltage_Handler, LowToErrorHighPrintout);
    RUN_TEST_CASE(Voltage_Handler, LowToLowNoPrintout);
    RUN_TEST_CASE(Voltage_Handler, HighToErrorLowPrintout);
    RUN_TEST_CASE(Voltage_Handler, HighToLowPrintout);
    RUN_TEST_CASE(Voltage_Handler, HighToNormalPrintout);
    RUN_TEST_CASE(Voltage_Handler, HighToErrorHighPrintout);
    RUN_TEST_CASE(Voltage_Handler, HighToHighNoPrintout);
    RUN_TEST_CASE(Voltage_Handler, ErrorHighToErrorLowPrintout);
    RUN_TEST_CASE(Voltage_Handler, ErrorHighToLowPrintout);
    RUN_TEST_CASE(Voltage_Handler, ErrorHighToNormalPrintout);
    RUN_TEST_CASE(Voltage_Handler, ErrorHighToHighPrintout);
    RUN_TEST_CASE(Voltage_Handler, ErrorHighToErrorHighNoPrintout);
}