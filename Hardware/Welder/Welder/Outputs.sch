EESchema Schematic File Version 4
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 8 8
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4100 3500 4100 3750
Wire Wire Line
	3800 3500 4100 3500
Wire Wire Line
	3800 3400 3800 3500
Wire Wire Line
	4100 3000 4100 2900
Wire Wire Line
	3800 3000 3800 3100
Wire Wire Line
	4100 3000 3800 3000
$Comp
L device:D D?
U 1 1 5D47031D
P 3800 3250
AR Path="/5D47031D" Ref="D?"  Part="1" 
AR Path="/5D470024/5D47031D" Ref="D13"  Part="1" 
F 0 "D13" V 3754 3329 50  0000 L CNN
F 1 "D" V 3845 3329 50  0000 L CNN
F 2 "Diodes_SMD:D_SOD-123F" H 3800 3250 50  0001 C CNN
F 3 "https://www.onsemi.com/pub/Collateral/NRVTS245ESF-D.PDF" H 3800 3250 50  0001 C CNN
F 4 "NRVTS245ESFT1G" V 3800 3250 50  0001 C CNN "PartNumber"
	1    3800 3250
	0    1    1    0   
$EndComp
Connection ~ 4100 3500
Wire Wire Line
	4100 3300 4100 3500
$Comp
L device:Q_NMOS_GSD Q?
U 1 1 5D470327
P 4000 3950
AR Path="/5D470327" Ref="Q?"  Part="1" 
AR Path="/5D470024/5D470327" Ref="Q2"  Part="1" 
F 0 "Q2" H 4205 3996 50  0000 L CNN
F 1 "Q_NMOS_GSD" H 4205 3905 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-323_SC-70" H 4200 4050 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/BSS138PW.pdf" H 4000 3950 50  0001 C CNN
F 4 "BSS138PW,115" H 4000 3950 50  0001 C CNN "PartNumber"
	1    4000 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 3000 7050 3000
Wire Wire Line
	7250 3150 7250 3000
Text GLabel 7050 3000 0    50   Input ~ 0
DEBUG_LED_EN
Wire Wire Line
	7250 3900 7250 4000
$Comp
L power:GND #PWR?
U 1 1 5D470332
P 7250 4000
AR Path="/5D470332" Ref="#PWR?"  Part="1" 
AR Path="/5D470024/5D470332" Ref="#PWR066"  Part="1" 
F 0 "#PWR066" H 7250 3750 50  0001 C CNN
F 1 "GND" V 7255 3872 50  0000 R CNN
F 2 "" H 7250 4000 50  0001 C CNN
F 3 "" H 7250 4000 50  0001 C CNN
	1    7250 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 3600 7250 3450
$Comp
L device:R R?
U 1 1 5D47033A
P 7250 3750
AR Path="/5D47033A" Ref="R?"  Part="1" 
AR Path="/5D470024/5D47033A" Ref="R28"  Part="1" 
F 0 "R28" H 7180 3704 50  0000 R CNN
F 1 "360R" H 7180 3795 50  0000 R CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 7180 3750 50  0001 C CNN
F 3 "~" H 7250 3750 50  0001 C CNN
F 4 "~" H -450 -2050 50  0001 C CNN "PartNumber"
	1    7250 3750
	-1   0    0    1   
$EndComp
$Comp
L device:LED D?
U 1 1 5D470342
P 7250 3300
AR Path="/5D470342" Ref="D?"  Part="1" 
AR Path="/5D470024/5D470342" Ref="D14"  Part="1" 
F 0 "D14" V 7288 3183 50  0000 R CNN
F 1 "LED" V 7197 3183 50  0000 R CNN
F 2 "LEDs:LED_0603_HandSoldering" H 7250 3300 50  0001 C CNN
F 3 "~" H 7250 3300 50  0001 C CNN
F 4 "~" H -450 -2050 50  0001 C CNN "PartNumber"
	1    7250 3300
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5D470349
P 7750 3150
AR Path="/5D470349" Ref="#PWR?"  Part="1" 
AR Path="/5D470024/5D470349" Ref="#PWR067"  Part="1" 
F 0 "#PWR067" H 7750 3000 50  0001 C CNN
F 1 "+5V" H 7765 3323 50  0000 C CNN
F 2 "" H 7750 3150 50  0001 C CNN
F 3 "" H 7750 3150 50  0001 C CNN
	1    7750 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 3900 7750 4000
$Comp
L power:GND #PWR?
U 1 1 5D470350
P 7750 4000
AR Path="/5D470350" Ref="#PWR?"  Part="1" 
AR Path="/5D470024/5D470350" Ref="#PWR068"  Part="1" 
F 0 "#PWR068" H 7750 3750 50  0001 C CNN
F 1 "GND" V 7755 3872 50  0000 R CNN
F 2 "" H 7750 4000 50  0001 C CNN
F 3 "" H 7750 4000 50  0001 C CNN
	1    7750 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 3600 7750 3450
$Comp
L device:R R?
U 1 1 5D470358
P 7750 3750
AR Path="/5D470358" Ref="R?"  Part="1" 
AR Path="/5D470024/5D470358" Ref="R29"  Part="1" 
F 0 "R29" H 7680 3704 50  0000 R CNN
F 1 "360R" H 7680 3795 50  0000 R CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 7680 3750 50  0001 C CNN
F 3 "~" H 7750 3750 50  0001 C CNN
F 4 "~" H -450 -2050 50  0001 C CNN "PartNumber"
	1    7750 3750
	-1   0    0    1   
$EndComp
$Comp
L device:LED D?
U 1 1 5D470360
P 7750 3300
AR Path="/5D470360" Ref="D?"  Part="1" 
AR Path="/5D470024/5D470360" Ref="D15"  Part="1" 
F 0 "D15" V 7788 3183 50  0000 R CNN
F 1 "LED" V 7697 3183 50  0000 R CNN
F 2 "LEDs:LED_0603_HandSoldering" H 7750 3300 50  0001 C CNN
F 3 "~" H 7750 3300 50  0001 C CNN
F 4 "~" H -450 -2050 50  0001 C CNN "PartNumber"
	1    7750 3300
	0    -1   -1   0   
$EndComp
Connection ~ 4100 3000
Wire Wire Line
	4100 3100 4100 3000
Wire Wire Line
	4150 3100 4100 3100
$Comp
L power:+5V #PWR?
U 1 1 5D47036A
P 4100 2900
AR Path="/5D47036A" Ref="#PWR?"  Part="1" 
AR Path="/5D470024/5D47036A" Ref="#PWR064"  Part="1" 
F 0 "#PWR064" H 4100 2750 50  0001 C CNN
F 1 "+5V" H 4115 3073 50  0000 C CNN
F 2 "" H 4100 2900 50  0001 C CNN
F 3 "" H 4100 2900 50  0001 C CNN
	1    4100 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 3300 4100 3300
Wire Wire Line
	3350 3950 3500 3950
Text GLabel 3350 3950 0    50   Input ~ 0
BUZZ_PWM
$Comp
L device:R R?
U 1 1 5D470374
P 3650 3950
AR Path="/5D470374" Ref="R?"  Part="1" 
AR Path="/5D470024/5D470374" Ref="R27"  Part="1" 
F 0 "R27" V 3857 3950 50  0000 C CNN
F 1 "10k" V 3766 3950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3580 3950 50  0001 C CNN
F 3 "~" H 3650 3950 50  0001 C CNN
F 4 "~" H -2500 -3150 50  0001 C CNN "PartNumber"
	1    3650 3950
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D47037B
P 4100 4150
AR Path="/5D47037B" Ref="#PWR?"  Part="1" 
AR Path="/5D470024/5D47037B" Ref="#PWR065"  Part="1" 
F 0 "#PWR065" H 4100 3900 50  0001 C CNN
F 1 "GND" V 4105 4022 50  0000 R CNN
F 2 "" H 4100 4150 50  0001 C CNN
F 3 "" H 4100 4150 50  0001 C CNN
	1    4100 4150
	1    0    0    -1  
$EndComp
$Comp
L device:Buzzer BZ?
U 1 1 5D470382
P 4250 3200
AR Path="/5D470382" Ref="BZ?"  Part="1" 
AR Path="/5D470024/5D470382" Ref="BZ1"  Part="1" 
F 0 "BZ1" H 4403 3229 50  0000 L CNN
F 1 "Buzzer" H 4403 3138 50  0000 L CNN
F 2 "Buzzers_Beepers:MagneticBuzzer_ProSignal_ABT-410-RC" V 4225 3300 50  0001 C CNN
F 3 "file:///C:/Users/kid%20group/Downloads/WT-1205-16-Feb-2019.pdf" V 4225 3300 50  0001 C CNN
F 4 "WT-1205" H -2500 -3150 50  0001 C CNN "PartNumber"
	1    4250 3200
	1    0    0    -1  
$EndComp
Text Notes 6950 2450 0    197  ~ 0
LEDs
Text Notes 3500 2500 0    197  ~ 0
BUZZER
$EndSCHEMATC
