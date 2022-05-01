@echo off
set PROJECT=%1
set ONLY_PROJECT_FOLDER=%2
set SRC=%3
set DST=%4\%PROJECT%

mkdir %DST%
xcopy /s /d /i %SRC%\%PROJECT%  %DST%\%PROJECT% 
if "%ONLY_PROJECT_FOLDER%"=="false" (
    xcopy /s /d %SRC%\shaders\%PROJECT%  %DST%\shaders\%PROJECT%
    xcopy /s /d %SRC%\shaders\common  %DST%\shaders\common
    xcopy /s /d %SRC%\resources\%PROJECT%  %DST%\resources\%PROJECT%
    xcopy /s /d %SRC%\resources\common  %DST%\resources\common

)
rem del %DST%\%PROJECT%\%PROJECT%-dbg.exe
rem del %DST%\%PROJECT%\%PROJECT%-dev.exe
rem del %DST%\%PROJECT%\%PROJECT%.exe
rem cd  %DST%\%PROJECT%
rem ren %PROJECT%-rel.exe %PROJECT%.exe


@echo cd %PROJECT%> %DST%\%PROJECT%.bat
@echo "%PROJECT%-rel.exe">> %DST%\%PROJECT%.bat