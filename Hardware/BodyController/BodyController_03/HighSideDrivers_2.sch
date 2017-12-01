EESchema Schematic File Version 2
LIBS:BodyController_03-rescue
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:BodyController_03-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 11
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
L BTS50202EKAXUMA2 U10
U 1 1 59909DBC
P 4250 4750
AR Path="/5986882A/59909A84/59909DBC" Ref="U10"  Part="1" 
AR Path="/5986882A/59909DBC" Ref="U10"  Part="1" 
F 0 "U10" H 4600 3950 59  0000 C CNN
F 1 "BTS50202EKAXUMA2" H 4300 5450 59  0000 C CNN
F 2 "Footprints:PG_DSO_14_40_EP" H 4300 5450 59  0001 C CNN
F 3 "" H 4300 5450 59  0001 C CNN
	1    4250 4750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR074
U 1 1 59909DBD
P 3450 5500
F 0 "#PWR074" H 3450 5250 50  0001 C CNN
F 1 "GND" H 3450 5350 50  0000 C CNN
F 2 "" H 3450 5500 50  0001 C CNN
F 3 "" H 3450 5500 50  0001 C CNN
	1    3450 5500
	1    0    0    -1  
$EndComp
$Comp
L R R61
U 1 1 59909DBE
P 3150 5050
F 0 "R61" V 3230 5050 50  0000 C CNN
F 1 "4k7" V 3150 5050 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3080 5050 50  0001 C CNN
F 3 "" H 3150 5050 50  0001 C CNN
	1    3150 5050
	-1   0    0    1   
$EndComp
$Comp
L R R58
U 1 1 59909DBF
P 2850 4850
F 0 "R58" V 2930 4850 50  0000 C CNN
F 1 "10k" V 2850 4850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2780 4850 50  0001 C CNN
F 3 "" H 2850 4850 50  0001 C CNN
	1    2850 4850
	0    1    1    0   
$EndComp
$Comp
L R R57
U 1 1 59909DC0
P 2850 4650
F 0 "R57" V 2930 4650 50  0000 C CNN
F 1 "100R" V 2850 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2780 4650 50  0001 C CNN
F 3 "" H 2850 4650 50  0001 C CNN
	1    2850 4650
	0    1    1    0   
$EndComp
$Comp
L R R60
U 1 1 59909DC1
P 3150 4550
F 0 "R60" V 3230 4550 50  0000 C CNN
F 1 "100R" V 3150 4550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3080 4550 50  0001 C CNN
F 3 "" H 3150 4550 50  0001 C CNN
	1    3150 4550
	0    1    1    0   
$EndComp
$Comp
L R R56
U 1 1 59909DC2
P 2850 4300
F 0 "R56" V 2930 4300 50  0000 C CNN
F 1 "100R" V 2850 4300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2780 4300 50  0001 C CNN
F 3 "" H 2850 4300 50  0001 C CNN
	1    2850 4300
	0    1    1    0   
$EndComp
$Comp
L R R59
U 1 1 59909DC3
P 3150 4200
F 0 "R59" V 3230 4200 50  0000 C CNN
F 1 "100R" V 3150 4200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3080 4200 50  0001 C CNN
F 3 "" H 3150 4200 50  0001 C CNN
	1    3150 4200
	0    1    1    0   
$EndComp
$Comp
L GND #PWR075
U 1 1 59909DC4
P 3150 5300
F 0 "#PWR075" H 3150 5050 50  0001 C CNN
F 1 "GND" H 3150 5150 50  0000 C CNN
F 2 "" H 3150 5300 50  0001 C CNN
F 3 "" H 3150 5300 50  0001 C CNN
	1    3150 5300
	1    0    0    -1  
$EndComp
$Comp
L C C36
U 1 1 59909DC5
P 2600 5050
F 0 "C36" H 2625 5150 50  0000 L CNN
F 1 "10nF" H 2625 4950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2638 4900 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2X7R1H103K080AA.pdf" H 2600 5050 50  0001 C CNN
F 4 "CGA3E2X7R1H103K080AA" H 0   0   50  0001 C CNN "PartNumber"
	1    2600 5050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR076
U 1 1 59909DC6
P 2600 5300
F 0 "#PWR076" H 2600 5050 50  0001 C CNN
F 1 "GND" H 2600 5150 50  0000 C CNN
F 2 "" H 2600 5300 50  0001 C CNN
F 3 "" H 2600 5300 50  0001 C CNN
	1    2600 5300
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR077
U 1 1 59909DC7
P 5250 4050
F 0 "#PWR077" H 5250 3900 50  0001 C CNN
F 1 "+12V" H 5250 4190 50  0000 C CNN
F 2 "" H 5250 4050 50  0001 C CNN
F 3 "" H 5250 4050 50  0001 C CNN
	1    5250 4050
	1    0    0    -1  
$EndComp
$Comp
L D D6
U 1 1 5990AB5D
P 5050 5600
F 0 "D6" H 5050 5700 50  0000 C CNN
F 1 "D" H 5050 5500 50  0000 C CNN
F 2 "Diodes_SMD:D_SMB" H 5050 5600 50  0001 C CNN
F 3 "https://www.fairchildsemi.com/datasheets/S3/S3AB.pdf" H 5050 5600 50  0001 C CNN
F 4 "S3AB" H 0   0   50  0001 C CNN "PartNumber"
	1    5050 5600
	0    1    1    0   
$EndComp
$Comp
L D D7
U 1 1 5990AC2C
P 5300 5600
F 0 "D7" H 5300 5700 50  0000 C CNN
F 1 "D" H 5300 5500 50  0000 C CNN
F 2 "Diodes_SMD:D_SMB" H 5300 5600 50  0001 C CNN
F 3 "https://www.fairchildsemi.com/datasheets/S3/S3AB.pdf" H 5300 5600 50  0001 C CNN
F 4 "S3AB" H 0   0   50  0001 C CNN "PartNumber"
	1    5300 5600
	0    1    1    0   
$EndComp
$Comp
L GND #PWR078
U 1 1 5990AEF3
P 5050 5750
F 0 "#PWR078" H 5050 5500 50  0001 C CNN
F 1 "GND" H 5050 5600 50  0000 C CNN
F 2 "" H 5050 5750 50  0001 C CNN
F 3 "" H 5050 5750 50  0001 C CNN
	1    5050 5750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR079
U 1 1 5990AF43
P 5300 5750
F 0 "#PWR079" H 5300 5500 50  0001 C CNN
F 1 "GND" H 5300 5600 50  0000 C CNN
F 2 "" H 5300 5750 50  0001 C CNN
F 3 "" H 5300 5750 50  0001 C CNN
	1    5300 5750
	1    0    0    -1  
$EndComp
$Comp
L C C40
U 1 1 59913A6A
P 9250 1550
F 0 "C40" H 9275 1650 50  0000 L CNN
F 1 "10nF" H 9275 1450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 9288 1400 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2X7R1H103K080AA.pdf" H 9250 1550 50  0001 C CNN
F 4 "CGA3E2X7R1H103K080AA" H 0   0   50  0001 C CNN "PartNumber"
	1    9250 1550
	1    0    0    -1  
$EndComp
$Comp
L C C38
U 1 1 59913BA9
P 8350 1550
F 0 "C38" H 8375 1650 50  0000 L CNN
F 1 "10nF" H 8375 1450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8388 1400 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2X7R1H103K080AA.pdf" H 8350 1550 50  0001 C CNN
F 4 "CGA3E2X7R1H103K080AA" H 0   0   50  0001 C CNN "PartNumber"
	1    8350 1550
	1    0    0    -1  
$EndComp
$Comp
L R R64
U 1 1 59913C10
P 8800 1350
F 0 "R64" V 8880 1350 50  0000 C CNN
F 1 "10k" V 8800 1350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8730 1350 50  0001 C CNN
F 3 "" H 8800 1350 50  0001 C CNN
	1    8800 1350
	0    1    1    0   
$EndComp
$Comp
L GND #PWR080
U 1 1 599148E7
P 8350 1750
F 0 "#PWR080" H 8350 1500 50  0001 C CNN
F 1 "GND" H 8350 1600 50  0000 C CNN
F 2 "" H 8350 1750 50  0001 C CNN
F 3 "" H 8350 1750 50  0001 C CNN
	1    8350 1750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR081
U 1 1 5991493E
P 9250 1700
F 0 "#PWR081" H 9250 1450 50  0001 C CNN
F 1 "GND" H 9250 1550 50  0000 C CNN
F 2 "" H 9250 1700 50  0001 C CNN
F 3 "" H 9250 1700 50  0001 C CNN
	1    9250 1700
	1    0    0    -1  
$EndComp
$Comp
L C C41
U 1 1 59915478
P 9300 2650
F 0 "C41" H 9325 2750 50  0000 L CNN
F 1 "10nF" H 9325 2550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 9338 2500 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2X7R1H103K080AA.pdf" H 9300 2650 50  0001 C CNN
F 4 "CGA3E2X7R1H103K080AA" H 0   0   50  0001 C CNN "PartNumber"
	1    9300 2650
	1    0    0    -1  
$EndComp
$Comp
L C C39
U 1 1 5991547E
P 8400 2650
F 0 "C39" H 8425 2750 50  0000 L CNN
F 1 "10nF" H 8425 2550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8438 2500 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2X7R1H103K080AA.pdf" H 8400 2650 50  0001 C CNN
F 4 "CGA3E2X7R1H103K080AA" H 0   0   50  0001 C CNN "PartNumber"
	1    8400 2650
	1    0    0    -1  
$EndComp
$Comp
L R R65
U 1 1 59915484
P 8850 2450
F 0 "R65" V 8930 2450 50  0000 C CNN
F 1 "10k" V 8850 2450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8780 2450 50  0001 C CNN
F 3 "" H 8850 2450 50  0001 C CNN
	1    8850 2450
	0    1    1    0   
$EndComp
$Comp
L GND #PWR082
U 1 1 5991548C
P 8400 2900
F 0 "#PWR082" H 8400 2650 50  0001 C CNN
F 1 "GND" H 8400 2750 50  0000 C CNN
F 2 "" H 8400 2900 50  0001 C CNN
F 3 "" H 8400 2900 50  0001 C CNN
	1    8400 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR083
U 1 1 59915493
P 9300 2800
F 0 "#PWR083" H 9300 2550 50  0001 C CNN
F 1 "GND" H 9300 2650 50  0000 C CNN
F 2 "" H 9300 2800 50  0001 C CNN
F 3 "" H 9300 2800 50  0001 C CNN
	1    9300 2800
	1    0    0    -1  
$EndComp
$Comp
L R R97
U 1 1 59979113
P 9500 1100
F 0 "R97" V 9580 1100 50  0000 C CNN
F 1 "10k" V 9500 1100 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9430 1100 50  0001 C CNN
F 3 "" H 9500 1100 50  0001 C CNN
	1    9500 1100
	-1   0    0    1   
$EndComp
$Comp
L R R98
U 1 1 59979189
P 9500 2250
F 0 "R98" V 9580 2250 50  0000 C CNN
F 1 "10k" V 9500 2250 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9430 2250 50  0001 C CNN
F 3 "" H 9500 2250 50  0001 C CNN
	1    9500 2250
	-1   0    0    1   
$EndComp
$Comp
L +3.3VP #PWR084
U 1 1 599A8FBF
P 9500 850
F 0 "#PWR084" H 9650 800 50  0001 C CNN
F 1 "+3.3VP" H 9500 950 50  0000 C CNN
F 2 "" H 9500 850 50  0001 C CNN
F 3 "" H 9500 850 50  0001 C CNN
	1    9500 850 
	1    0    0    -1  
$EndComp
$Comp
L +3.3VP #PWR085
U 1 1 599A901C
P 9500 2000
F 0 "#PWR085" H 9650 1950 50  0001 C CNN
F 1 "+3.3VP" H 9500 2100 50  0000 C CNN
F 2 "" H 9500 2000 50  0001 C CNN
F 3 "" H 9500 2000 50  0001 C CNN
	1    9500 2000
	1    0    0    -1  
$EndComp
$Comp
L C C63
U 1 1 599FFA4C
P 8850 3750
F 0 "C63" H 8875 3850 50  0000 L CNN
F 1 "10nF" H 8875 3650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8888 3600 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2X7R1H103K080AA.pdf" H 8850 3750 50  0001 C CNN
F 4 "CGA3E2X7R1H103K080AA" H 0   0   50  0001 C CNN "PartNumber"
	1    8850 3750
	1    0    0    -1  
$EndComp
$Comp
L R R121
U 1 1 599FFA52
P 8400 3550
F 0 "R121" V 8480 3550 50  0000 C CNN
F 1 "10k" V 8400 3550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8330 3550 50  0001 C CNN
F 3 "" H 8400 3550 50  0001 C CNN
	1    8400 3550
	0    1    1    0   
$EndComp
$Comp
L GND #PWR086
U 1 1 599FFA58
P 8850 3900
F 0 "#PWR086" H 8850 3650 50  0001 C CNN
F 1 "GND" H 8850 3750 50  0000 C CNN
F 2 "" H 8850 3900 50  0001 C CNN
F 3 "" H 8850 3900 50  0001 C CNN
	1    8850 3900
	1    0    0    -1  
$EndComp
$Comp
L C C65
U 1 1 599FFCCB
P 8850 5250
F 0 "C65" H 8875 5350 50  0000 L CNN
F 1 "10nF" H 8875 5150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8888 5100 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2X7R1H103K080AA.pdf" H 8850 5250 50  0001 C CNN
F 4 "CGA3E2X7R1H103K080AA" H 0   0   50  0001 C CNN "PartNumber"
	1    8850 5250
	1    0    0    -1  
$EndComp
$Comp
L R R123
U 1 1 599FFCD1
P 8400 5050
F 0 "R123" V 8480 5050 50  0000 C CNN
F 1 "10k" V 8400 5050 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8330 5050 50  0001 C CNN
F 3 "" H 8400 5050 50  0001 C CNN
	1    8400 5050
	0    1    1    0   
$EndComp
$Comp
L GND #PWR087
U 1 1 599FFCD7
P 8850 5400
F 0 "#PWR087" H 8850 5150 50  0001 C CNN
F 1 "GND" H 8850 5250 50  0000 C CNN
F 2 "" H 8850 5400 50  0001 C CNN
F 3 "" H 8850 5400 50  0001 C CNN
	1    8850 5400
	1    0    0    -1  
$EndComp
$Comp
L C C66
U 1 1 599FFD7C
P 8850 6000
F 0 "C66" H 8875 6100 50  0000 L CNN
F 1 "10nF" H 8875 5900 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8888 5850 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2X7R1H103K080AA.pdf" H 8850 6000 50  0001 C CNN
F 4 "CGA3E2X7R1H103K080AA" H 0   0   50  0001 C CNN "PartNumber"
	1    8850 6000
	1    0    0    -1  
$EndComp
$Comp
L R R124
U 1 1 599FFD82
P 8400 5800
F 0 "R124" V 8480 5800 50  0000 C CNN
F 1 "10k" V 8400 5800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8330 5800 50  0001 C CNN
F 3 "" H 8400 5800 50  0001 C CNN
	1    8400 5800
	0    1    1    0   
$EndComp
$Comp
L GND #PWR088
U 1 1 599FFD88
P 8850 6150
F 0 "#PWR088" H 8850 5900 50  0001 C CNN
F 1 "GND" H 8850 6000 50  0000 C CNN
F 2 "" H 8850 6150 50  0001 C CNN
F 3 "" H 8850 6150 50  0001 C CNN
	1    8850 6150
	1    0    0    -1  
$EndComp
$Comp
L BTS50202EKAXUMA2 U11
U 1 1 59A005A4
P 4200 2050
F 0 "U11" H 4550 1250 59  0000 C CNN
F 1 "BTS50202EKAXUMA2" H 4250 2750 59  0000 C CNN
F 2 "Footprints:PG_DSO_14_40_EP" H 4250 2750 59  0001 C CNN
F 3 "" H 4250 2750 59  0001 C CNN
	1    4200 2050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR089
U 1 1 59A005AA
P 3400 2800
F 0 "#PWR089" H 3400 2550 50  0001 C CNN
F 1 "GND" H 3400 2650 50  0000 C CNN
F 2 "" H 3400 2800 50  0001 C CNN
F 3 "" H 3400 2800 50  0001 C CNN
	1    3400 2800
	1    0    0    -1  
$EndComp
$Comp
L R R63
U 1 1 59A005B0
P 3100 2350
F 0 "R63" V 3180 2350 50  0000 C CNN
F 1 "4k7" V 3100 2350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3030 2350 50  0001 C CNN
F 3 "" H 3100 2350 50  0001 C CNN
	1    3100 2350
	-1   0    0    1   
$EndComp
$Comp
L R R22
U 1 1 59A005B6
P 2800 2150
F 0 "R22" V 2880 2150 50  0000 C CNN
F 1 "10k" V 2800 2150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2730 2150 50  0001 C CNN
F 3 "" H 2800 2150 50  0001 C CNN
	1    2800 2150
	0    1    1    0   
$EndComp
$Comp
L R R21
U 1 1 59A005BC
P 2800 1950
F 0 "R21" V 2880 1950 50  0000 C CNN
F 1 "100R" V 2800 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2730 1950 50  0001 C CNN
F 3 "" H 2800 1950 50  0001 C CNN
	1    2800 1950
	0    1    1    0   
$EndComp
$Comp
L R R62
U 1 1 59A005C2
P 3100 1850
F 0 "R62" V 3180 1850 50  0000 C CNN
F 1 "100R" V 3100 1850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3030 1850 50  0001 C CNN
F 3 "" H 3100 1850 50  0001 C CNN
	1    3100 1850
	0    1    1    0   
$EndComp
$Comp
L R R20
U 1 1 59A005C8
P 2800 1600
F 0 "R20" V 2880 1600 50  0000 C CNN
F 1 "100R" V 2800 1600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2730 1600 50  0001 C CNN
F 3 "" H 2800 1600 50  0001 C CNN
	1    2800 1600
	0    1    1    0   
$EndComp
$Comp
L R R55
U 1 1 59A005CE
P 3100 1500
F 0 "R55" V 3180 1500 50  0000 C CNN
F 1 "100R" V 3100 1500 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3030 1500 50  0001 C CNN
F 3 "" H 3100 1500 50  0001 C CNN
	1    3100 1500
	0    1    1    0   
$EndComp
$Comp
L GND #PWR090
U 1 1 59A005D4
P 3100 2600
F 0 "#PWR090" H 3100 2350 50  0001 C CNN
F 1 "GND" H 3100 2450 50  0000 C CNN
F 2 "" H 3100 2600 50  0001 C CNN
F 3 "" H 3100 2600 50  0001 C CNN
	1    3100 2600
	1    0    0    -1  
$EndComp
$Comp
L C C62
U 1 1 59A005DA
P 2550 2350
F 0 "C62" H 2575 2450 50  0000 L CNN
F 1 "10nF" H 2575 2250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2588 2200 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2X7R1H103K080AA.pdf" H 2550 2350 50  0001 C CNN
F 4 "CGA3E2X7R1H103K080AA" H 0   0   50  0001 C CNN "PartNumber"
	1    2550 2350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR091
U 1 1 59A005E0
P 2550 2600
F 0 "#PWR091" H 2550 2350 50  0001 C CNN
F 1 "GND" H 2550 2450 50  0000 C CNN
F 2 "" H 2550 2600 50  0001 C CNN
F 3 "" H 2550 2600 50  0001 C CNN
	1    2550 2600
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR092
U 1 1 59A005E6
P 5200 1350
F 0 "#PWR092" H 5200 1200 50  0001 C CNN
F 1 "+12V" H 5200 1490 50  0000 C CNN
F 2 "" H 5200 1350 50  0001 C CNN
F 3 "" H 5200 1350 50  0001 C CNN
	1    5200 1350
	1    0    0    -1  
$EndComp
$Comp
L D D18
U 1 1 59A005EC
P 5000 2900
F 0 "D18" H 5000 3000 50  0000 C CNN
F 1 "D" H 5000 2800 50  0000 C CNN
F 2 "Diodes_SMD:D_SMB" H 5000 2900 50  0001 C CNN
F 3 "https://www.fairchildsemi.com/datasheets/S3/S3AB.pdf" H 5000 2900 50  0001 C CNN
F 4 "S3AB" H 0   0   50  0001 C CNN "PartNumber"
	1    5000 2900
	0    1    1    0   
$EndComp
$Comp
L D D19
U 1 1 59A005F2
P 5250 2900
F 0 "D19" H 5250 3000 50  0000 C CNN
F 1 "D" H 5250 2800 50  0000 C CNN
F 2 "Diodes_SMD:D_SMB" H 5250 2900 50  0001 C CNN
F 3 "https://www.fairchildsemi.com/datasheets/S3/S3AB.pdf" H 5250 2900 50  0001 C CNN
F 4 "S3AB" H 0   0   50  0001 C CNN "PartNumber"
	1    5250 2900
	0    1    1    0   
$EndComp
$Comp
L GND #PWR093
U 1 1 59A005F8
P 5000 3050
F 0 "#PWR093" H 5000 2800 50  0001 C CNN
F 1 "GND" H 5000 2900 50  0000 C CNN
F 2 "" H 5000 3050 50  0001 C CNN
F 3 "" H 5000 3050 50  0001 C CNN
	1    5000 3050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR094
U 1 1 59A005FE
P 5250 3050
F 0 "#PWR094" H 5250 2800 50  0001 C CNN
F 1 "GND" H 5250 2900 50  0000 C CNN
F 2 "" H 5250 3050 50  0001 C CNN
F 3 "" H 5250 3050 50  0001 C CNN
	1    5250 3050
	1    0    0    -1  
$EndComp
$Comp
L C C64
U 1 1 59A013EB
P 8850 4500
F 0 "C64" H 8875 4600 50  0000 L CNN
F 1 "10nF" H 8875 4400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8888 4350 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2X7R1H103K080AA.pdf" H 8850 4500 50  0001 C CNN
F 4 "CGA3E2X7R1H103K080AA" H 0   0   50  0001 C CNN "PartNumber"
	1    8850 4500
	1    0    0    -1  
$EndComp
$Comp
L R R122
U 1 1 59A013F1
P 8400 4300
F 0 "R122" V 8480 4300 50  0000 C CNN
F 1 "10k" V 8400 4300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8330 4300 50  0001 C CNN
F 3 "" H 8400 4300 50  0001 C CNN
	1    8400 4300
	0    1    1    0   
$EndComp
$Comp
L GND #PWR095
U 1 1 59A013F7
P 8850 4650
F 0 "#PWR095" H 8850 4400 50  0001 C CNN
F 1 "GND" H 8850 4500 50  0000 C CNN
F 2 "" H 8850 4650 50  0001 C CNN
F 3 "" H 8850 4650 50  0001 C CNN
	1    8850 4650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR096
U 1 1 59B327F1
P 9600 1600
F 0 "#PWR096" H 9600 1350 50  0001 C CNN
F 1 "GND" H 9600 1450 50  0000 C CNN
F 2 "" H 9600 1600 50  0001 C CNN
F 3 "" H 9600 1600 50  0001 C CNN
	1    9600 1600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR097
U 1 1 59B32E08
P 9650 2700
F 0 "#PWR097" H 9650 2450 50  0001 C CNN
F 1 "GND" H 9650 2550 50  0000 C CNN
F 2 "" H 9650 2700 50  0001 C CNN
F 3 "" H 9650 2700 50  0001 C CNN
	1    9650 2700
	1    0    0    -1  
$EndComp
Text GLabel 2500 4200 0    59   Input ~ 0
PUMP_1_EN
Text GLabel 2500 4300 0    59   Input ~ 0
PUMP_2_EN
Text GLabel 2500 4550 0    59   Input ~ 0
DIAG_EN
Text GLabel 2500 4650 0    59   Input ~ 0
DIAG_SELECT
Text GLabel 2500 4850 0    59   Output ~ 0
PUMP_ISENSE
Text GLabel 5400 4650 2    59   Output ~ 0
PUMP_1_OUT
Text GLabel 5400 5100 2    59   Output ~ 0
PUMP_2_OUT
Text Notes 2200 4000 0    59   ~ 0
PUMP OUTPUT\n5A Rated Current (each)\nEfuse 50A
Text GLabel 9650 1350 2    60   Output ~ 0
RADIATOR_OUTPUT_TEMP_OUT
Text GLabel 8050 1350 0    60   Output ~ 0
RADIATOR_OUTPUT_TEMP_AI
Text GLabel 9700 2450 2    60   Output ~ 0
RADIATOR_INPUT_TEMP_OUT
Text GLabel 8100 2450 0    60   Output ~ 0
RADIATOR_INPUT_TEMP_AI
Text GLabel 9250 3550 2    60   Output ~ 0
FAN_SPEED_1_OUT
Text GLabel 2450 1500 0    59   Input ~ 0
FAN_1_EN
Text GLabel 2450 1600 0    59   Input ~ 0
FAN_2_EN
Text GLabel 2450 1850 0    59   Input ~ 0
DIAG_EN
Text GLabel 2450 1950 0    59   Input ~ 0
DIAG_SELECT
Text GLabel 2450 2150 0    59   Output ~ 0
FAN_ISENSE
Text GLabel 5350 1950 2    59   Output ~ 0
FAN_1_OUT
Text GLabel 5350 2400 2    59   Output ~ 0
FAN_2_OUT
Text Notes 2150 1300 0    59   ~ 0
FAN OUTPUT\n5A Rated Current (each)\nEfuse 50A
Text GLabel 9250 4300 2    60   Output ~ 0
FAN_SPEED_2_OUT
Text GLabel 9250 5050 2    60   Output ~ 0
PUMP_SPEED_1_OUT
Text GLabel 9250 5800 2    60   Output ~ 0
PUMP_SPEED_2_OUT
Text GLabel 8050 3550 0    60   Output ~ 0
FAN_SPEED_1_PWM
Text GLabel 8050 4300 0    60   Output ~ 0
FAN_SPEED_2_PWM
Text GLabel 8050 5050 0    60   Output ~ 0
PUMP_SPEED_1_PWM
Text GLabel 8050 5800 0    60   Output ~ 0
PUMP_SPEED_2_PWM
Text GLabel 9750 1550 2    60   Output ~ 0
RADIATOR_OUTPUT_TEMP_RTN
Text GLabel 9800 2650 2    60   Output ~ 0
RADIATOR_INPUT_TEMP_RTN
Wire Wire Line
	3450 5400 3450 5500
Wire Wire Line
	3550 5400 3450 5400
Wire Wire Line
	3550 4200 3300 4200
Wire Wire Line
	3550 4300 3000 4300
Wire Wire Line
	3550 4550 3300 4550
Wire Wire Line
	3550 4650 3000 4650
Wire Wire Line
	3000 4200 2500 4200
Wire Wire Line
	2700 4300 2500 4300
Wire Wire Line
	3000 4550 2500 4550
Wire Wire Line
	2700 4650 2500 4650
Wire Wire Line
	2500 4850 2700 4850
Wire Wire Line
	3000 4850 3550 4850
Wire Wire Line
	3150 4900 3150 4850
Wire Wire Line
	3150 5300 3150 5200
Wire Wire Line
	2600 4900 2600 4850
Wire Wire Line
	2600 5300 2600 5200
Wire Wire Line
	5250 4050 5250 4250
Wire Wire Line
	5250 4250 4950 4250
Wire Wire Line
	4950 4650 4950 4850
Wire Wire Line
	4950 4650 5400 4650
Wire Wire Line
	4950 5100 4950 5300
Wire Wire Line
	4950 5100 5400 5100
Wire Wire Line
	5050 5450 5050 5100
Wire Wire Line
	5300 5450 5300 4650
Wire Wire Line
	9650 1350 8950 1350
Wire Wire Line
	8650 1350 8050 1350
Wire Wire Line
	8350 1700 8350 1750
Wire Wire Line
	9700 2450 9000 2450
Wire Wire Line
	8700 2450 8100 2450
Wire Wire Line
	8400 2800 8400 2900
Wire Wire Line
	8350 1400 8350 1350
Wire Wire Line
	8400 2500 8400 2450
Wire Wire Line
	9250 1400 9250 1350
Wire Wire Line
	9300 2500 9300 2450
Wire Wire Line
	9500 2450 9500 2400
Wire Wire Line
	9500 2100 9500 2000
Wire Wire Line
	9500 1250 9500 1350
Wire Wire Line
	9500 850  9500 950 
Wire Wire Line
	9250 3550 8550 3550
Wire Wire Line
	8850 3600 8850 3550
Wire Wire Line
	9250 5050 8550 5050
Wire Wire Line
	8850 5100 8850 5050
Wire Wire Line
	9250 5800 8550 5800
Wire Wire Line
	8850 5850 8850 5800
Wire Wire Line
	3400 2700 3400 2800
Wire Wire Line
	3500 2700 3400 2700
Wire Wire Line
	3500 1500 3250 1500
Wire Wire Line
	3500 1600 2950 1600
Wire Wire Line
	3500 1850 3250 1850
Wire Wire Line
	3500 1950 2950 1950
Wire Wire Line
	2950 1500 2450 1500
Wire Wire Line
	2650 1600 2450 1600
Wire Wire Line
	2950 1850 2450 1850
Wire Wire Line
	2650 1950 2450 1950
Wire Wire Line
	2450 2150 2650 2150
Wire Wire Line
	2950 2150 3500 2150
Wire Wire Line
	3100 2200 3100 2150
Wire Wire Line
	3100 2600 3100 2500
Wire Wire Line
	2550 2200 2550 2150
Wire Wire Line
	2550 2600 2550 2500
Wire Wire Line
	5200 1350 5200 1550
Wire Wire Line
	5200 1550 4900 1550
Wire Wire Line
	4900 1950 4900 2150
Wire Wire Line
	4900 1950 5350 1950
Wire Wire Line
	4900 2400 4900 2600
Wire Wire Line
	4900 2400 5350 2400
Wire Wire Line
	5000 2750 5000 2400
Wire Wire Line
	5250 2750 5250 1950
Wire Wire Line
	9250 4300 8550 4300
Wire Wire Line
	8850 4350 8850 4300
Wire Wire Line
	8250 3550 8050 3550
Wire Wire Line
	8250 4300 8050 4300
Wire Wire Line
	8250 5050 8050 5050
Wire Wire Line
	8250 5800 8050 5800
Wire Wire Line
	9750 1550 9600 1550
Wire Wire Line
	9600 1550 9600 1600
Wire Wire Line
	9800 2650 9650 2650
Wire Wire Line
	9650 2650 9650 2700
Connection ~ 3150 4850
Connection ~ 2600 4850
Connection ~ 4950 4750
Connection ~ 4950 4650
Connection ~ 4950 5100
Connection ~ 4950 5200
Connection ~ 5050 5100
Connection ~ 5300 4650
Connection ~ 8350 1350
Connection ~ 8400 2450
Connection ~ 9250 1350
Connection ~ 9300 2450
Connection ~ 9500 2450
Connection ~ 9500 1350
Connection ~ 8850 3550
Connection ~ 8850 5050
Connection ~ 8850 5800
Connection ~ 3100 2150
Connection ~ 2550 2150
Connection ~ 4900 2050
Connection ~ 4900 1950
Connection ~ 4900 2400
Connection ~ 4900 2500
Connection ~ 5000 2400
Connection ~ 5250 1950
Connection ~ 8850 4300
$EndSCHEMATC
