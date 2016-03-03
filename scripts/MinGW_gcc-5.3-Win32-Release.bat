@echo off
set startdir=%cd%
cd internal
call MinGW.bat C:\mingw32_gcc-5.3
cd %startdir%