@echo off
set PROJECT=%1
set BUILD_TYPE=%2

cd Builds/win/%PROJECT%
%PROJECT%-%BUILD_TYPE%.exe "-project -%PROJECT%" "-loadDataBaseFromFBX -no"
cd ../../..