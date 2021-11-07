@echo off
set PROJECT=%1

set CWD=%cd%
cd ECSCodeGen
ECSCodeGen-rel.exe %CWD%/sources/common_systems %CWD%/sources/%PROJECT%
cd ..