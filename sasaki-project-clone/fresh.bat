@echo off
setlocal enabledelayedexpansion

:: Remove any leading spaces and get the start time in total milliseconds
for /F "tokens=1-4 delims=:.," %%a in ("%time%") do (
    set /A startHour=(100%%a %% 100)
    set /A startMin=(100%%b %% 100)
    set /A startSec=(100%%c %% 100)
    set /A startMS=(100%%d %% 100)
    set /A startTotalMS=(startHour * 3600000) + (startMin * 60000) + (startSec * 1000) + startMS
)

echo Performing fresh setup...

:: Remove old godot-cpp directory if it exists
if exist godot-cpp (
    rmdir /S /Q godot-cpp
)

:: Clone the godot-cpp repository (version 4.3)
git clone -b 4.3 https://github.com/godotengine/godot-cpp/ temp-godot-cpp

:: Move contents (including folders) from temp-godot-cpp to godot-cpp
robocopy temp-godot-cpp godot-cpp /MOVE /E

:: Dump the extension API
echo abt to cd
cd godot-cpp\gdextension
echo cd'd
godot --dump-extension-api

:: Build godot-cpp using SCons
cd ..
scons

:: Build the project
cd ..
scons

:: Get the end time in total milliseconds
for /F "tokens=1-4 delims=:.," %%a in ("%time%") do (
    set /A endHour=(100%%a %% 100)
    set /A endMin=(100%%b %% 100)
    set /A endSec=(100%%c %% 100)
    set /A endMS=(100%%d %% 100)
    set /A endTotalMS=(endHour * 3600000) + (endMin * 60000) + (endSec * 1000) + endMS
)

:: Calculate elapsed time in milliseconds
set /A elapsedTotalMS=(endTotalMS - startTotalMS)

:: If elapsed time is negative (due to passing midnight), add 24 hours in milliseconds
if !elapsedTotalMS! lss 0 (set /A elapsedTotalMS+=86400000)

:: Convert elapsed milliseconds to hours, minutes, seconds, and milliseconds
set /A elapsedHour=(elapsedTotalMS / 3600000)
set /A elapsedTotalMS=(elapsedTotalMS %% 3600000)
set /A elapsedMin=(elapsedTotalMS / 60000)
set /A elapsedTotalMS=(elapsedTotalMS %% 60000)
set /A elapsedSec=(elapsedTotalMS / 1000)
set /A elapsedMS=(elapsedTotalMS %% 1000)

:: Display elapsed time
echo Elapsed Time: !elapsedHour! hours, !elapsedMin! minutes, !elapsedSec!.!elapsedMS! seconds

endlocal
pause
