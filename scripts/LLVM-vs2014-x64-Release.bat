@echo off

set startdir=%cd%
cd internal
call LLVM-vs2014.bat x64 Release
cd %startdir%
