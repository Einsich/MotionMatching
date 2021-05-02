@echo off
set PROJECT=%1
set BUILD_TYPE=%2

cd Builds/win/%PROJECT%
%PROJECT%-%BUILD_TYPE%.exe "-project -%PROJECT%" "-showBones -yes" "-showGoal -yes" "-showMatchingStatistic -yes" "-loadDataBaseFromFBX -no"
cd ../../..