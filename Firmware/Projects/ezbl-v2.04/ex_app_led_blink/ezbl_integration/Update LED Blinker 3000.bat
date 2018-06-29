@echo off
echo This is an example batch file to help firmware deployment. Customization is 
echo required before use. Refer to the ex_app_led_blink project documentation in 
echo "help\EZBL Library Documentation.pdf" for distribution information.
goto End

setlocal
:: Test if java is installed, sending stdout and stderr streams to the 
:: bit-bucket so nothing displays, pass or fail
java -version >>NUL 2>>NUL
if ERRORLEVEL 0 goto JavaFound
echo Java 7 or newer is required to update LED Blinker 3000. Please install 
echo the latest version of Java from: https://www.java.com/
goto End

:JavaFound
set USER_COM=
echo Please specify the communications port that LED Blinker 3000
echo is connected on. It will be updated to firmware v3001.
ezbl_comm.exe -enum
echo.
set /p USER_COM=Enter nothing to abort: 
if "%USER_COM%"=="" goto UserAbort
java -jar ezbl_tools.jar --communicator ^
    -baud=230400 -timeout=1100 -log="update_log.txt" ^
    -artifact="ex_app_led_blink.production.bl2" ^
    -com=%USER_COM%
goto End

:UserAbort
echo Firmware update aborted.

:End
pause
endlocal
@echo on