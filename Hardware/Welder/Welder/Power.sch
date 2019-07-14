EESchema Schematic File Version 4
LIBS:Welder-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 8
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
L Transformer:TRANSF6 TR?
U 1 1 5D445499
P 4050 3350
AR Path="/5D445499" Ref="TR?"  Part="1" 
AR Path="/5D43ED05/5D445499" Ref="TR1"  Part="1" 
F 0 "TR1" H 4050 3832 50  0000 C CNN
F 1 "TRANSF6" H 4050 3741 50  0000 C CNN
F 2 "Transformers_THT:Transformer_Breve_TEZ-22x24" H 4050 3350 50  0001 C CNN
F 3 "http://catalog.triadmagnetics.com/Asset/FS20-120-C2.pdf" H 4050 3350 50  0001 C CNN
F 4 "FS20-120-C2" H 2400 2150 50  0001 C CNN "PartNumber"
	1    4050 3350
	1    0    0    -1  
$EndComp
$Comp
L device:D_Bridge_+-AA D?
U 1 1 5D4454A1
P 5050 3300
AR Path="/5D4454A1" Ref="D?"  Part="1" 
AR Path="/5D43ED05/5D4454A1" Ref="D1"  Part="1" 
F 0 "D1" H 5391 3346 50  0000 L CNN
F 1 "D_Bridge_+-AA" H 5391 3255 50  0000 L CNN
F 2 "Diodes_THT:Diode_Bridge_DIP-4_W7.62mm_P5.08mm" H 5050 3300 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/ds21201.pdf" H 5050 3300 50  0001 C CNN
F 4 "DF04M" H 5050 3300 50  0001 C CNN "PartNumber"
	1    5050 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 3550 4950 3650
Wire Wire Line
	4950 3650 5050 3650
Wire Wire Line
	5050 3650 5050 3600
Wire Wire Line
	5050 3000 5050 2950
Wire Wire Line
	5050 2950 4950 2950
Wire Wire Line
	4950 2950 4950 3050
$Comp
L power:GND #PWR?
U 1 1 5D4454AE
P 4750 3300
AR Path="/5D4454AE" Ref="#PWR?"  Part="1" 
AR Path="/5D43ED05/5D4454AE" Ref="#PWR04"  Part="1" 
F 0 "#PWR04" H 4750 3050 50  0001 C CNN
F 1 "GND" V 4850 3300 50  0000 R CNN
F 2 "" H 4750 3300 50  0001 C CNN
F 3 "" H 4750 3300 50  0001 C CNN
	1    4750 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	4300 3250 4250 3250
Wire Wire Line
	4300 3250 4300 3350
Wire Wire Line
	4300 3350 4250 3350
Wire Wire Line
	3850 3250 3800 3250
Wire Wire Line
	3800 3250 3800 3300
Wire Wire Line
	3800 3350 3850 3350
$Comp
L Regulator_Linear:LM7805_TO220 U?
U 1 1 5D4454BB
P 6900 3300
AR Path="/5D4454BB" Ref="U?"  Part="1" 
AR Path="/5D43ED05/5D4454BB" Ref="U1"  Part="1" 
F 0 "U1" H 6900 3542 50  0000 C CNN
F 1 "LM7805_TO220" H 6900 3451 50  0000 C CNN
F 2 "TO_SOT_Packages_THT:TO-220-3_Vertical" H 6900 3525 50  0001 C CIN
F 3 "http://www.fairchildsemi.com/ds/LM/LM7805.pdf" H 6900 3250 50  0001 C CNN
F 4 "MC7805CTG" H 2550 2150 50  0001 C CNN "PartNumber"
	1    6900 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 3300 6400 3350
Connection ~ 6400 3300
Wire Wire Line
	6400 3300 6050 3300
$Comp
L power:GND #PWR?
U 1 1 5D4454C5
P 6900 3600
AR Path="/5D4454C5" Ref="#PWR?"  Part="1" 
AR Path="/5D43ED05/5D4454C5" Ref="#PWR07"  Part="1" 
F 0 "#PWR07" H 6900 3350 50  0001 C CNN
F 1 "GND" V 6905 3472 50  0000 R CNN
F 2 "" H 6900 3600 50  0001 C CNN
F 3 "" H 6900 3600 50  0001 C CNN
	1    6900 3600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D4454CB
P 6400 3650
AR Path="/5D4454CB" Ref="#PWR?"  Part="1" 
AR Path="/5D43ED05/5D4454CB" Ref="#PWR06"  Part="1" 
F 0 "#PWR06" H 6400 3400 50  0001 C CNN
F 1 "GND" V 6405 3522 50  0000 R CNN
F 2 "" H 6400 3650 50  0001 C CNN
F 3 "" H 6400 3650 50  0001 C CNN
	1    6400 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D4454D1
P 7400 3650
AR Path="/5D4454D1" Ref="#PWR?"  Part="1" 
AR Path="/5D43ED05/5D4454D1" Ref="#PWR08"  Part="1" 
F 0 "#PWR08" H 7400 3400 50  0001 C CNN
F 1 "GND" V 7405 3522 50  0000 R CNN
F 2 "" H 7400 3650 50  0001 C CNN
F 3 "" H 7400 3650 50  0001 C CNN
	1    7400 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 3300 6400 3300
Wire Wire Line
	7400 3350 7400 3300
Wire Wire Line
	7400 3300 7200 3300
$Comp
L power:+5V #PWR?
U 1 1 5D4454DA
P 7800 3250
AR Path="/5D4454DA" Ref="#PWR?"  Part="1" 
AR Path="/5D43ED05/5D4454DA" Ref="#PWR09"  Part="1" 
F 0 "#PWR09" H 7800 3100 50  0001 C CNN
F 1 "+5V" H 7815 3423 50  0000 C CNN
F 2 "" H 7800 3250 50  0001 C CNN
F 3 "" H 7800 3250 50  0001 C CNN
	1    7800 3250
	1    0    0    -1  
$EndComp
$Comp
L power:PRI_HI #PWR?
U 1 1 5D4454E0
P 3200 3050
AR Path="/5D4454E0" Ref="#PWR?"  Part="1" 
AR Path="/5D43ED05/5D4454E0" Ref="#PWR01"  Part="1" 
F 0 "#PWR01" H 3200 2900 50  0001 C CNN
F 1 "PRI_HI" H 3217 3223 50  0000 C CNN
F 2 "" H 3200 3050 50  0001 C CNN
F 3 "" H 3200 3050 50  0001 C CNN
	1    3200 3050
	1    0    0    -1  
$EndComp
$Comp
L power:PRI_LO #PWR?
U 1 1 5D4454E6
P 3200 3550
AR Path="/5D4454E6" Ref="#PWR?"  Part="1" 
AR Path="/5D43ED05/5D4454E6" Ref="#PWR02"  Part="1" 
F 0 "#PWR02" H 3200 3400 50  0001 C CNN
F 1 "PRI_LO" H 3217 3723 50  0000 C CNN
F 2 "" H 3200 3550 50  0001 C CNN
F 3 "" H 3200 3550 50  0001 C CNN
	1    3200 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 3300 7400 3300
Connection ~ 7400 3300
$Comp
L device:C C?
U 1 1 5D4454EF
P 6400 3500
AR Path="/5D4454EF" Ref="C?"  Part="1" 
AR Path="/5D43ED05/5D4454EF" Ref="C2"  Part="1" 
F 0 "C2" H 6515 3546 50  0000 L CNN
F 1 "10uF" H 6515 3455 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 6438 3350 50  0001 C CNN
F 3 "http://www.samsungsem.com/kr/support/product-search/mlcc/__icsFiles/afieldfile/2019/05/10/19-CL31A106MBHNNNE.pdf" H 6400 3500 50  0001 C CNN
F 4 "CL31A106MBHNNNE" H 2550 2150 50  0001 C CNN "PartNumber"
	1    6400 3500
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5D4454F7
P 7400 3500
AR Path="/5D4454F7" Ref="C?"  Part="1" 
AR Path="/5D43ED05/5D4454F7" Ref="C3"  Part="1" 
F 0 "C3" H 7515 3546 50  0000 L CNN
F 1 "10uF" H 7515 3455 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 7438 3350 50  0001 C CNN
F 3 "http://www.samsungsem.com/kr/support/product-search/mlcc/__icsFiles/afieldfile/2019/05/10/19-CL31A106MBHNNNE.pdf" H 7400 3500 50  0001 C CNN
F 4 "CL31A106MBHNNNE" H 2550 2150 50  0001 C CNN "PartNumber"
	1    7400 3500
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5D4454FF
P 7800 3500
AR Path="/5D4454FF" Ref="C?"  Part="1" 
AR Path="/5D43ED05/5D4454FF" Ref="C4"  Part="1" 
F 0 "C4" H 7915 3546 50  0000 L CNN
F 1 "10nF" H 7915 3455 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7838 3350 50  0001 C CNN
F 3 "~" H 7800 3500 50  0001 C CNN
F 4 "~" H 2550 2150 50  0001 C CNN "PartNumber"
	1    7800 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D445506
P 7800 3650
AR Path="/5D445506" Ref="#PWR?"  Part="1" 
AR Path="/5D43ED05/5D445506" Ref="#PWR010"  Part="1" 
F 0 "#PWR010" H 7800 3400 50  0001 C CNN
F 1 "GND" V 7805 3522 50  0000 R CNN
F 2 "" H 7800 3650 50  0001 C CNN
F 3 "" H 7800 3650 50  0001 C CNN
	1    7800 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 3350 7800 3300
Connection ~ 7800 3300
Wire Wire Line
	7800 3250 7800 3300
Wire Wire Line
	3200 3550 3850 3550
Wire Wire Line
	3200 3050 3850 3050
$Comp
L power:GND #PWR?
U 1 1 5D445511
P 3800 3300
AR Path="/5D445511" Ref="#PWR?"  Part="1" 
AR Path="/5D43ED05/5D445511" Ref="#PWR03"  Part="1" 
F 0 "#PWR03" H 3800 3050 50  0001 C CNN
F 1 "GND" V 3900 3300 50  0000 R CNN
F 2 "" H 3800 3300 50  0001 C CNN
F 3 "" H 3800 3300 50  0001 C CNN
	1    3800 3300
	0    1    1    0   
$EndComp
Connection ~ 3800 3300
Wire Wire Line
	3800 3300 3800 3350
Wire Wire Line
	4250 3550 4950 3550
Wire Wire Line
	4250 3050 4950 3050
$Comp
L device:CP C?
U 1 1 5D44551C
P 6050 3500
AR Path="/5D44551C" Ref="C?"  Part="1" 
AR Path="/5D43ED05/5D44551C" Ref="C1"  Part="1" 
F 0 "C1" H 6168 3546 50  0000 L CNN
F 1 "47u" H 6168 3455 50  0000 L CNN
F 2 "Capacitors_SMD:CP_Elec_6.3x7.7" H 6088 3350 50  0001 C CNN
F 3 "https://media.digikey.com/pdf/Data%20Sheets/Panasonic%20Electronic%20Components/S_Series,Type_V_Rev2018.pdf" H 6050 3500 50  0001 C CNN
F 4 "EEE-1HA470XP" H 6050 3500 50  0001 C CNN "PartNumber"
	1    6050 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 3300 6050 3350
Connection ~ 6050 3300
Wire Wire Line
	6050 3300 5350 3300
$Comp
L power:GND #PWR?
U 1 1 5D445526
P 6050 3650
AR Path="/5D445526" Ref="#PWR?"  Part="1" 
AR Path="/5D43ED05/5D445526" Ref="#PWR05"  Part="1" 
F 0 "#PWR05" H 6050 3400 50  0001 C CNN
F 1 "GND" V 6055 3522 50  0000 R CNN
F 2 "" H 6050 3650 50  0001 C CNN
F 3 "" H 6050 3650 50  0001 C CNN
	1    6050 3650
	1    0    0    -1  
$EndComp
Text Notes 4600 2200 0    197  ~ 0
Input Power
Text Notes 900  3400 0    197  ~ 0
120-240 Vac
Wire Notes Line
	4050 2450 4050 4400
Wire Notes Line
	4050 4400 1000 4400
Wire Notes Line
	1000 4400 1000 2450
Wire Notes Line
	1000 2450 4050 2450
Text Notes 1000 2400 0    98   ~ 0
HIGH VOLTAGE
$EndSCHEMATC
