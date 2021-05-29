@echo off
set PROJECT=ECSCodeGen
set BUILD_TYPE=%1

cmake -G Ninja -DPROJECT=%PROJECT% -DBUILD_TYPE=%BUILD_TYPE% -B Builds/win/%PROJECT%/%BUILD_TYPE% -S Projects/%PROJECT%

cd Builds/win/%PROJECT%/%BUILD_TYPE% 

ninja
move %PROJECT%-%BUILD_TYPE%.exe ..
cd ../../../..