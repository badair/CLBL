@echo off

set startdir=%cd%
cd internal
call LLVM-vs2014.bat Win32 Release
cd %startdir%
