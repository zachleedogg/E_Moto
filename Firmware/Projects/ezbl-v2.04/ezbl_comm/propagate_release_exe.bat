@echo off
pushd %~dp0
FOR /R .. /D %%p IN (ezbl_integratio?) DO ^
echo ====== Copying to "%%p" & ^
cp -u .\Release\ezbl_comm.exe "%%p"
popd
@echo on
