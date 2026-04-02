@echo off

WHERE /Q g++
IF %ERRORLEVEL% NEQ 0 (
        ECHO ERROR: g++ not found; install MinGW via 'powershell -ExecutionPolicy Bypass -Command "irm https://get.scoop.sh | iex; scoop install mingw"', then restart terminal
        EXIT 1
)

WHERE /Q xmake
IF %ERRORLEVEL% NEQ 0 (
        ECHO ERROR: xmake not found; install via 'powershell -ExecutionPolicy Bypass -Command "irm https://xmake.io/psget.text | iex"', then restart terminal
        EXIT 1
)

xmake
