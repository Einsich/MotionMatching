@echo off
set PROJECT=MotionMatching
set BUILD_TYPE=rel

cd Builds/%PROJECT%
%PROJECT%-%BUILD_TYPE%.exe "-project -%PROJECT%" "-showBones -yes" "-showGoal -yes" "-showMatchingStatistic -yes" "-loadDataBaseFromFBX -no"
cd ../..