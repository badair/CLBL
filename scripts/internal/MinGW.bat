@echo off

cd ..\..\
set build_dir=%cd%\build_gcc

if not exist "%1\bin\g++.exe" (
    goto build_if_default_gcc_found
)

setlocal
where /q g++ && goto gcc_conflict

set Path=%Path%;%1\bin
set CURRENT_MINGW_PATH=%1\
for %%f in (%CURRENT_MINGW_PATH:~0,-1%) do set MINGW_FOLDER=%%~nxf
set build_dir=%cd%\build_%MINGW_FOLDER%
goto main_logic

:build_if_default_gcc_found
    echo BUILD SCRIPT: MinGW directory not specified. Looking for g++...
    where /q g++ && goto main_logic
    echo ERROR: g++.exe not found. You may pass the MinGW root directory as the first argument to this script.
    goto exit_script
    
:gcc_conflict
    echo WARNING: g++.exe cannot be exist in the PATH when specifying a MinGW directory. Using the g++ found in the PATH instead.
    goto main_logic
    
:main_logic

    set startdir=%cd%
    if not exist "%build_dir%" ( mkdir %build_dir% )
    echo BUILD SCRIPT: Building at %build_dir%...
    cd %build_dir%

    if not exist ".\Makefile" (
        @echo on
        cmake -G "MinGW Makefiles" .. -DCMAKE_CXX_COMPILER=g++
        @echo off
    ) else (
        @echo on
        make clean
        @echo off
    )

    @echo on
    make check
    @echo off

    cd %startdir%

:exit_script

    for %%x in (%cmdcmdline%) do if /i "%%~x"=="/c" set CLICKED=1
    if defined CLICKED pause
