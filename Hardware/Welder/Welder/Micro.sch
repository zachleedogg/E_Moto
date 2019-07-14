EESchema Schematic File Version 4
LIBS:Welder-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 8
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
L MCU_Module:Arduino_Nano_v3.x A?
U 1 1 5D462BB6
P 5600 4100
AR Path="/5D462BB6" Ref="A?"  Part="1" 
AR Path="/5D45E1C0/5D462BB6" Ref="A1"  Part="1" 
F 0 "A1" H 5300 5150 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 5050 5250 50  0000 C CNN
F 2 "Modules:Arduino_Nano" H 5750 3150 50  0001 L CNN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 5600 3100 50  0001 C CNN
F 4 "~" H -4250 -300 50  0001 C CNN "PartNumber"
	1    5600 4100
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5D462BBD
P 5800 2600
AR Path="/5D462BBD" Ref="#PWR?"  Part="1" 
AR Path="/5D45E1C0/5D462BBD" Ref="#PWR019"  Part="1" 
F 0 "#PWR019" H 5800 2450 50  0001 C CNN
F 1 "+5V" H 5815 2773 50  0000 C CNN
F 2 "" H 5800 2600 50  0001 C CNN
F 3 "" H 5800 2600 50  0001 C CNN
	1    5800 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D462BC3
P 5650 5200
AR Path="/5D462BC3" Ref="#PWR?"  Part="1" 
AR Path="/5D45E1C0/5D462BC3" Ref="#PWR018"  Part="1" 
F 0 "#PWR018" H 5650 4950 50  0001 C CNN
F 1 "GND" V 5655 5072 50  0000 R CNN
F 2 "" H 5650 5200 50  0001 C CNN
F 3 "" H 5650 5200 50  0001 C CNN
	1    5650 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 5100 5700 5200
Wire Wire Line
	5700 5200 5650 5200
Wire Wire Line
	5600 5200 5600 5100
Connection ~ 5650 5200
Wire Wire Line
	5650 5200 5600 5200
Text GLabel 5100 4000 0    50   Output ~ 0
BUZZ_PWM
Text GLabel 5100 4200 0    50   Input ~ 0
MODE_BUTTON
Text GLabel 5100 4300 0    50   Input ~ 0
SELECT_BUTTON
Text GLabel 5100 4400 0    50   Input ~ 0
WELD_BUTTON
Text GLabel 5100 4100 0    50   Output ~ 0
DEBUG_LED_EN
Text GLabel 5100 3900 0    50   Output ~ 0
MOT_EN
Text GLabel 6100 4100 2    50   Input ~ 0
PRIMARY_A_AI
Text GLabel 6100 4200 2    50   Input ~ 0
PRIMARY_B_AI
$Comp
L device:C C?
U 1 1 5D462BD7
P 6100 2850
AR Path="/5D462BD7" Ref="C?"  Part="1" 
AR Path="/5D45E1C0/5D462BD7" Ref="C7"  Part="1" 
F 0 "C7" H 6215 2896 50  0000 L CNN
F 1 "10uF" H 6215 2805 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 6138 2700 50  0001 C CNN
F 3 "http://www.samsungsem.com/kr/support/product-search/mlcc/__icsFiles/afieldfile/2019/05/10/19-CL31A106MBHNNNE.pdf" H 6100 2850 50  0001 C CNN
F 4 "CL31A106MBHNNNE" H 6100 2850 50  0001 C CNN "PartNumber"
	1    6100 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 2700 6100 2650
Wire Wire Line
	6100 2650 5800 2650
Wire Wire Line
	5800 2650 5800 2600
Wire Wire Line
	5800 2650 5800 3100
Connection ~ 5800 2650
$Comp
L power:GND #PWR?
U 1 1 5D462BE3
P 6100 3000
AR Path="/5D462BE3" Ref="#PWR?"  Part="1" 
AR Path="/5D45E1C0/5D462BE3" Ref="#PWR020"  Part="1" 
F 0 "#PWR020" H 6100 2750 50  0001 C CNN
F 1 "GND" V 6105 2872 50  0000 R CNN
F 2 "" H 6100 3000 50  0001 C CNN
F 3 "" H 6100 3000 50  0001 C CNN
	1    6100 3000
	1    0    0    -1  
$EndComp
Text GLabel 6100 4600 2    50   BiDi ~ 0
SCL
Text GLabel 6100 4500 2    50   BiDi ~ 0
SDA
Text GLabel 5100 4500 0    50   Output ~ 0
CS
Text GLabel 5100 4600 0    50   Output ~ 0
MOSI
Text GLabel 5100 4700 0    50   Input ~ 0
MISO
Text GLabel 5100 4800 0    50   Output ~ 0
CLK
Text GLabel 5100 3700 0    50   Input ~ 0
ZERO_CROSS
Text GLabel 2100 4900 0    50   BiDi ~ 0
SCL
Text GLabel 2100 4800 0    50   BiDi ~ 0
SDA
$Comp
L device:R R?
U 1 1 5D4673EE
P 2450 4650
AR Path="/5D4673EE" Ref="R?"  Part="1" 
AR Path="/5D45E1C0/5D4673EE" Ref="R6"  Part="1" 
F 0 "R6" H 2380 4604 50  0000 R CNN
F 1 "10k" H 2380 4695 50  0000 R CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2380 4650 50  0001 C CNN
F 3 "~" H 2450 4650 50  0001 C CNN
F 4 "~" H -2500 -1950 50  0001 C CNN "PartNumber"
	1    2450 4650
	-1   0    0    1   
$EndComp
$Comp
L device:R R?
U 1 1 5D4673F6
P 2750 4650
AR Path="/5D4673F6" Ref="R?"  Part="1" 
AR Path="/5D45E1C0/5D4673F6" Ref="R8"  Part="1" 
F 0 "R8" H 2680 4604 50  0000 R CNN
F 1 "10k" H 2680 4695 50  0000 R CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2680 4650 50  0001 C CNN
F 3 "~" H 2750 4650 50  0001 C CNN
F 4 "~" H -2200 -1950 50  0001 C CNN "PartNumber"
	1    2750 4650
	-1   0    0    1   
$EndComp
Wire Wire Line
	2100 4800 2450 4800
Wire Wire Line
	2100 4900 2750 4900
Wire Wire Line
	2750 4900 2750 4800
$Comp
L power:+5V #PWR?
U 1 1 5D467400
P 2750 4500
AR Path="/5D467400" Ref="#PWR?"  Part="1" 
AR Path="/5D45E1C0/5D467400" Ref="#PWR016"  Part="1" 
F 0 "#PWR016" H 2750 4350 50  0001 C CNN
F 1 "+5V" H 2765 4673 50  0000 C CNN
F 2 "" H 2750 4500 50  0001 C CNN
F 3 "" H 2750 4500 50  0001 C CNN
	1    2750 4500
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5D467406
P 2450 4500
AR Path="/5D467406" Ref="#PWR?"  Part="1" 
AR Path="/5D45E1C0/5D467406" Ref="#PWR014"  Part="1" 
F 0 "#PWR014" H 2450 4350 50  0001 C CNN
F 1 "+5V" H 2465 4673 50  0000 C CNN
F 2 "" H 2450 4500 50  0001 C CNN
F 3 "" H 2450 4500 50  0001 C CNN
	1    2450 4500
	1    0    0    -1  
$EndComp
Text GLabel 2150 3750 0    50   Output ~ 0
CS
Text GLabel 2150 3850 0    50   Input ~ 0
MISO
$Comp
L device:R R?
U 1 1 5D46740F
P 2500 3600
AR Path="/5D46740F" Ref="R?"  Part="1" 
AR Path="/5D45E1C0/5D46740F" Ref="R7"  Part="1" 
F 0 "R7" H 2430 3554 50  0000 R CNN
F 1 "10k" H 2430 3645 50  0000 R CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2430 3600 50  0001 C CNN
F 3 "~" H 2500 3600 50  0001 C CNN
F 4 "~" H -2450 -3000 50  0001 C CNN "PartNumber"
	1    2500 3600
	-1   0    0    1   
$EndComp
$Comp
L device:R R?
U 1 1 5D467417
P 2800 3600
AR Path="/5D467417" Ref="R?"  Part="1" 
AR Path="/5D45E1C0/5D467417" Ref="R9"  Part="1" 
F 0 "R9" H 2730 3554 50  0000 R CNN
F 1 "10k" H 2730 3645 50  0000 R CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2730 3600 50  0001 C CNN
F 3 "~" H 2800 3600 50  0001 C CNN
F 4 "~" H -2150 -3000 50  0001 C CNN "PartNumber"
	1    2800 3600
	-1   0    0    1   
$EndComp
Wire Wire Line
	2150 3750 2500 3750
Wire Wire Line
	2150 3850 2800 3850
Wire Wire Line
	2800 3850 2800 3750
$Comp
L power:+5V #PWR?
U 1 1 5D467421
P 2800 3450
AR Path="/5D467421" Ref="#PWR?"  Part="1" 
AR Path="/5D45E1C0/5D467421" Ref="#PWR017"  Part="1" 
F 0 "#PWR017" H 2800 3300 50  0001 C CNN
F 1 "+5V" H 2815 3623 50  0000 C CNN
F 2 "" H 2800 3450 50  0001 C CNN
F 3 "" H 2800 3450 50  0001 C CNN
	1    2800 3450
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5D467427
P 2500 3450
AR Path="/5D467427" Ref="#PWR?"  Part="1" 
AR Path="/5D45E1C0/5D467427" Ref="#PWR015"  Part="1" 
F 0 "#PWR015" H 2500 3300 50  0001 C CNN
F 1 "+5V" H 2515 3623 50  0000 C CNN
F 2 "" H 2500 3450 50  0001 C CNN
F 3 "" H 2500 3450 50  0001 C CNN
	1    2500 3450
	1    0    0    -1  
$EndComp
$EndSCHEMATC
