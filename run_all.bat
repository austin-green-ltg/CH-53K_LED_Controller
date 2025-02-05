cd .\Core
call format.bat
cd ..
cd .\Docs\latex
start call make.bat
cd ..\..
cd .\Test
call format.bat
start call run_tests_rebuild.bat
cd ..
exit