java -jar "%~dp0ezbl_tools.jar" --blobber -dump -artifact="%1" > "%TEMP%\%~n1-%~n0.txt"
@if not errorlevel 0 pause
@pushd "%TEMP%"
start "%~n0" "%~n1-%~n0.txt"
@popd

