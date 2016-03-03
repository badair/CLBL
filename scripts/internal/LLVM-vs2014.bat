::should've used PowerShell - oh well

@echo off

set clbl_root=..\..\

::process 1st argument
if "%~1"=="x64" (
  set cmake_cmd=cmake .. -TLLVM-vs2014 -G"Visual Studio 14 2015 Win64"  -DCMAKE_CXX_FLAGS="-Qunused-arguments"
) else (
  if "%~1"=="Win32" (
    set cmake_cmd=cmake .. -TLLVM-vs2014 -G"Visual Studio 14 2015"
  )
)
if not defined cmake_cmd (
  echo BUILD SCRIPT: ERROR: Invalid parameter: %1 - expected 'Win32' or 'x64'
  call :exit_script
)

::process 2nd argument
if "%~2"=="Release" (
  set config_success="0"
  
) else (
  if "%~2"=="Debug" (
    set config_success="0"
  )
)
if not defined config_success (
  echo BUILD SCRIPT: ERROR: Invalid parameter: %2 - expected 'Release' or 'Debug'
  call :exit_script
)

::process 3rd argument
if "%~2"=="Release" (
  set config_success="0"
  
) else (
  if "%~2"=="Debug" (
    set config_success="0"
  )
)
if not defined config_success (
  echo BUILD SCRIPT: ERROR: Invalid parameter: %2 - expected 'Release' or 'Debug'
  call :exit_script
)

set msbuild_environment=/p:Configuration=%2 /p:Platform=%1
echo BUILD SCRIPT: Preparing %1 %2 build...

::find MSBuild.exe
if exist "%ProgramFiles%\MSBuild\14.0\Bin\MSBuild.exe" (
  echo BUILD SCRIPT: MSBuild.exe found...
  set msbuild_cmd="%ProgramFiles%\MSBuild\14.0\Bin\MSBuild.exe"
) else (
  if exist "%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe" (
    echo BUILD SCRIPT: MSBuild.exe found...
    set msbuild_cmd="%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe"
  )
)
if not defined msbuild_cmd (
  echo BUILD SCRIPT: ERROR: MSBuild.exe not found... is Visual Studio 2015 installed?
  call :exit_script
)

cd %clbl_root%
set build_dir= "build_LLVM_vs2014_%1_%2"

echo BUILD SCRIPT: Building at %build_dir%...

if exist %build_dir% (
  cd %build_dir%
  if exist "CLBL.sln" (
    echo BUILD SCRIPT: CLBL.sln found - cleaning...
    echo . | %msbuild_cmd% CLBL.sln /t:clean %msbuild_environment% /v:m /m /nologo
	cd ..\
  ) else (
    echo BUILD SCRIPT: CLBL.sln not found - deleting build directory...
    cd ..
    rmdir /q /s %build_dir%
  )
)

if exist %build_dir% (
  cd %build_dir%
) else (
  echo BUILD SCRIPT: Creating build directory...
  mkdir %build_dir%
  cd %build_dir%
  echo BUILD SCRIPT: Running cmake...
  echo . | %cmake_cmd%
  echo BUILD SCRIPT: CLBL.sln...
  echo . | %msbuild_cmd% CLBL.sln %msbuild_environment% /v:m /m /nologo
)

echo BUILD SCRIPT: CHECK.vcxproj...
echo . | %msbuild_cmd% CHECK.vcxproj %msbuild_environment%

::echo BUILD SCRIPT: doc/doc.vcxproj...
::echo . | %msbuild_cmd% doc\doc.vcxproj %msbuild_environment%

::echo BUILD SCRIPT: RUN_TESTS.vcxproj...
::echo . | %msbuild_cmd% RUN_TESTS.vcxproj %msbuild_environment%

:exit_script
::pause if this script was started by double clicked from Windows
::Explorer instead of starting from a pre-existing cmd.exe window
for %%x in (%cmdcmdline%) do if /i "%%~x"=="/c" set CLICKED=1
if defined CLICKED pause

cd ..\scripts

