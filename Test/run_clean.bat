REM to add another test, add test in selected folder
REM to add another test group, duplicate folder, change test.c, add file to Makefile in C_SRCS, and duplicate pattern below

REM change to folder name
pushd button handler
REM clean build
start cmd.exe /c make clean
REM return to original folder (here)
popd
pushd delay handler
start cmd.exe /c make clean
popd
pushd pwm handler
start cmd.exe /c make clean
popd
pushd requirements
start cmd.exe /c make clean
popd
pushd thermistor handler
start cmd.exe /c make clean
popd
