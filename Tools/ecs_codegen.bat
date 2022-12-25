@echo off
set PROJECT=%1

set CWD=%cd%
cd Builds/rel/ECSCodeGen
rem ECSCodeGen.exe %CWD%/sources/common_systems %CWD%/sources/%PROJECT%
cd ../../..