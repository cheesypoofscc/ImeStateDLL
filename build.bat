@echo off

call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

cl /LD /O2 /EHsc ImeState.cpp ole32.lib oleaut32.lib imm32.lib user32.lib


pause
