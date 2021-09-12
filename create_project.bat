@echo off
set PROJECT=%1

cd Projects
mkdir %PROJECT%
cd %PROJECT%
mkdir Resources
mkdir Shaders
mkdir Sources
cd Resources
mklink /d Common ..\..\..\CommonResources
cd ../Shaders
mklink /d Common ..\..\..\CommonShaders
cd ..
mklink CMakeLists.txt ..\GameCMakeLists.txt
