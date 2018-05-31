EESchema Schematic File Version 4
LIBS:BMS_01-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 30
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 1100 750  1900 1050
U 5AAB4D1F
F0 "Micro" 197
F1 "Micro.sch" 197
$EndSheet
$Sheet
S 3400 750  2000 1100
U 5AAC0444
F0 "PowerRails" 197
F1 "PowerRails.sch" 197
$EndSheet
$Comp
L device:D D1
U 1 1 5AC2C048
P 3050 4750
F 0 "D1" H 3050 4534 50  0000 C CNN
F 1 "D" H 3050 4625 50  0000 C CNN
F 2 "" H 3050 4750 50  0001 C CNN
F 3 "" H 3050 4750 50  0001 C CNN
	1    3050 4750
	-1   0    0    1   
$EndComp
Wire Wire Line
	2750 4450 3300 4450
Wire Wire Line
	2750 4750 2900 4750
Wire Wire Line
	2750 4650 3100 4650
Wire Wire Line
	3200 4750 3700 4750
Wire Wire Line
	3400 4650 3700 4650
Wire Wire Line
	3600 4450 3700 4450
$Comp
L device:D D2
U 1 1 5AC2C068
P 3250 4650
F 0 "D2" H 3250 4434 50  0000 C CNN
F 1 "D" H 3250 4525 50  0000 C CNN
F 2 "" H 3250 4650 50  0001 C CNN
F 3 "" H 3250 4650 50  0001 C CNN
	1    3250 4650
	-1   0    0    1   
$EndComp
$Comp
L device:D D3
U 1 1 5AC2BFD0
P 3450 4450
F 0 "D3" H 3450 4234 50  0000 C CNN
F 1 "D" H 3450 4325 50  0000 C CNN
F 2 "" H 3450 4450 50  0001 C CNN
F 3 "" H 3450 4450 50  0001 C CNN
	1    3450 4450
	-1   0    0    1   
$EndComp
$Sheet
S 3700 4350 1650 3000
U 5AC37515
F0 "Stack_01" 50
F1 "BattStack.sch" 50
F2 "TOS" I R 5350 4900 50 
F3 "Mode" I R 5350 5000 50 
F4 "Vreg" I R 5350 5100 50 
F5 "Vref" I R 5350 5200 50 
F6 "SCKL" I R 5350 4750 50 
F7 "SDI" I R 5350 4650 50 
F8 "SDO" I R 5350 4550 50 
F9 "CSBI" I R 5350 4450 50 
F10 "CSBO" I L 3700 4450 50 
F11 "SDIO" I L 3700 4650 50 
F12 "SCKO" I L 3700 4750 50 
F13 "Cell_12" I L 3700 5000 50 
F14 "Cell_11" I L 3700 5100 50 
F15 "Cell_10" I L 3700 5200 50 
F16 "Cell_09" I L 3700 5300 50 
F17 "Cell_08" I L 3700 5400 50 
F18 "Cell_07" I L 3700 5500 50 
F19 "Cell_06" I L 3700 5600 50 
F20 "Cell_05" I L 3700 5700 50 
F21 "Cell_04" I L 3700 5800 50 
F22 "Cell_03" I L 3700 5900 50 
F23 "Cell_01" I L 3700 6100 50 
F24 "Cell_GND" I L 3700 6200 50 
F25 "Cell_02" I L 3700 6000 50 
$EndSheet
$Sheet
S 1050 4350 1700 3000
U 5AC3A314
F0 "Stack_02" 50
F1 "BattStack.sch" 50
F2 "TOS" I R 2750 4900 50 
F3 "Mode" I R 2750 5000 50 
F4 "Vreg" I R 2750 5100 50 
F5 "Vref" I R 2750 5200 50 
F6 "SCKL" I R 2750 4750 50 
F7 "SDI" I R 2750 4650 50 
F8 "SDO" I R 2750 4550 50 
F9 "CSBI" I R 2750 4450 50 
F10 "CSBO" I L 1050 4450 50 
F11 "SDIO" I L 1050 4650 50 
F12 "SCKO" I L 1050 4750 50 
F13 "Cell_12" I L 1050 5000 50 
F14 "Cell_11" I L 1050 5100 50 
F15 "Cell_10" I L 1050 5200 50 
F16 "Cell_09" I L 1050 5300 50 
F17 "Cell_08" I L 1050 5400 50 
F18 "Cell_07" I L 1050 5500 50 
F19 "Cell_06" I L 1050 5600 50 
F20 "Cell_05" I L 1050 5700 50 
F21 "Cell_04" I L 1050 5800 50 
F22 "Cell_03" I L 1050 5900 50 
F23 "Cell_01" I L 1050 6100 50 
F24 "Cell_GND" I L 1050 6200 50 
F25 "Cell_02" I L 1050 6000 50 
$EndSheet
$Sheet
S 6650 750  1800 1150
U 5AC3D661
F0 "Communication" 50
F1 "Communication.sch" 50
$EndSheet
$EndSCHEMATC