REM The purpose of this file is to check that everything is compiling fine with the IAR Compiler
REM and to run the test code. It will pop up two windows, one for the IAR compiler and one for
REM the tests. It is so you do not have to open EWARM to check the regular code compiles when
REM creating tests.

REM Compile with IAR Compiler
start cmd.exe /k make -f iar_makefile
REM Run Tests
start cmd.exe /k make -f test_makefile

exit
