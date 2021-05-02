@echo off
set PROJECT=MotionMatching
set BUILD_TYPE=rel

cd Builds

cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DPROJECT=%PROJECT% -DBUILD_TYPE=%BUILD_TYPE% -B %PROJECT%/%BUILD_TYPE%


cd %PROJECT%/%BUILD_TYPE% 

ninja
move %PROJECT%-%BUILD_TYPE%.exe ..
cd ../../..