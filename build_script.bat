@echo off
set PROJECT=%1
set BUILD_TYPE=%2
set REBUILD=%3
set ECS_CODEGEN=%4
set CMAKE=%5
set CWD=%cd%
cd Builds
echo %PROJECT% %BUILD_TYPE% %REBUILD% %ECS_CODEGEN% %CMAKE% %CWD%


if "%REBUILD%"=="yes" (
    cd win/%PROJECT%
    rmdir /s %BUILD_TYPE% 
    cd ../..
    echo %PROJECT%/%BUILD_TYPE% was removed
)
if "%ECS_CODEGEN%"=="yes" (
    cd win/ECSCodeGen
    ECSCodeGen-rel.exe "-ecsPath -%CWD%/Common/Sources/Systems"
    ECSCodeGen-rel.exe "-ecsPath -%CWD%/Projects/%PROJECT%/Sources/Systems"
    cd ../..
)
if "%CMAKE%"=="yes" (
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DPROJECT=%PROJECT% -DBUILD_TYPE=%BUILD_TYPE% -B win/%PROJECT%/%BUILD_TYPE%
    echo builded
)

cd win/%PROJECT%/%BUILD_TYPE% 

ninja
move %PROJECT%-%BUILD_TYPE%.exe ..
cd ../..
if "%CMAKE%"=="yes" (
    copy assimp-vc142-mt.dll %PROJECT%
)
cd ../..