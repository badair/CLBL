@echo off

cd ..

if not exist "build" (
  mkdir build
  cd build
  @echo on
  echo cmake .. -TLLVM-vs2014 -G"Visual Studio 14 2015 Win64"  -DCMAKE_CXX_FLAGS="-Qunused-arguments"
  @echo off
) else (
  cd build
)

if exist "CLBL.sln" (
  @echo on
  "C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe" CLBL.sln /t:clean /p:Configuration=Debug /p:Platform=x64 /v:m /m /nologo
  @echo off
) else (
  cd ..
  rmdir /q /s build
  mkdir build
  cd build
  @echo on
  cmake .. -TLLVM-vs2014 -G"Visual Studio 14 2015 Win64"  -DCMAKE_CXX_FLAGS="-Qunused-arguments"
)

"C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe" CLBL.sln /p:Configuration=Debug /p:Platform=x64 /v:m /m /nologo
"C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe" RUN_TESTS.vcxproj /p:Configuration=Debug /p:Platform=x64

pause

@echo off
cd ..
cd scripts