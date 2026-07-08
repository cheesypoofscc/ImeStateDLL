@echo off

call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

cl /LD /O2 ImeState.cpp ole32.lib oleaut32.lib


pause