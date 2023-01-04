@echo off
set PROJECT=%1
set BUILD_TYPE=%2
set CMAKE=%3
set AOT_COMPILER=%4

cd sources
if "%CMAKE%"=="yes" (
    cmake -G Ninja -DGAME=%PROJECT% -DBUILD_TYPE=%BUILD_TYPE% -DAOT_COMPILER=%AOT_COMPILER% -B ../Builds/%BUILD_TYPE%
)

cd ../Builds/%BUILD_TYPE%

ninja

set binPath=..\..\%PROJECT%
IF not exist %binPath% (mkdir %binPath%)


if "%CMAKE%"=="yes" (
    IF not exist %binPath%\assimp.dll (copy ..\assimp.dll %binPath%)
)
cd ../..