@echo off
set PROJECT=%1
set BUILD_TYPE=%2
set REBUILD=%3
set ECS_CODEGEN=%4
set CMAKE=%5
set RELEASE=%6

set CWD=%cd%
cd Builds


if "%REBUILD%"=="yes" (
    cd win/%PROJECT%
    rmdir /s %BUILD_TYPE% 
    cd ../..
    echo %PROJECT%/%BUILD_TYPE% was removed
)
if "%ECS_CODEGEN%"=="yes" (
    cd win/ECSCodeGen
    ECSCodeGen-rel.exe %CWD%/Common/Sources/Systems %CWD%/Projects/%PROJECT%/Sources/Systems
    cd ../..
)
cd ..
Rem call build_lib_script.bat Common/Sources Engine %BUILD_TYPE% %CMAKE% 
Rem call build_lib_script.bat Projects/%PROJECT%/Sources %PROJECT% %BUILD_TYPE% %CMAKE% 
cd Projects

if "%CMAKE%"=="yes" (
    cmake -G Ninja -DPROJECT=%PROJECT% -DBUILD_TYPE=%BUILD_TYPE% -DRELEASE=%RELEASE% -B ../Builds/win/%BUILD_TYPE%
    echo builded
)
cd ../Builds/win/%BUILD_TYPE% 

ninja
ren Application.exe %PROJECT%-%BUILD_TYPE%.exe
move %PROJECT%-%BUILD_TYPE%.exe ../%PROJECT%
if "%CMAKE%"=="yes" (
    copy assimp-vc142-mt.dll %PROJECT%
)
cd ../../..