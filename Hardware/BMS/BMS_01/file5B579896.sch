EESchema Schematic File Version 4
LIBS:BMS_01-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 32 32
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L device:R R?
U 1 1 5B579950
P 5850 3550
F 0 "R?" H 5920 3596 50  0000 L CNN
F 1 "R" H 5920 3505 50  0000 L CNN
F 2 "" V 5780 3550 50  0001 C CNN
F 3 "~" H 5850 3550 50  0001 C CNN
	1    5850 3550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3VP #PWR?
U 1 1 5B579957
P 5850 3400
F 0 "#PWR?" H 6000 3350 50  0001 C CNN
F 1 "+3.3VP" H 5870 3543 50  0000 C CNN
F 2 "" H 5850 3400 50  0001 C CNN
F 3 "" H 5850 3400 50  0001 C CNN
	1    5850 3400
	1    0    0    -1  
$EndComp
$Comp
L device:R R?
U 1 1 5B57995D
P 5600 3800
F 0 "R?" V 5393 3800 50  0000 C CNN
F 1 "R" V 5484 3800 50  0000 C CNN
F 2 "" V 5530 3800 50  0001 C CNN
F 3 "~" H 5600 3800 50  0001 C CNN
	1    5600 3800
	0    1    1    0   
$EndComp
$Comp
L device:C C?
U 1 1 5B579964
P 5250 4050
F 0 "C?" H 5365 4096 50  0000 L CNN
F 1 "C" H 5365 4005 50  0000 L CNN
F 2 "" H 5288 3900 50  0001 C CNN
F 3 "~" H 5250 4050 50  0001 C CNN
	1    5250 4050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5B57996B
P 5250 4200
F 0 "#PWR?" H 5250 3950 50  0001 C CNN
F 1 "GND" H 5255 4027 50  0000 C CNN
F 2 "" H 5250 4200 50  0001 C CNN
F 3 "" H 5250 4200 50  0001 C CNN
	1    5250 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 3900 5250 3800
Wire Wire Line
	5250 3800 5450 3800
Wire Wire Line
	5750 3800 5850 3800
Wire Wire Line
	5850 3800 5850 3700
Wire Wire Line
	6050 3800 5850 3800
Connection ~ 5850 3800
$Comp
L power:GND #PWR?
U 1 1 5B579977
P 5800 4200
F 0 "#PWR?" H 5800 3950 50  0001 C CNN
F 1 "GND" H 5805 4027 50  0000 C CNN
F 2 "" H 5800 4200 50  0001 C CNN
F 3 "" H 5800 4200 50  0001 C CNN
	1    5800 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 4200 5800 4000
Wire Wire Line
	5800 4000 6050 4000
Wire Wire Line
	5200 3800 5250 3800
Connection ~ 5250 3800
Text HLabel 6050 3800 2    50   Output ~ 0
TEMP_SENSOR_OUT
Text HLabel 6050 4000 2    50   Input ~ 0
TEMP_SENSOR_RTN
Text HLabel 5200 3800 0    50   Output ~ 0
TEMP_SENSOR_AI
$EndSCHEMATC