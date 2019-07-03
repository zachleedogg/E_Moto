EESchema Schematic File Version 4
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 8
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
L Relay_SolidState:MOC3052M U?
U 1 1 5D455944
P 5500 3550
AR Path="/5D455944" Ref="U?"  Part="1" 
AR Path="/5D4556E3/5D455944" Ref="U2"  Part="1" 
F 0 "U2" H 5500 3875 50  0000 C CNN
F 1 "MOC3052M" H 5500 3784 50  0000 C CNN
F 2 "Housings_DIP:DIP-6_W7.62mm" H 5300 3350 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/MO/MOC3052M.pdf" H 5475 3550 50  0001 L CNN
F 4 "MOC3052M" H -2200 2450 50  0001 C CNN "PartNumber"
	1    5500 3550
	1    0    0    -1  
$EndComp
$Comp
L device:Q_TRIAC_A1A2G D?
U 1 1 5D45594C
P 6700 3700
AR Path="/5D45594C" Ref="D?"  Part="1" 
AR Path="/5D4556E3/5D45594C" Ref="D2"  Part="1" 
F 0 "D2" H 6829 3746 50  0000 L CNN
F 1 "Q_TRIAC_A1A2G" H 6829 3655 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-280_Vertical" V 6775 3725 50  0001 C CNN
F 3 "https://www.st.com/content/ccc/resource/technical/document/datasheet/da/b9/e2/fa/76/a3/40/9b/CD00002264.pdf/files/CD00002264.pdf/jcr:content/translations/en.CD00002264.pdf" V 6700 3700 50  0001 C CNN
F 4 "BTA26-800BWRG" H -2200 2450 50  0001 C CNN "PartNumber"
	1    6700 3700
	1    0    0    -1  
$EndComp
$Comp
L device:R R?
U 1 1 5D455954
P 6450 3450
AR Path="/5D455954" Ref="R?"  Part="1" 
AR Path="/5D4556E3/5D455954" Ref="R4"  Part="1" 
F 0 "R4" V 6243 3450 50  0000 C CNN
F 1 "470R" V 6334 3450 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0414_L11.9mm_D4.5mm_P15.24mm_Horizontal" V 6380 3450 50  0001 C CNN
F 3 "~" H 6450 3450 50  0001 C CNN
F 4 "~" H -2200 2450 50  0001 C CNN "PartNumber"
	1    6450 3450
	0    1    1    0   
$EndComp
$Comp
L device:R R?
U 1 1 5D45595C
P 6050 3450
AR Path="/5D45595C" Ref="R?"  Part="1" 
AR Path="/5D4556E3/5D45595C" Ref="R3"  Part="1" 
F 0 "R3" V 5843 3450 50  0000 C CNN
F 1 "360R" V 5934 3450 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0414_L11.9mm_D4.5mm_P15.24mm_Horizontal" V 5980 3450 50  0001 C CNN
F 3 "~" H 6050 3450 50  0001 C CNN
F 4 "~" H -2200 2450 50  0001 C CNN "PartNumber"
	1    6050 3450
	0    1    1    0   
$EndComp
$Comp
L device:C C?
U 1 1 5D455964
P 6250 4050
AR Path="/5D455964" Ref="C?"  Part="1" 
AR Path="/5D4556E3/5D455964" Ref="C5"  Part="1" 
F 0 "C5" H 6365 4096 50  0000 L CNN
F 1 "0.047uF" H 6365 4005 50  0000 L CNN
F 2 "Capacitors_THT:C_Axial_L12.0mm_D6.5mm_P15.00mm_Horizontal" H 6288 3900 50  0001 C CNN
F 3 "~" H 6250 4050 50  0001 C CNN
F 4 "~" H -2200 2450 50  0001 C CNN "PartNumber"
	1    6250 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 3850 6700 4350
Wire Wire Line
	6700 4350 6250 4350
Wire Wire Line
	6250 4350 6250 4200
Wire Wire Line
	6700 3550 6700 3450
Wire Wire Line
	6700 3450 6600 3450
Wire Wire Line
	6300 3450 6250 3450
Wire Wire Line
	6250 3450 6250 3900
Connection ~ 6250 3450
Wire Wire Line
	6250 3450 6200 3450
Wire Wire Line
	5900 3450 5800 3450
Wire Wire Line
	6550 3800 6400 3800
Wire Wire Line
	6400 3800 6400 3650
Wire Wire Line
	6400 3650 5800 3650
$Comp
L device:R R?
U 1 1 5D455979
P 7600 3700
AR Path="/5D455979" Ref="R?"  Part="1" 
AR Path="/5D4556E3/5D455979" Ref="R5"  Part="1" 
F 0 "R5" H 7670 3746 50  0000 L CNN
F 1 "39R" H 7670 3655 50  0000 L CNN
F 2 "Resistors_THT:R_Axial_DIN0414_L11.9mm_D4.5mm_P15.24mm_Horizontal" V 7530 3700 50  0001 C CNN
F 3 "~" H 7600 3700 50  0001 C CNN
F 4 "~" H -2200 2450 50  0001 C CNN "PartNumber"
	1    7600 3700
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5D455981
P 7600 4050
AR Path="/5D455981" Ref="C?"  Part="1" 
AR Path="/5D4556E3/5D455981" Ref="C6"  Part="1" 
F 0 "C6" H 7715 4096 50  0000 L CNN
F 1 "0.1uF" H 7715 4005 50  0000 L CNN
F 2 "Capacitors_THT:C_Axial_L12.0mm_D6.5mm_P15.00mm_Horizontal" H 7638 3900 50  0001 C CNN
F 3 "~" H 7600 4050 50  0001 C CNN
F 4 "~" H -2200 2450 50  0001 C CNN "PartNumber"
	1    7600 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 4200 7600 4350
Wire Wire Line
	7600 4350 6700 4350
Connection ~ 6700 4350
Wire Wire Line
	7600 3850 7600 3900
Wire Wire Line
	7600 3550 7600 3450
Connection ~ 6700 3450
$Comp
L power:PRI_HI #PWR?
U 1 1 5D45598E
P 8000 3400
AR Path="/5D45598E" Ref="#PWR?"  Part="1" 
AR Path="/5D4556E3/5D45598E" Ref="#PWR013"  Part="1" 
F 0 "#PWR013" H 8000 3250 50  0001 C CNN
F 1 "PRI_HI" H 8017 3573 50  0000 C CNN
F 2 "" H 8000 3400 50  0001 C CNN
F 3 "" H 8000 3400 50  0001 C CNN
	1    8000 3400
	1    0    0    -1  
$EndComp
Text GLabel 7900 4350 2    50   Output ~ 0
MOT_+
Wire Wire Line
	7900 4350 7600 4350
Connection ~ 7600 4350
$Comp
L power:+5V #PWR?
U 1 1 5D455997
P 4800 3400
AR Path="/5D455997" Ref="#PWR?"  Part="1" 
AR Path="/5D4556E3/5D455997" Ref="#PWR011"  Part="1" 
F 0 "#PWR011" H 4800 3250 50  0001 C CNN
F 1 "+5V" H 4815 3573 50  0000 C CNN
F 2 "" H 4800 3400 50  0001 C CNN
F 3 "" H 4800 3400 50  0001 C CNN
	1    4800 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 3450 4800 3450
Wire Wire Line
	4800 3450 4800 3400
$Comp
L device:R R?
U 1 1 5D4559A0
P 4800 3850
AR Path="/5D4559A0" Ref="R?"  Part="1" 
AR Path="/5D4556E3/5D4559A0" Ref="R2"  Part="1" 
F 0 "R2" H 4730 3804 50  0000 R CNN
F 1 "220R" H 4730 3895 50  0000 R CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4730 3850 50  0001 C CNN
F 3 "~" H 4800 3850 50  0001 C CNN
F 4 "~" H -2450 2450 50  0001 C CNN "PartNumber"
	1    4800 3850
	-1   0    0    1   
$EndComp
Wire Wire Line
	4800 3650 4800 3700
$Comp
L power:GND #PWR?
U 1 1 5D4559A9
P 4800 4400
AR Path="/5D4559A9" Ref="#PWR?"  Part="1" 
AR Path="/5D4556E3/5D4559A9" Ref="#PWR012"  Part="1" 
F 0 "#PWR012" H 4800 4150 50  0001 C CNN
F 1 "GND" V 4805 4272 50  0000 R CNN
F 2 "" H 4800 4400 50  0001 C CNN
F 3 "" H 4800 4400 50  0001 C CNN
	1    4800 4400
	1    0    0    -1  
$EndComp
$Comp
L device:R R?
U 1 1 5D4559B0
P 4350 4200
AR Path="/5D4559B0" Ref="R?"  Part="1" 
AR Path="/5D4556E3/5D4559B0" Ref="R1"  Part="1" 
F 0 "R1" V 4557 4200 50  0000 C CNN
F 1 "10k" V 4466 4200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4280 4200 50  0001 C CNN
F 3 "~" H 4350 4200 50  0001 C CNN
F 4 "~" H -2450 2450 50  0001 C CNN "PartNumber"
	1    4350 4200
	0    -1   -1   0   
$EndComp
Text GLabel 4050 4200 0    50   Input ~ 0
MOT_EN
Wire Wire Line
	4050 4200 4200 4200
Wire Wire Line
	6700 3450 7600 3450
Wire Wire Line
	8000 3400 8000 3450
Wire Wire Line
	8000 3450 7600 3450
Connection ~ 7600 3450
$Comp
L device:Q_NMOS_GSD Q?
U 1 1 5D4559BE
P 4700 4200
AR Path="/5D4559BE" Ref="Q?"  Part="1" 
AR Path="/5D4556E3/5D4559BE" Ref="Q1"  Part="1" 
F 0 "Q1" H 4905 4246 50  0000 L CNN
F 1 "Q_NMOS_GSD" H 4905 4155 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-323_SC-70" H 4900 4300 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/BSS138PW.pdf" H 4700 4200 50  0001 C CNN
F 4 "BSS138PW,115" H -2450 2450 50  0001 C CNN "PartNumber"
	1    4700 4200
	1    0    0    -1  
$EndComp
Wire Notes Line
	8600 4700 8600 2500
Wire Notes Line
	8600 2500 5450 2500
Wire Notes Line
	5450 2500 5450 4700
Wire Notes Line
	5450 4700 8600 4700
Wire Wire Line
	4800 3650 5200 3650
Text Notes 5450 2450 0    98   ~ 0
HIGH VOLTAGE
Text Notes 4550 1150 0    197  ~ 0
TRIAC DRIVER
$EndSCHEMATC
