@echo off
set PROJECT=%1
set BUILD_TYPE=%2
set DEPENDENCIES=%3
set CMAKE=%4
set RELEASE=%5


cd sources
if "%CMAKE%"=="yes" (
    cmake -G Ninja -DDEPENDENCIES=%DEPENDENCIES%%PROJECT% -DBUILD_TYPE=%BUILD_TYPE% -DRELEASE=%RELEASE% -B ../Builds/%BUILD_TYPE%
)

cd ../Builds/%BUILD_TYPE% 

"../../Tools/Ninja/ninja.exe"
set binPath=..\..\%PROJECT%
IF not exist %binPath% (mkdir %binPath%)

set APP_NAME=%PROJECT%-%BUILD_TYPE%.exe
IF "%BUILD_TYPE%"=="rel" (
    set APP_NAME=%PROJECT%.exe
)
ren Application.exe %APP_NAME%
move %APP_NAME% %binPath%
if "%CMAKE%"=="yes" (
    IF not exist %binPath%\assimp-vc142-mt.dll (copy ..\assimp-vc142-mt.dll %binPath%)
)
cd ../..