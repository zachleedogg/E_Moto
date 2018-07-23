EESchema Schematic File Version 4
LIBS:DCDC_01-cache
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
L Regulator_Switching:LTC3638 U2
U 1 1 5B85874B
P 4600 2050
F 0 "U2" H 4300 1600 50  0000 C CNN
F 1 "LTC3638" H 4850 1600 50  0000 C CNN
F 2 "Package_SO:Linear_MSOP-12-16-1EP_3x4mm_P0.5mm" H 4650 1600 50  0001 L CNN
F 3 "http://www.analog.com/media/en/technical-documentation/data-sheets/3638fa.pdf" H 4600 1950 50  0001 C CNN
F 4 " " H 4600 2050 394 0000 C CNN "DNP"
	1    4600 2050
	1    0    0    -1  
$EndComp
$Comp
L power:+BATT #PWR?
U 1 1 5B858B6E
P 800 950
AR Path="/5B2FB0D8/5B858B6E" Ref="#PWR?"  Part="1" 
AR Path="/5B2FB036/5B858B6E" Ref="#PWR013"  Part="1" 
AR Path="/5C8FC872/5C8FC885/5B858B6E" Ref="#PWR013"  Part="1" 
F 0 "#PWR013" H 800 800 50  0001 C CNN
F 1 "+BATT" H 815 1123 50  0000 C CNN
F 2 "" H 800 950 50  0001 C CNN
F 3 "" H 800 950 50  0001 C CNN
	1    800  950 
	1    0    0    -1  
$EndComp
$Comp
L device:Ferrite_Bead L?
U 1 1 5B858B74
P 1100 1050
AR Path="/5B2FB0D8/5B858B74" Ref="L?"  Part="1" 
AR Path="/5B2FB036/5B858B74" Ref="L2"  Part="1" 
AR Path="/5C8FC872/5C8FC885/5B858B74" Ref="L2"  Part="1" 
F 0 "L2" V 826 1050 50  0000 C CNN
F 1 "Ferrite_Bead" V 917 1050 50  0000 C CNN
F 2 "Inductors_SMD:L_0603_HandSoldering" V 1030 1050 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/beads_automotive_signal_mmz1608_en.pdf" H 1100 1050 50  0001 C CNN
F 4 " " H -450 0   50  0001 C CNN "DNP"
F 5 "MMZ1608B121CTDH5" H -450 0   50  0001 C CNN "MPN"
	1    1100 1050
	0    1    1    0   
$EndComp
$Comp
L device:C C14
U 1 1 5B858BA1
P 4250 1200
F 0 "C14" H 4365 1246 50  0000 L CNN
F 1 "1uF" H 4365 1155 50  0000 L CNN
F 2 "" H 4288 1050 50  0001 C CNN
F 3 "~" H 4250 1200 50  0001 C CNN
F 4 " " H -100 0   50  0001 C CNN "DNP"
F 5 "250V" H 4450 1050 50  0000 C CNN "Voltage"
	1    4250 1200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR020
U 1 1 5B858C48
P 4250 1350
F 0 "#PWR020" H 4250 1100 50  0001 C CNN
F 1 "GND" H 4255 1177 50  0000 C CNN
F 2 "" H 4250 1350 50  0001 C CNN
F 3 "" H 4250 1350 50  0001 C CNN
	1    4250 1350
	1    0    0    -1  
$EndComp
$Comp
L device:C C16
U 1 1 5B858CCC
P 6750 2000
F 0 "C16" H 6865 2046 50  0000 L CNN
F 1 "10uF" H 6865 1955 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 6788 1850 50  0001 C CNN
F 3 "~" H 6750 2000 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "50V" H 0   0   50  0001 C CNN "Voltage"
	1    6750 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR024
U 1 1 5B858CD3
P 6750 2150
F 0 "#PWR024" H 6750 1900 50  0001 C CNN
F 1 "GND" H 6755 1977 50  0000 C CNN
F 2 "" H 6750 2150 50  0001 C CNN
F 3 "" H 6750 2150 50  0001 C CNN
	1    6750 2150
	1    0    0    -1  
$EndComp
$Comp
L device:C C17
U 1 1 5B858CFC
P 7150 2000
F 0 "C17" H 7265 2046 50  0000 L CNN
F 1 "10uF" H 7265 1955 50  0000 L CNN
F 2 "1206" H 7188 1850 50  0001 C CNN
F 3 "~" H 7150 2000 50  0001 C CNN
F 4 " " H 100 0   50  0001 C CNN "DNP"
F 5 "50V" H 100 0   50  0001 C CNN "Voltage"
	1    7150 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR025
U 1 1 5B858D03
P 7150 2150
F 0 "#PWR025" H 7150 1900 50  0001 C CNN
F 1 "GND" H 7155 1977 50  0000 C CNN
F 2 "" H 7150 2150 50  0001 C CNN
F 3 "" H 7150 2150 50  0001 C CNN
	1    7150 2150
	1    0    0    -1  
$EndComp
$Comp
L device:C C18
U 1 1 5B858D25
P 7550 2000
F 0 "C18" H 7665 2046 50  0000 L CNN
F 1 "100nF" H 7665 1955 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7588 1850 50  0001 C CNN
F 3 "~" H 7550 2000 50  0001 C CNN
F 4 " " H 200 0   50  0001 C CNN "DNP"
F 5 "50V" H 200 0   50  0001 C CNN "Voltage"
	1    7550 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR026
U 1 1 5B858D2C
P 7550 2150
F 0 "#PWR026" H 7550 1900 50  0001 C CNN
F 1 "GND" H 7555 1977 50  0000 C CNN
F 2 "" H 7550 2150 50  0001 C CNN
F 3 "" H 7550 2150 50  0001 C CNN
	1    7550 2150
	1    0    0    -1  
$EndComp
$Comp
L device:C C19
U 1 1 5B858D4B
P 8000 2000
F 0 "C19" H 8115 2046 50  0000 L CNN
F 1 "10nF" H 8115 1955 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8038 1850 50  0001 C CNN
F 3 "~" H 8000 2000 50  0001 C CNN
F 4 " " H 350 0   50  0001 C CNN "DNP"
F 5 "50V" H 350 0   50  0001 C CNN "Voltage"
	1    8000 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR027
U 1 1 5B858D52
P 8000 2150
F 0 "#PWR027" H 8000 1900 50  0001 C CNN
F 1 "GND" H 8005 1977 50  0000 C CNN
F 2 "" H 8000 2150 50  0001 C CNN
F 3 "" H 8000 2150 50  0001 C CNN
	1    8000 2150
	1    0    0    -1  
$EndComp
$Comp
L device:L L3
U 1 1 5B858D7F
P 6300 1850
F 0 "L3" V 6490 1850 50  0000 C CNN
F 1 "220u" V 6399 1850 50  0000 C CNN
F 2 "" H 6300 1850 50  0001 C CNN
F 3 "~" H 6300 1850 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
	1    6300 1850
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5B85927B
P 3400 2900
F 0 "#PWR016" H 3400 2650 50  0001 C CNN
F 1 "GND" H 3405 2727 50  0000 C CNN
F 2 "" H 3400 2900 50  0001 C CNN
F 3 "" H 3400 2900 50  0001 C CNN
	1    3400 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR021
U 1 1 5B859259
P 4600 2900
F 0 "#PWR021" H 4600 2650 50  0001 C CNN
F 1 "GND" H 4605 2727 50  0000 C CNN
F 2 "" H 4600 2900 50  0001 C CNN
F 3 "" H 4600 2900 50  0001 C CNN
	1    4600 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR019
U 1 1 5B85CA94
P 4200 2900
F 0 "#PWR019" H 4200 2650 50  0001 C CNN
F 1 "GND" H 4205 2727 50  0000 C CNN
F 2 "" H 4200 2900 50  0001 C CNN
F 3 "" H 4200 2900 50  0001 C CNN
	1    4200 2900
	1    0    0    -1  
$EndComp
$Comp
L device:C C13
U 1 1 5B85CEFC
P 3800 1200
F 0 "C13" H 3915 1246 50  0000 L CNN
F 1 "100nF" H 3915 1155 50  0000 L CNN
F 2 "" H 3838 1050 50  0001 C CNN
F 3 "~" H 3800 1200 50  0001 C CNN
F 4 " " H -250 0   50  0001 C CNN "DNP"
F 5 "250V" H 4000 1050 50  0000 C CNN "Voltage"
	1    3800 1200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR018
U 1 1 5B85CF03
P 3800 1350
F 0 "#PWR018" H 3800 1100 50  0001 C CNN
F 1 "GND" H 3805 1177 50  0000 C CNN
F 2 "" H 3800 1350 50  0001 C CNN
F 3 "" H 3800 1350 50  0001 C CNN
	1    3800 1350
	1    0    0    -1  
$EndComp
$Comp
L device:D_Schottky D1
U 1 1 5B85D611
P 5150 2700
F 0 "D1" V 5104 2779 50  0000 L CNN
F 1 "D_Schottky" V 5195 2779 50  0000 L CNN
F 2 "" H 5150 2700 50  0001 C CNN
F 3 "~" H 5150 2700 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
	1    5150 2700
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR022
U 1 1 5B85D688
P 5150 2900
F 0 "#PWR022" H 5150 2650 50  0001 C CNN
F 1 "GND" H 5155 2727 50  0000 C CNN
F 2 "" H 5150 2900 50  0001 C CNN
F 3 "" H 5150 2900 50  0001 C CNN
	1    5150 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR023
U 1 1 5B85E9B8
P 5800 2900
F 0 "#PWR023" H 5800 2650 50  0001 C CNN
F 1 "GND" H 5805 2727 50  0000 C CNN
F 2 "" H 5800 2900 50  0001 C CNN
F 3 "" H 5800 2900 50  0001 C CNN
	1    5800 2900
	1    0    0    -1  
$EndComp
$Comp
L device:C C15
U 1 1 5B85F387
P 5800 2450
F 0 "C15" H 5915 2496 50  0000 L CNN
F 1 "330pF" H 5915 2405 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5838 2300 50  0001 C CNN
F 3 "~" H 5800 2450 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "50V" H 0   0   50  0001 C CNN "Voltage"
	1    5800 2450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5B863E64
P 3200 2900
F 0 "#PWR015" H 3200 2650 50  0001 C CNN
F 1 "GND" H 3205 2727 50  0000 C CNN
F 2 "" H 3200 2900 50  0001 C CNN
F 3 "" H 3200 2900 50  0001 C CNN
	1    3200 2900
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR031
U 1 1 5B8656D7
P 8250 1850
F 0 "#PWR031" H 8250 1700 50  0001 C CNN
F 1 "+5V" H 8265 2023 50  0000 C CNN
F 2 "" H 8250 1850 50  0001 C CNN
F 3 "" H 8250 1850 50  0001 C CNN
	1    8250 1850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR028
U 1 1 5B865929
P 9800 2850
F 0 "#PWR028" H 9800 2600 50  0001 C CNN
F 1 "GND" H 9805 2677 50  0000 C CNN
F 2 "" H 9800 2850 50  0001 C CNN
F 3 "" H 9800 2850 50  0001 C CNN
	1    9800 2850
	1    0    0    -1  
$EndComp
$Comp
L device:C C20
U 1 1 5B86595C
P 10100 2700
F 0 "C20" H 10215 2746 50  0000 L CNN
F 1 "100nF" H 10215 2655 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 10138 2550 50  0001 C CNN
F 3 "~" H 10100 2700 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "50V" H 0   0   50  0001 C CNN "Voltage"
	1    10100 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR029
U 1 1 5B86627B
P 10100 2850
F 0 "#PWR029" H 10100 2600 50  0001 C CNN
F 1 "GND" H 10105 2677 50  0000 C CNN
F 2 "" H 10100 2850 50  0001 C CNN
F 3 "" H 10100 2850 50  0001 C CNN
	1    10100 2850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR030
U 1 1 5B873B5E
P 9000 2800
F 0 "#PWR030" H 9000 2550 50  0001 C CNN
F 1 "GND" H 9005 2627 50  0000 C CNN
F 2 "" H 9000 2800 50  0001 C CNN
F 3 "" H 9000 2800 50  0001 C CNN
	1    9000 2800
	-1   0    0    -1  
$EndComp
$Comp
L device:R R15
U 1 1 5B8774F7
P 8500 2600
F 0 "R15" V 8400 2600 50  0000 C CNN
F 1 "100R" V 8500 2600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8430 2600 50  0001 C CNN
F 3 "~" H 8500 2600 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
	1    8500 2600
	0    -1   1    0   
$EndComp
$Comp
L device:R R4
U 1 1 5B87BC21
P 2800 2000
F 0 "R4" V 2900 2000 50  0000 C CNN
F 1 "1M49" V 2800 2000 50  0000 C CNN
F 2 "" V 2730 2000 50  0001 C CNN
F 3 "~" H 2800 2000 50  0001 C CNN
F 4 "X" H 2800 2000 394 0000 C CNN "DNP"
	1    2800 2000
	0    -1   -1   0   
$EndComp
$Comp
L device:C C12
U 1 1 5B882234
P 3600 2400
F 0 "C12" H 3715 2446 50  0000 L CNN
F 1 "500nF" H 3715 2355 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 3638 2250 50  0001 C CNN
F 3 "~" H 3600 2400 50  0001 C CNN
F 4 " " H -250 -300 50  0001 C CNN "DNP"
F 5 "50V" H -250 -300 50  0001 C CNN "Voltage"
	1    3600 2400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR017
U 1 1 5B8873FE
P 3600 2900
F 0 "#PWR017" H 3600 2650 50  0001 C CNN
F 1 "GND" H 3605 2727 50  0000 C CNN
F 2 "" H 3600 2900 50  0001 C CNN
F 3 "" H 3600 2900 50  0001 C CNN
	1    3600 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR058
U 1 1 5BBFB5AD
P 3250 7500
F 0 "#PWR058" H 3250 7250 50  0001 C CNN
F 1 "GND" H 3255 7327 50  0000 C CNN
F 2 "" H 3250 7500 50  0001 C CNN
F 3 "" H 3250 7500 50  0001 C CNN
	1    3250 7500
	1    0    0    -1  
$EndComp
$Comp
L device:C C42
U 1 1 5BBFB5B3
P 4000 7350
F 0 "C42" H 4115 7396 50  0000 L CNN
F 1 "100nF" H 4115 7305 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4038 7200 50  0001 C CNN
F 3 "~" H 4000 7350 50  0001 C CNN
F 4 " " H -4950 2000 50  0001 C CNN "DNP"
F 5 "50V" H -4950 2000 50  0001 C CNN "Voltage"
	1    4000 7350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR059
U 1 1 5BBFB5BC
P 4000 7500
F 0 "#PWR059" H 4000 7250 50  0001 C CNN
F 1 "GND" H 4005 7327 50  0000 C CNN
F 2 "" H 4000 7500 50  0001 C CNN
F 3 "" H 4000 7500 50  0001 C CNN
	1    4000 7500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR060
U 1 1 5BBFB5E0
P 2400 7550
F 0 "#PWR060" H 2400 7300 50  0001 C CNN
F 1 "GND" H 2405 7377 50  0000 C CNN
F 2 "" H 2400 7550 50  0001 C CNN
F 3 "" H 2400 7550 50  0001 C CNN
	1    2400 7550
	-1   0    0    -1  
$EndComp
$Comp
L device:R R38
U 1 1 5BBFB5E7
P 1900 7350
F 0 "R38" V 1800 7350 50  0000 C CNN
F 1 "100R" V 1900 7350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1830 7350 50  0001 C CNN
F 3 "~" H 1900 7350 50  0001 C CNN
F 4 " " H -5400 2250 50  0001 C CNN "DNP"
	1    1900 7350
	0    -1   1    0   
$EndComp
$Comp
L power:+12V #PWR057
U 1 1 5BBFC9D6
P 1650 6350
F 0 "#PWR057" H 1650 6200 50  0001 C CNN
F 1 "+12V" H 1665 6523 50  0000 C CNN
F 2 "" H 1650 6350 50  0001 C CNN
F 3 "" H 1650 6350 50  0001 C CNN
	1    1650 6350
	-1   0    0    -1  
$EndComp
$Comp
L Regulator_Linear:TPS73633DBV U9
U 1 1 5C0DD63C
P 9400 5650
F 0 "U9" H 9400 5992 50  0000 C CNN
F 1 "TPS73633DBV" H 9400 5901 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 9400 5975 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/tps736.pdf" H 9400 5600 50  0001 C CNN
F 4 " " H 5000 -300 50  0001 C CNN "DNP"
	1    9400 5650
	1    0    0    -1  
$EndComp
$Comp
L device:C C74
U 1 1 5C0DD972
P 7850 5800
F 0 "C74" H 7965 5846 50  0000 L CNN
F 1 "10uF" H 7965 5755 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 7888 5650 50  0001 C CNN
F 3 "~" H 7850 5800 50  0001 C CNN
F 4 " " H 5000 -300 50  0001 C CNN "DNP"
F 5 "50V" H 5000 -300 50  0001 C CNN "Voltage"
	1    7850 5800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR099
U 1 1 5C0DD979
P 7850 5950
F 0 "#PWR099" H 7850 5700 50  0001 C CNN
F 1 "GND" H 7855 5777 50  0000 C CNN
F 2 "" H 7850 5950 50  0001 C CNN
F 3 "" H 7850 5950 50  0001 C CNN
	1    7850 5950
	1    0    0    -1  
$EndComp
$Comp
L device:R R74
U 1 1 5C0E1961
P 8850 5650
F 0 "R74" V 8750 5650 50  0000 C CNN
F 1 "100R" V 8850 5650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8780 5650 50  0001 C CNN
F 3 "~" H 8850 5650 50  0001 C CNN
F 4 " " H 5000 -300 50  0001 C CNN "DNP"
	1    8850 5650
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 5C0E9A13
P 9400 5950
F 0 "#PWR0103" H 9400 5700 50  0001 C CNN
F 1 "GND" H 9405 5777 50  0000 C CNN
F 2 "" H 9400 5950 50  0001 C CNN
F 3 "" H 9400 5950 50  0001 C CNN
	1    9400 5950
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR098
U 1 1 5C0E9BDA
P 7850 5550
F 0 "#PWR098" H 7850 5400 50  0001 C CNN
F 1 "+5V" H 7865 5723 50  0000 C CNN
F 2 "" H 7850 5550 50  0001 C CNN
F 3 "" H 7850 5550 50  0001 C CNN
	1    7850 5550
	1    0    0    -1  
$EndComp
$Comp
L device:C C77
U 1 1 5C0EA05B
P 10050 5850
F 0 "C77" H 10165 5896 50  0000 L CNN
F 1 "10uF" H 10165 5805 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 10088 5700 50  0001 C CNN
F 3 "~" H 10050 5850 50  0001 C CNN
F 4 " " H 5000 -300 50  0001 C CNN "DNP"
F 5 "50V" H 5000 -300 50  0001 C CNN "Voltage"
	1    10050 5850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 5C0EA062
P 10050 6000
F 0 "#PWR0107" H 10050 5750 50  0001 C CNN
F 1 "GND" H 10055 5827 50  0000 C CNN
F 2 "" H 10050 6000 50  0001 C CNN
F 3 "" H 10050 6000 50  0001 C CNN
	1    10050 6000
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3VP #PWR0106
U 1 1 5C0ECDD1
P 10050 5550
F 0 "#PWR0106" H 10200 5500 50  0001 C CNN
F 1 "+3.3VP" H 10070 5693 50  0000 C CNN
F 2 "" H 10050 5550 50  0001 C CNN
F 3 "" H 10050 5550 50  0001 C CNN
	1    10050 5550
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:TPS73633DBV U8
U 1 1 5C0ED038
P 9100 4150
F 0 "U8" H 9100 4492 50  0000 C CNN
F 1 "TPS73633DBV" H 9100 4401 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 9100 4475 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/tps736.pdf" H 9100 4100 50  0001 C CNN
F 4 " " H 5000 -300 50  0001 C CNN "DNP"
	1    9100 4150
	1    0    0    -1  
$EndComp
$Comp
L device:C C75
U 1 1 5C0ED03F
P 8150 4300
F 0 "C75" H 8265 4346 50  0000 L CNN
F 1 "10uF" H 8265 4255 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 8188 4150 50  0001 C CNN
F 3 "~" H 8150 4300 50  0001 C CNN
F 4 " " H 5000 -300 50  0001 C CNN "DNP"
F 5 "50V" H 5000 -300 50  0001 C CNN "Voltage"
	1    8150 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5C0ED046
P 8150 4450
F 0 "#PWR0101" H 8150 4200 50  0001 C CNN
F 1 "GND" H 8155 4277 50  0000 C CNN
F 2 "" H 8150 4450 50  0001 C CNN
F 3 "" H 8150 4450 50  0001 C CNN
	1    8150 4450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5C0ED057
P 9100 4450
F 0 "#PWR0102" H 9100 4200 50  0001 C CNN
F 1 "GND" H 9105 4277 50  0000 C CNN
F 2 "" H 9100 4450 50  0001 C CNN
F 3 "" H 9100 4450 50  0001 C CNN
	1    9100 4450
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0100
U 1 1 5C0ED05D
P 8150 4050
F 0 "#PWR0100" H 8150 3900 50  0001 C CNN
F 1 "+5V" H 8165 4223 50  0000 C CNN
F 2 "" H 8150 4050 50  0001 C CNN
F 3 "" H 8150 4050 50  0001 C CNN
	1    8150 4050
	1    0    0    -1  
$EndComp
$Comp
L device:C C76
U 1 1 5C0ED064
P 9750 4350
F 0 "C76" H 9865 4396 50  0000 L CNN
F 1 "10uF" H 9865 4305 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 9788 4200 50  0001 C CNN
F 3 "~" H 9750 4350 50  0001 C CNN
F 4 " " H 5000 -300 50  0001 C CNN "DNP"
F 5 "50V" H 5000 -300 50  0001 C CNN "Voltage"
	1    9750 4350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 5C0ED06B
P 9750 4500
F 0 "#PWR0105" H 9750 4250 50  0001 C CNN
F 1 "GND" H 9755 4327 50  0000 C CNN
F 2 "" H 9750 4500 50  0001 C CNN
F 3 "" H 9750 4500 50  0001 C CNN
	1    9750 4500
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0104
U 1 1 5C0F3B21
P 9750 4050
F 0 "#PWR0104" H 9750 3900 50  0001 C CNN
F 1 "+3V3" H 9765 4223 50  0000 C CNN
F 2 "" H 9750 4050 50  0001 C CNN
F 3 "" H 9750 4050 50  0001 C CNN
	1    9750 4050
	1    0    0    -1  
$EndComp
$Comp
L power:+5VP #PWR0109
U 1 1 5C16F887
P 10300 1850
F 0 "#PWR0109" H 10300 1700 50  0001 C CNN
F 1 "+5VP" H 10315 2023 50  0000 C CNN
F 2 "" H 10300 1850 50  0001 C CNN
F 3 "" H 10300 1850 50  0001 C CNN
	1    10300 1850
	1    0    0    -1  
$EndComp
$Comp
L power:+12P #PWR0113
U 1 1 5C18B5EC
P 3500 6350
F 0 "#PWR0113" H 3500 6200 50  0001 C CNN
F 1 "+12P" H 3515 6523 50  0000 C CNN
F 2 "" H 3500 6350 50  0001 C CNN
F 3 "" H 3500 6350 50  0001 C CNN
	1    3500 6350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0125
U 1 1 5C18FA7A
P 1950 2950
F 0 "#PWR0125" H 1950 2700 50  0001 C CNN
F 1 "GND" H 1955 2777 50  0000 C CNN
F 2 "" H 1950 2950 50  0001 C CNN
F 3 "" H 1950 2950 50  0001 C CNN
	1    1950 2950
	-1   0    0    -1  
$EndComp
$Comp
L device:R R79
U 1 1 5C18FA80
P 1500 2750
F 0 "R79" V 1400 2750 50  0000 C CNN
F 1 "100R" V 1500 2750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1430 2750 50  0001 C CNN
F 3 "~" H 1500 2750 50  0001 C CNN
F 4 "X" H 1500 2750 394 0000 C CNN "DNP"
	1    1500 2750
	0    -1   1    0   
$EndComp
$Comp
L device:R R81
U 1 1 5C19EF06
P 1950 1350
F 0 "R81" V 1850 1350 50  0000 C CNN
F 1 "10k" V 1950 1350 50  0000 C CNN
F 2 "" V 1880 1350 50  0001 C CNN
F 3 "~" H 1950 1350 50  0001 C CNN
F 4 "X" H 1950 1350 394 0000 C CNN "DNP"
	1    1950 1350
	1    0    0    -1  
$EndComp
$Comp
L device:R R80
U 1 1 5C19EF94
P 1950 2000
F 0 "R80" V 1850 2000 50  0000 C CNN
F 1 "100k" V 1950 2000 50  0000 C CNN
F 2 "" V 1880 2000 50  0001 C CNN
F 3 "~" H 1950 2000 50  0001 C CNN
F 4 "X" H 1950 2000 394 0000 C CNN "DNP"
	1    1950 2000
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5C227436
P 10000 1850
F 0 "#FLG0102" H 10000 1925 50  0001 C CNN
F 1 "PWR_FLAG" H 10000 2024 50  0000 C CNN
F 2 "" H 10000 1850 50  0001 C CNN
F 3 "~" H 10000 1850 50  0001 C CNN
	1    10000 1850
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0107
U 1 1 5C2275D7
P 2700 1050
F 0 "#FLG0107" H 2700 1125 50  0001 C CNN
F 1 "PWR_FLAG" H 2700 1224 50  0000 C CNN
F 2 "" H 2700 1050 50  0001 C CNN
F 3 "~" H 2700 1050 50  0001 C CNN
	1    2700 1050
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0108
U 1 1 5C227BDA
P 3200 6350
F 0 "#FLG0108" H 3200 6425 50  0001 C CNN
F 1 "PWR_FLAG" H 3200 6524 50  0000 C CNN
F 2 "" H 3200 6350 50  0001 C CNN
F 3 "~" H 3200 6350 50  0001 C CNN
	1    3200 6350
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0109
U 1 1 5C227C43
P 8650 1850
F 0 "#FLG0109" H 8650 1925 50  0001 C CNN
F 1 "PWR_FLAG" H 8650 2024 50  0000 C CNN
F 2 "" H 8650 1850 50  0001 C CNN
F 3 "~" H 8650 1850 50  0001 C CNN
	1    8650 1850
	1    0    0    -1  
$EndComp
$Comp
L device:Net-Tie_2 NT9
U 1 1 5C625CC7
P 6350 2250
F 0 "NT9" H 6350 2428 50  0000 C CNN
F 1 "Net-Tie_2" H 6350 2337 50  0000 C CNN
F 2 "" H 6350 2250 50  0001 C CNN
F 3 "~" H 6350 2250 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
	1    6350 2250
	1    0    0    -1  
$EndComp
Text GLabel 1350 2750 0    50   Input ~ 0
ENABLE_STBY_SUPPLY
Text GLabel 10450 2450 2    50   Output ~ 0
5V_MONITOR_AI
Text GLabel 8250 2600 0    50   Input ~ 0
SW_EN
Text GLabel 4350 7100 2    50   Output ~ 0
12V_MONITOR_AI
Text GLabel 1650 7350 0    50   Input ~ 0
SW_EN
Text GLabel 8600 5650 0    50   Input ~ 0
SW_EN
Text Notes 7600 5200 0    197  ~ 0
3V3_Switched_power
Text Notes 7950 3700 0    197  ~ 0
3V3_Always_On
Text Notes 6800 1150 0    197  ~ 0
5V_Always_On
Text Label 4600 1050 0    49   ~ 0
VBATT_FILT_5V_HV
Wire Wire Line
	950  1050 800  1050
Wire Wire Line
	800  1050 800  950 
Wire Wire Line
	4600 1050 4600 1550
Wire Wire Line
	6450 1850 6550 1850
Wire Wire Line
	6750 1850 7150 1850
Wire Wire Line
	7150 1850 7550 1850
Wire Wire Line
	4200 2250 3600 2250
Wire Wire Line
	3600 2250 3600 1950
Wire Wire Line
	3600 1950 4200 1950
Wire Wire Line
	4200 2050 3400 2050
Wire Wire Line
	3400 2050 3400 2900
Wire Wire Line
	6450 2250 6550 2250
Wire Wire Line
	6550 2250 6550 1850
Wire Wire Line
	6550 1850 6750 1850
Wire Wire Line
	4200 2350 4200 2500
Wire Wire Line
	4600 2550 4600 2900
Wire Wire Line
	4200 2850 4200 2900
Wire Wire Line
	5150 2850 5150 2900
Wire Wire Line
	5150 2550 5150 1850
Wire Wire Line
	5150 1850 5800 1850
Wire Wire Line
	5800 2300 5800 1850
Wire Wire Line
	5800 1850 6150 1850
Wire Wire Line
	3200 1100 3200 1050
Wire Wire Line
	3200 1750 4200 1750
Wire Wire Line
	3200 2750 3200 2900
Wire Wire Line
	10100 2450 10450 2450
Wire Wire Line
	8000 1850 8250 1850
Wire Wire Line
	9800 1850 9800 2000
Wire Wire Line
	9000 1850 9000 1900
Wire Wire Line
	9000 1850 9300 1850
Wire Wire Line
	9000 2400 9000 2350
Wire Wire Line
	8350 2600 8250 2600
Wire Wire Line
	2950 2000 3200 2000
Wire Wire Line
	3600 2900 3600 2550
Wire Wire Line
	2400 6800 2400 6750
Wire Wire Line
	2400 6850 2400 6800
Wire Wire Line
	1750 7350 1650 7350
Wire Wire Line
	9000 5550 7850 5550
Wire Wire Line
	7850 5550 7850 5650
Wire Wire Line
	8700 5650 8600 5650
Wire Wire Line
	10050 5550 9800 5550
Wire Wire Line
	10050 5550 10050 5700
Wire Wire Line
	8700 4050 8500 4050
Wire Wire Line
	8150 4050 8150 4150
Wire Wire Line
	9750 4050 9500 4050
Wire Wire Line
	9750 4050 9750 4200
Wire Wire Line
	8700 4150 8500 4150
Wire Wire Line
	8500 4150 8500 4050
Wire Wire Line
	8500 4050 8150 4050
Wire Wire Line
	9800 1850 9700 1850
Wire Wire Line
	9000 2350 9500 2350
Wire Wire Line
	9000 2200 9000 2350
Wire Wire Line
	8250 1850 8650 1850
Wire Wire Line
	10100 2550 10100 2450
Wire Wire Line
	10100 2450 9800 2450
Wire Wire Line
	9800 2450 9800 2300
Wire Wire Line
	9800 2450 9800 2550
Wire Wire Line
	9800 1850 10000 1850
Wire Wire Line
	1650 6350 2400 6350
Wire Wire Line
	2400 6450 2400 6350
Wire Wire Line
	2400 6350 2600 6350
Wire Wire Line
	2400 6800 2800 6800
Wire Wire Line
	3250 6350 3250 6650
Wire Wire Line
	3250 6350 3200 6350
Wire Wire Line
	3250 6350 3500 6350
Wire Wire Line
	1950 1800 1950 1850
Wire Wire Line
	10000 1850 10300 1850
Wire Wire Line
	3200 6350 3000 6350
Wire Wire Line
	8650 1850 9000 1850
Wire Wire Line
	5000 2250 6250 2250
Connection ~ 6750 1850
Connection ~ 7150 1850
Connection ~ 7550 1850
Connection ~ 6550 1850
Connection ~ 5150 1850
Connection ~ 5800 1850
Connection ~ 3200 1050
Connection ~ 9000 1850
Connection ~ 3200 1750
Connection ~ 3600 2250
Connection ~ 2400 6800
Connection ~ 7850 5550
Connection ~ 10050 5550
Connection ~ 8150 4050
Connection ~ 8500 4050
Connection ~ 9750 4050
Connection ~ 8250 1850
Connection ~ 9000 2350
Connection ~ 10100 2450
Connection ~ 9800 2450
Connection ~ 9800 1850
Connection ~ 2400 6350
Connection ~ 3250 6350
Connection ~ 1950 1800
Connection ~ 10000 1850
Connection ~ 2700 1050
Connection ~ 3200 6350
Connection ~ 8650 1850
NoConn ~ 9800 5650
NoConn ~ 9500 4150
$Comp
L device:R R5
U 1 1 5B526AEA
P 3200 2200
F 0 "R5" V 3100 2200 50  0000 C CNN
F 1 "32k" V 3200 2200 50  0000 C CNN
F 2 "" V 3130 2200 50  0001 C CNN
F 3 "~" H 3200 2200 50  0001 C CNN
F 4 " " H 1250 200 50  0001 C CNN "DNP"
	1    3200 2200
	1    0    0    -1  
$EndComp
$Comp
L device:R R7
U 1 1 5B529A26
P 3200 1250
F 0 "R7" V 3100 1250 50  0000 C CNN
F 1 "1M49" V 3200 1250 50  0000 C CNN
F 2 "" V 3130 1250 50  0001 C CNN
F 3 "~" H 3200 1250 50  0001 C CNN
F 4 " " H 1250 -750 50  0001 C CNN "DNP"
	1    3200 1250
	1    0    0    -1  
$EndComp
$Comp
L device:R R8
U 1 1 5B5327FC
P 3200 2600
F 0 "R8" V 3100 2600 50  0000 C CNN
F 1 "13k" V 3200 2600 50  0000 C CNN
F 2 "" V 3130 2600 50  0001 C CNN
F 3 "~" H 3200 2600 50  0001 C CNN
F 4 " " H 1250 600 50  0001 C CNN "DNP"
	1    3200 2600
	1    0    0    -1  
$EndComp
$Comp
L device:R R9
U 1 1 5B53572F
P 4200 2700
F 0 "R9" V 4300 2700 50  0000 C CNN
F 1 "176k" V 4200 2700 50  0000 C CNN
F 2 "" V 4130 2700 50  0001 C CNN
F 3 "~" H 4200 2700 50  0001 C CNN
F 4 " " H 4200 2700 394 0001 C CNN "DNP"
	1    4200 2700
	1    0    0    -1  
$EndComp
$Comp
L device:R R10
U 1 1 5B5389A5
P 5800 2750
F 0 "R10" V 5700 2750 50  0000 C CNN
F 1 "3R3" V 5800 2750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5730 2750 50  0001 C CNN
F 3 "~" H 5800 2750 50  0001 C CNN
F 4 " " H 3850 750 50  0001 C CNN "DNP"
	1    5800 2750
	1    0    0    -1  
$EndComp
$Comp
L device:R R35
U 1 1 5B53B93A
P 9000 2050
F 0 "R35" V 8900 2050 50  0000 C CNN
F 1 "100k" V 9000 2050 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8930 2050 50  0001 C CNN
F 3 "~" H 9000 2050 50  0001 C CNN
F 4 " " H 7050 50  50  0001 C CNN "DNP"
	1    9000 2050
	1    0    0    -1  
$EndComp
$Comp
L device:R R36
U 1 1 5B53E872
P 9800 2150
F 0 "R36" V 9700 2150 50  0000 C CNN
F 1 "10k" V 9800 2150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9730 2150 50  0001 C CNN
F 3 "~" H 9800 2150 50  0001 C CNN
F 4 " " H 7850 150 50  0001 C CNN "DNP"
	1    9800 2150
	1    0    0    -1  
$EndComp
$Comp
L device:R R37
U 1 1 5B5417A1
P 9800 2700
F 0 "R37" V 9700 2700 50  0000 C CNN
F 1 "10k" V 9800 2700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9730 2700 50  0001 C CNN
F 3 "~" H 9800 2700 50  0001 C CNN
F 4 " " H 7850 700 50  0001 C CNN "DNP"
	1    9800 2700
	1    0    0    -1  
$EndComp
$Comp
L device:R R12
U 1 1 5B5447F0
P 2400 6600
F 0 "R12" V 2300 6600 50  0000 C CNN
F 1 "100k" V 2400 6600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2330 6600 50  0001 C CNN
F 3 "~" H 2400 6600 50  0001 C CNN
F 4 " " H 450 4600 50  0001 C CNN "DNP"
	1    2400 6600
	1    0    0    -1  
$EndComp
$Comp
L device:R R13
U 1 1 5B547720
P 3250 6800
F 0 "R13" V 3150 6800 50  0000 C CNN
F 1 "47k" V 3250 6800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3180 6800 50  0001 C CNN
F 3 "~" H 3250 6800 50  0001 C CNN
F 4 " " H 1300 4800 50  0001 C CNN "DNP"
	1    3250 6800
	1    0    0    -1  
$EndComp
$Comp
L device:R R14
U 1 1 5B54A65B
P 3250 7350
F 0 "R14" V 3150 7350 50  0000 C CNN
F 1 "10k" V 3250 7350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3180 7350 50  0001 C CNN
F 3 "~" H 3250 7350 50  0001 C CNN
F 4 " " H 1300 5350 50  0001 C CNN "DNP"
	1    3250 7350
	1    0    0    -1  
$EndComp
Connection ~ 8000 1850
Wire Wire Line
	7550 1850 8000 1850
$Comp
L device:R R78
U 1 1 5B5D6CA3
P 2400 7000
F 0 "R78" V 2300 7000 50  0000 C CNN
F 1 "10k" V 2400 7000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2330 7000 50  0001 C CNN
F 3 "~" H 2400 7000 50  0001 C CNN
F 4 " " H 450 5000 50  0001 C CNN "DNP"
	1    2400 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 7100 3500 7100
Wire Wire Line
	3250 7200 3250 7100
Connection ~ 3250 7100
Wire Wire Line
	3250 7100 3250 6950
Wire Wire Line
	4000 7200 4000 7100
Connection ~ 4000 7100
Wire Wire Line
	4000 7100 4350 7100
Connection ~ 3800 1050
Wire Wire Line
	3800 1050 3200 1050
Connection ~ 4250 1050
Wire Wire Line
	4250 1050 4600 1050
Wire Wire Line
	3800 1050 4250 1050
$Comp
L device:D_Zener D9
U 1 1 5B4AE5CB
P 1350 1350
F 0 "D9" V 1300 1450 50  0000 L CNN
F 1 "15V" V 1400 1450 50  0000 L CNN
F 2 "" H 1350 1350 50  0001 C CNN
F 3 "~" H 1350 1350 50  0001 C CNN
F 4 "X" V 1350 1150 394 0000 L CNN "DNP"
	1    1350 1350
	0    1    1    0   
$EndComp
Wire Wire Line
	1950 1800 1350 1800
Wire Wire Line
	1350 1800 1350 1500
Wire Wire Line
	1350 1200 1350 1050
Connection ~ 1350 1050
Wire Wire Line
	1350 1050 1250 1050
Connection ~ 3200 2000
Wire Wire Line
	3200 2000 3200 2050
Wire Wire Line
	3200 1400 3200 1750
Wire Wire Line
	1350 1050 1950 1050
Wire Wire Line
	1950 1500 1950 1700
Wire Wire Line
	2150 1700 1950 1700
Connection ~ 1950 1700
Wire Wire Line
	1950 1700 1950 1800
Wire Wire Line
	2450 1500 2450 1050
Connection ~ 2450 1050
Wire Wire Line
	2450 1050 2700 1050
Wire Wire Line
	1950 1200 1950 1050
Connection ~ 1950 1050
Wire Wire Line
	1950 1050 2450 1050
Wire Wire Line
	2450 1900 2450 2000
Wire Wire Line
	2450 2000 2650 2000
Wire Wire Line
	1950 2150 1950 2250
Wire Wire Line
	3200 1750 3200 2000
Wire Wire Line
	2700 1050 3200 1050
Wire Wire Line
	3200 2350 3200 2400
Wire Wire Line
	4200 1850 3300 1850
Wire Wire Line
	3300 1850 3300 2400
Wire Wire Line
	3300 2400 3200 2400
Connection ~ 3200 2400
Wire Wire Line
	3200 2400 3200 2450
$Comp
L device:C C80
U 1 1 5B565103
P 3800 2700
F 0 "C80" H 3915 2746 50  0000 L CNN
F 1 "280pF" H 3915 2655 50  0000 L CNN
F 2 "" H 3838 2550 50  0001 C CNN
F 3 "~" H 3800 2700 50  0001 C CNN
F 4 "X" H 3800 2700 394 0000 C CNN "DNP"
F 5 "50V" H -50 0   50  0001 C CNN "Voltage"
	1    3800 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 5B56C167
P 3800 2900
F 0 "#PWR014" H 3800 2650 50  0001 C CNN
F 1 "GND" H 3805 2727 50  0000 C CNN
F 2 "" H 3800 2900 50  0001 C CNN
F 3 "" H 3800 2900 50  0001 C CNN
	1    3800 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 2500 3800 2500
Wire Wire Line
	3800 2500 3800 2550
Connection ~ 4200 2500
Wire Wire Line
	4200 2500 4200 2550
Wire Wire Line
	3800 2850 3800 2900
Text Notes 6650 2950 0    50   ~ 0
Vin_HV: 40V - 140V\nVout: 5V\nSwitching Freq: 100kHz\nRMS Current: 200mA\nSoftStart 100mS
$Comp
L power:GND #PWR0121
U 1 1 5B57E236
P 3200 5000
F 0 "#PWR0121" H 3200 4750 50  0001 C CNN
F 1 "GND" H 3205 4827 50  0000 C CNN
F 2 "" H 3200 5000 50  0001 C CNN
F 3 "" H 3200 5000 50  0001 C CNN
	1    3200 5000
	1    0    0    -1  
$EndComp
$Comp
L device:C C83
U 1 1 5B57E23E
P 4050 4850
F 0 "C83" H 4165 4896 50  0000 L CNN
F 1 "100nF" H 4165 4805 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4088 4700 50  0001 C CNN
F 3 "~" H 4050 4850 50  0001 C CNN
F 4 " " H -4900 -500 50  0001 C CNN "DNP"
F 5 "50V" H -4900 -500 50  0001 C CNN "Voltage"
	1    4050 4850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0122
U 1 1 5B57E245
P 4050 5000
F 0 "#PWR0122" H 4050 4750 50  0001 C CNN
F 1 "GND" H 4055 4827 50  0000 C CNN
F 2 "" H 4050 5000 50  0001 C CNN
F 3 "" H 4050 5000 50  0001 C CNN
	1    4050 5000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0135
U 1 1 5B57E253
P 2150 5350
F 0 "#PWR0135" H 2150 5100 50  0001 C CNN
F 1 "GND" H 2155 5177 50  0000 C CNN
F 2 "" H 2150 5350 50  0001 C CNN
F 3 "" H 2150 5350 50  0001 C CNN
	1    2150 5350
	-1   0    0    -1  
$EndComp
$Comp
L device:R R6
U 1 1 5B57E25A
P 1650 5150
F 0 "R6" V 1550 5150 50  0000 C CNN
F 1 "100R" V 1650 5150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1580 5150 50  0001 C CNN
F 3 "~" H 1650 5150 50  0001 C CNN
F 4 " " H -5650 50  50  0001 C CNN "DNP"
	1    1650 5150
	0    -1   1    0   
$EndComp
Text GLabel 4400 4600 2    50   Output ~ 0
VBATT_MONITOR_AI
Text GLabel 1400 5150 0    50   Input ~ 0
SW_EN
Wire Wire Line
	2150 4150 2150 4100
Wire Wire Line
	2150 4200 2150 4150
Wire Wire Line
	1500 5150 1400 5150
Wire Wire Line
	1400 3700 1750 3700
Wire Wire Line
	2150 3800 2150 3700
Wire Wire Line
	2150 3700 2350 3700
Wire Wire Line
	2150 4150 2550 4150
Wire Wire Line
	3200 3700 3200 3750
Wire Wire Line
	3200 3700 3450 3700
Connection ~ 2150 4150
Connection ~ 2150 3700
Connection ~ 3200 3700
$Comp
L device:R R83
U 1 1 5B57E28F
P 2150 4350
F 0 "R83" V 2050 4350 50  0000 C CNN
F 1 "100k" V 2150 4350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2080 4350 50  0001 C CNN
F 3 "~" H 2150 4350 50  0001 C CNN
F 4 " " H 200 2350 50  0001 C CNN "DNP"
	1    2150 4350
	1    0    0    -1  
$EndComp
$Comp
L device:R R84
U 1 1 5B57E297
P 3200 3900
F 0 "R84" V 3100 3900 50  0000 C CNN
F 1 "47k" V 3200 3900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3130 3900 50  0001 C CNN
F 3 "~" H 3200 3900 50  0001 C CNN
F 4 " " H 1250 1900 50  0001 C CNN "DNP"
	1    3200 3900
	1    0    0    -1  
$EndComp
$Comp
L device:R R85
U 1 1 5B57E29F
P 3200 4850
F 0 "R85" V 3100 4850 50  0000 C CNN
F 1 "10k" V 3200 4850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3130 4850 50  0001 C CNN
F 3 "~" H 3200 4850 50  0001 C CNN
F 4 " " H 1250 2850 50  0001 C CNN "DNP"
	1    3200 4850
	1    0    0    -1  
$EndComp
$Comp
L device:R R82
U 1 1 5B57E2A7
P 2150 3950
F 0 "R82" V 2050 3950 50  0000 C CNN
F 1 "10k" V 2150 3950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2080 3950 50  0001 C CNN
F 3 "~" H 2150 3950 50  0001 C CNN
F 4 " " H 200 1950 50  0001 C CNN "DNP"
	1    2150 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 4600 3500 4600
Wire Wire Line
	3200 4700 3200 4600
Connection ~ 3200 4600
Wire Wire Line
	3200 4600 3200 4450
Wire Wire Line
	4050 4700 4050 4600
Connection ~ 4050 4600
Wire Wire Line
	4050 4600 4400 4600
$Comp
L power:+BATT #PWR?
U 1 1 5B587EC2
P 1400 3700
AR Path="/5B2FB0D8/5B587EC2" Ref="#PWR?"  Part="1" 
AR Path="/5B2FB036/5B587EC2" Ref="#PWR?"  Part="1" 
AR Path="/5C8FC872/5C8FC885/5B587EC2" Ref="#PWR0136"  Part="1" 
F 0 "#PWR0136" H 1400 3550 50  0001 C CNN
F 1 "+BATT" H 1415 3873 50  0000 C CNN
F 2 "" H 1400 3700 50  0001 C CNN
F 3 "" H 1400 3700 50  0001 C CNN
	1    1400 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 3700 3200 3700
$Comp
L device:D_Zener D10
U 1 1 5B58E618
P 1750 3950
F 0 "D10" V 1704 4029 50  0000 L CNN
F 1 "15V" V 1795 4029 50  0000 L CNN
F 2 "" H 1750 3950 50  0001 C CNN
F 3 "~" H 1750 3950 50  0001 C CNN
F 4 " " V 1841 4029 394 0001 L CNN "DNP"
	1    1750 3950
	0    1    1    0   
$EndComp
Wire Wire Line
	2150 4150 1750 4150
Wire Wire Line
	1750 4150 1750 4100
Wire Wire Line
	1750 3800 1750 3700
Connection ~ 1750 3700
Wire Wire Line
	1750 3700 2150 3700
Wire Wire Line
	1800 5150 1850 5150
Wire Notes Line rgb(255, 0, 0)
	600  550  6450 550 
Wire Notes Line rgb(255, 0, 0)
	6450 550  6450 3200
Wire Notes Line rgb(255, 0, 0)
	6450 3200 600  3200
Wire Notes Line rgb(255, 0, 0)
	600  3200 600  550 
Text Notes 5850 650  0    50   ~ 0
High Voltage
Text Label 5450 1850 0    50   ~ 0
5V_SW_NODE_HV
Text Label 3400 1750 0    50   ~ 0
5V_RUN_HV
Wire Wire Line
	5000 1850 5150 1850
NoConn ~ 5000 2350
Text Label 3400 1850 0    50   ~ 0
5V_OVLO_HV
Text Label 3450 3700 0    50   ~ 0
+BATT_SW_HV
Wire Wire Line
	2150 4950 2150 4850
Text Label 1000 4150 0    50   ~ 0
+BATT_SW_GATE_HV
Wire Wire Line
	1750 4150 1000 4150
Connection ~ 1750 4150
Text Label 1050 4850 0    50   ~ 0
+BATT_SW_FET_HV
Wire Wire Line
	2150 4850 1050 4850
Connection ~ 2150 4850
Wire Wire Line
	2150 4850 2150 4800
Wire Notes Line rgb(255, 0, 0)
	600  3300 5150 3300
Wire Notes Line rgb(255, 0, 0)
	5150 3300 5150 5600
Wire Notes Line rgb(255, 0, 0)
	5150 5600 600  5600
Wire Notes Line rgb(255, 0, 0)
	600  5650 600  3300
Text Notes 3950 3400 0    50   ~ 0
High Voltage
$Comp
L device:R R90
U 1 1 5B59E65C
P 3800 4600
F 0 "R90" V 3700 4600 50  0000 C CNN
F 1 "10k" V 3800 4600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3730 4600 50  0001 C CNN
F 3 "~" H 3800 4600 50  0001 C CNN
F 4 " " H 1850 2600 50  0001 C CNN "DNP"
	1    3800 4600
	0    1    1    0   
$EndComp
Wire Wire Line
	3950 4600 4050 4600
$Comp
L device:R R88
U 1 1 5B5A4224
P 3200 4300
F 0 "R88" V 3100 4300 50  0000 C CNN
F 1 "47k" V 3200 4300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3130 4300 50  0001 C CNN
F 3 "~" H 3200 4300 50  0001 C CNN
F 4 " " H 1250 2300 50  0001 C CNN "DNP"
	1    3200 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 4150 3200 4050
Connection ~ 1950 1850
Wire Wire Line
	1950 1850 1950 2000
$Comp
L device:R R86
U 1 1 5B5B041D
P 1950 2400
F 0 "R86" V 1850 2400 50  0000 C CNN
F 1 "100k" V 1950 2400 50  0000 C CNN
F 2 "" V 1880 2400 50  0001 C CNN
F 3 "~" H 1950 2400 50  0001 C CNN
F 4 "X" H 1950 2400 394 0000 C CNN "DNP"
	1    1950 2400
	1    0    0    -1  
$EndComp
$Comp
L device:R R87
U 1 1 5B5C2886
P 2150 4650
F 0 "R87" V 2050 4650 50  0000 C CNN
F 1 "100k" V 2150 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2080 4650 50  0001 C CNN
F 3 "~" H 2150 4650 50  0001 C CNN
F 4 " " H 200 2650 50  0001 C CNN "DNP"
	1    2150 4650
	1    0    0    -1  
$EndComp
$Comp
L device:D_Zener D11
U 1 1 5B5DA38E
P 3500 4850
F 0 "D11" V 3454 4929 50  0000 L CNN
F 1 "15V" V 3545 4929 50  0000 L CNN
F 2 "" H 3500 4850 50  0001 C CNN
F 3 "~" H 3500 4850 50  0001 C CNN
F 4 " " V 3591 4929 394 0001 L CNN "DNP"
	1    3500 4850
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0137
U 1 1 5B5DA43D
P 3500 5000
F 0 "#PWR0137" H 3500 4750 50  0001 C CNN
F 1 "GND" H 3505 4827 50  0000 C CNN
F 2 "" H 3500 5000 50  0001 C CNN
F 3 "" H 3500 5000 50  0001 C CNN
	1    3500 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 4700 3500 4600
Connection ~ 3500 4600
Wire Wire Line
	3500 4600 3650 4600
Connection ~ 5800 2600
Text Label 5900 2600 0    50   ~ 0
5V_SNUB_HV
Wire Wire Line
	5800 2600 6350 2600
Text Label 500  1800 0    50   ~ 0
ENABLE_STBY_SUPPLY_PMOS_GATE_HV
Wire Wire Line
	1350 1800 500  1800
Connection ~ 1350 1800
$Comp
L device:R R89
U 1 1 5B7523D2
P 3650 7100
F 0 "R89" V 3550 7100 50  0000 C CNN
F 1 "10k" V 3650 7100 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3580 7100 50  0001 C CNN
F 3 "~" H 3650 7100 50  0001 C CNN
F 4 " " H 1700 5100 50  0001 C CNN "DNP"
	1    3650 7100
	0    1    1    0   
$EndComp
Wire Wire Line
	3800 7100 4000 7100
$Comp
L ICs:NX1029X,115 Q2
U 1 1 5B753FDB
P 2400 7350
F 0 "Q2" H 2517 7403 60  0000 L CNN
F 1 "NX1029X,115" H 2517 7297 60  0000 L CNN
F 2 "" H 2375 7350 60  0001 C CNN
F 3 "" H 2375 7350 60  0001 C CNN
	1    2400 7350
	1    0    0    -1  
$EndComp
$Comp
L ICs:NX1029X,115 Q2
U 2 1 5B754077
P 2800 6350
F 0 "Q2" V 3076 6350 60  0000 C CNN
F 1 "NX1029X,115" V 2970 6350 60  0000 C CNN
F 2 "" H 2775 6350 60  0001 C CNN
F 3 "" H 2775 6350 60  0001 C CNN
	2    2800 6350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2800 6550 2800 6800
Wire Wire Line
	2050 7350 2200 7350
$Comp
L ICs:NX1029X,115 Q5
U 1 1 5B7614D6
P 9000 2600
F 0 "Q5" H 9117 2653 60  0000 L CNN
F 1 "NX1029X,115" H 9117 2547 60  0000 L CNN
F 2 "" H 8975 2600 60  0001 C CNN
F 3 "" H 8975 2600 60  0001 C CNN
	1    9000 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 2600 8800 2600
$Comp
L ICs:NX1029X,115 Q5
U 2 1 5B767F21
P 9500 1850
F 0 "Q5" V 9776 1850 60  0000 C CNN
F 1 "NX1029X,115" V 9670 1850 60  0000 C CNN
F 2 "" H 9475 1850 60  0001 C CNN
F 3 "" H 9475 1850 60  0001 C CNN
	2    9500 1850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9500 2050 9500 2350
$Comp
L device:Q_PMOS_DGS Q4
U 1 1 5B76EB94
P 2550 3800
F 0 "Q4" V 2893 3556 50  0000 L CNN
F 1 "Q_PMOS_DGS" V 2802 3556 50  0000 L CNN
F 2 "" H 2750 3900 50  0001 C CNN
F 3 "~" H 2550 3800 50  0001 C CNN
	1    2550 3800
	0    1    -1   0   
$EndComp
Wire Wire Line
	2550 4000 2550 4150
$Comp
L device:Q_NMOS_DGS Q1
U 1 1 5B775564
P 2050 5150
F 0 "Q1" H 2255 5196 50  0000 L CNN
F 1 "Q_NMOS_DGS" H 2255 5105 50  0000 L CNN
F 2 "" H 2250 5250 50  0001 C CNN
F 3 "~" H 2050 5150 50  0001 C CNN
	1    2050 5150
	1    0    0    -1  
$EndComp
$Comp
L device:Q_NMOS_DGS Q14
U 1 1 5C18FA73
P 1850 2750
F 0 "Q14" H 2056 2796 50  0000 L CNN
F 1 "Q_NMOS_DGS" H 2056 2705 50  0000 L CNN
F 2 "" H 2050 2850 50  0001 C CNN
F 3 "~" H 1850 2750 50  0001 C CNN
F 4 "X" H 1900 2750 394 0000 C CNN "DNP"
	1    1850 2750
	1    0    0    -1  
$EndComp
$Comp
L device:Q_PMOS_DGS Q15
U 1 1 5C1982EE
P 2350 1700
F 0 "Q15" H 2556 1654 50  0000 L CNN
F 1 "Q_PMOS_DGS" H 2556 1745 50  0000 L CNN
F 2 "" H 2550 1800 50  0001 C CNN
F 3 "~" H 2350 1700 50  0001 C CNN
F 4 "X" H 2400 1700 394 0000 C CNN "DNP"
	1    2350 1700
	1    0    0    1   
$EndComp
$EndSCHEMATC
