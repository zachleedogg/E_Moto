@REM java -jar "%~dp0ezbl_tools.jar" --communicator -com=COM21 -baud=230400 -timeout=1100 -artifact="%1"
java -jar "%~dp0ezbl_tools.jar" --communicator -com=I2C -i2c_address=0x60 -baud=400000 -timeout=1100 -artifact="%1"

@pause