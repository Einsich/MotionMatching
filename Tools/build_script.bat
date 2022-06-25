@echo off
set PROJECT=%1
set BUILD_TYPE=%2
set CMAKE=%3

cd sources
if "%CMAKE%"=="yes" (
    cmake -G Ninja  -DGAME=%PROJECT% -DBUILD_TYPE=%BUILD_TYPE% -B ../Builds/%BUILD_TYPE%
)

cd ../Builds/%BUILD_TYPE%

"../../Tools/Ninja/ninja.exe"
set binPath=..\..\%PROJECT%
IF not exist %binPath% (mkdir %binPath%)

set APP_NAME=%PROJECT%-%BUILD_TYPE%.exe

ren Application.exe %APP_NAME%
move %APP_NAME% %binPath%
if "%CMAKE%"=="yes" (
    IF not exist %binPath%\assimp-vc142-mt.dll (copy ..\assimp-vc142-mt.dll %binPath%)
)
cd ../..