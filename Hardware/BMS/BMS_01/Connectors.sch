EESchema Schematic File Version 4
LIBS:BMS_01-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 57 58
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
L Connector:USB_B_Micro J?
U 1 1 5C18C517
P 9450 4100
AR Path="/5B64EF74/5C18C517" Ref="J?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5C18C517" Ref="J5"  Part="1" 
F 0 "J5" H 9505 4567 50  0000 C CNN
F 1 "USB_B_Micro" H 9505 4476 50  0000 C CNN
F 2 "Connectors:USB_Micro-B_10103594-0001LF" H 9600 4050 50  0001 C CNN
F 3 "~" H 9600 4050 50  0001 C CNN
F 4 " " H 1050 -800 50  0001 C CNN "DNP"
	1    9450 4100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5C18C6FD
P 9450 4500
AR Path="/5B64EF74/5C18C6FD" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5C18C6FD" Ref="#PWR0139"  Part="1" 
F 0 "#PWR0139" H 9450 4250 50  0001 C CNN
F 1 "GND" H 9455 4327 50  0000 C CNN
F 2 "" H 9450 4500 50  0001 C CNN
F 3 "" H 9450 4500 50  0001 C CNN
	1    9450 4500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5C18C715
P 9000 4950
AR Path="/5B64EF74/5C18C715" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5C18C715" Ref="#PWR0137"  Part="1" 
F 0 "#PWR0137" H 9000 4700 50  0001 C CNN
F 1 "GND" H 9005 4777 50  0000 C CNN
F 2 "" H 9000 4950 50  0001 C CNN
F 3 "" H 9000 4950 50  0001 C CNN
	1    9000 4950
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5C18C739
P 9000 4800
AR Path="/5B64EF74/5C18C739" Ref="C?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5C18C739" Ref="C96"  Part="1" 
F 0 "C96" H 9115 4846 50  0000 L CNN
F 1 "C" H 9115 4755 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 9038 4650 50  0001 C CNN
F 3 "~" H 9000 4800 50  0001 C CNN
F 4 " " H 1050 -800 50  0001 C CNN "DNP"
F 5 "50V" H 1050 -800 50  0001 C CNN "Voltage"
	1    9000 4800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5C18CFAA
P 2450 5800
AR Path="/5B64EF74/5C18CFAA" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5C18CFAA" Ref="#PWR0141"  Part="1" 
F 0 "#PWR0141" H 2450 5550 50  0001 C CNN
F 1 "GND" H 2455 5627 50  0000 C CNN
F 2 "" H 2450 5800 50  0001 C CNN
F 3 "" H 2450 5800 50  0001 C CNN
	1    2450 5800
	0    1    1    0   
$EndComp
$Comp
L power:+BATT #PWR?
U 1 1 5C1B76AE
P 1900 1600
AR Path="/5B64EF74/5C1B76AE" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5C1B76AE" Ref="#PWR0133"  Part="1" 
F 0 "#PWR0133" H 1900 1450 50  0001 C CNN
F 1 "+BATT" H 1915 1773 50  0000 C CNN
F 2 "" H 1900 1600 50  0001 C CNN
F 3 "" H 1900 1600 50  0001 C CNN
	1    1900 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5C1B7790
P 2100 4650
AR Path="/5B64EF74/5C1B7790" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5C1B7790" Ref="#PWR0134"  Part="1" 
F 0 "#PWR0134" H 2100 4400 50  0001 C CNN
F 1 "GND" H 2105 4477 50  0000 C CNN
F 2 "" H 2100 4650 50  0001 C CNN
F 3 "" H 2100 4650 50  0001 C CNN
	1    2100 4650
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5C4905F0
P 8300 1450
AR Path="/5B64EF74/5C4905F0" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5C4905F0" Ref="#PWR0135"  Part="1" 
F 0 "#PWR0135" H 8300 1300 50  0001 C CNN
F 1 "+3V3" H 8315 1623 50  0000 C CNN
F 2 "" H 8300 1450 50  0001 C CNN
F 3 "" H 8300 1450 50  0001 C CNN
	1    8300 1450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5C490708
P 8300 2150
AR Path="/5B64EF74/5C490708" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5C490708" Ref="#PWR0136"  Part="1" 
F 0 "#PWR0136" H 8300 1900 50  0001 C CNN
F 1 "GND" H 8305 1977 50  0000 C CNN
F 2 "" H 8300 2150 50  0001 C CNN
F 3 "" H 8300 2150 50  0001 C CNN
	1    8300 2150
	1    0    0    -1  
$EndComp
$Comp
L device:D_TVS D?
U 1 1 5C56ABB1
P 9300 2150
AR Path="/5B64EF74/5C56ABB1" Ref="D?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5C56ABB1" Ref="D86"  Part="1" 
F 0 "D86" V 9254 2229 50  0000 L CNN
F 1 "D_TVS" V 9345 2229 50  0000 L CNN
F 2 "Diodes_SMD:D_0603" H 9300 2150 50  0001 C CNN
F 3 "~" H 9300 2150 50  0001 C CNN
F 4 " " H 1100 -250 50  0001 C CNN "DNP"
	1    9300 2150
	0    1    1    0   
$EndComp
$Comp
L device:D_TVS D?
U 1 1 5C56AC04
P 9500 2450
AR Path="/5B64EF74/5C56AC04" Ref="D?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5C56AC04" Ref="D87"  Part="1" 
F 0 "D87" V 9454 2529 50  0000 L CNN
F 1 "D_TVS" V 9545 2529 50  0000 L CNN
F 2 "Diodes_SMD:D_0603" H 9500 2450 50  0001 C CNN
F 3 "~" H 9500 2450 50  0001 C CNN
F 4 " " H 1100 -250 50  0001 C CNN "DNP"
	1    9500 2450
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5C56B1A3
P 9300 2300
AR Path="/5B64EF74/5C56B1A3" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5C56B1A3" Ref="#PWR0138"  Part="1" 
F 0 "#PWR0138" H 9300 2050 50  0001 C CNN
F 1 "GND" H 9305 2127 50  0000 C CNN
F 2 "" H 9300 2300 50  0001 C CNN
F 3 "" H 9300 2300 50  0001 C CNN
	1    9300 2300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5C56B1C0
P 9500 2600
AR Path="/5B64EF74/5C56B1C0" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5C56B1C0" Ref="#PWR0140"  Part="1" 
F 0 "#PWR0140" H 9500 2350 50  0001 C CNN
F 1 "GND" H 9505 2427 50  0000 C CNN
F 2 "" H 9500 2600 50  0001 C CNN
F 3 "" H 9500 2600 50  0001 C CNN
	1    9500 2600
	1    0    0    -1  
$EndComp
Text GLabel 9750 3900 2    60   Output ~ 0
USB_5V
Text GLabel 9750 4100 2    60   BiDi ~ 0
USB_D+
Text GLabel 9750 4200 2    60   BiDi ~ 0
USB_D-
Text GLabel 3250 5900 0    60   BiDi ~ 0
CAN_P
Text GLabel 3250 6000 0    60   BiDi ~ 0
CAN_N
Text GLabel 8650 1750 0    49   Output ~ 0
PGED
Text GLabel 8650 1850 0    49   Output ~ 0
PGEC
Text GLabel 9700 1450 0    49   Output ~ 0
nRESET
Text Notes 1700 1050 0    197  ~ 0
POWER
Text Notes 8300 950  0    197  ~ 0
DEBUG/JTAG
Text Notes 2000 6250 0    197  ~ 0
CAN
Text Notes 9150 3400 0    197  ~ 0
USB
Wire Wire Line
	9350 4500 9350 4550
Wire Wire Line
	9350 4550 9000 4550
Wire Wire Line
	9000 4550 9000 4650
Wire Wire Line
	1900 1750 1900 1600
Wire Wire Line
	2100 4650 2100 4550
Wire Wire Line
	2100 4550 2400 4550
Wire Wire Line
	8300 1450 8300 1550
Wire Wire Line
	8300 1550 9700 1550
Wire Wire Line
	8300 2150 8300 1650
Wire Wire Line
	8300 1650 9700 1650
Wire Wire Line
	9300 2000 9300 1750
Wire Wire Line
	9300 1750 9250 1750
Wire Wire Line
	9300 1750 9700 1750
Wire Wire Line
	9700 1850 9500 1850
Wire Wire Line
	9500 2300 9500 1850
Wire Wire Line
	9500 1850 9000 1850
Connection ~ 9300 1750
Connection ~ 9500 1850
NoConn ~ 9750 4300
NoConn ~ 9700 1950
$Comp
L device:R R?
U 1 1 5B5C8B23
P 9100 1750
AR Path="/5B2FB0D8/5B5C8B23" Ref="R?"  Part="1" 
AR Path="/5BC21A84/5B5C8B23" Ref="R?"  Part="1" 
AR Path="/5C8FC872/5C8FC88B/5B5C8B23" Ref="R?"  Part="1" 
AR Path="/5C8FC872/5C8FC88D/5B5C8B23" Ref="R?"  Part="1" 
AR Path="/5B64EF74/5B5C8B23" Ref="R?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5B5C8B23" Ref="R188"  Part="1" 
F 0 "R188" V 9000 1750 50  0000 C CNN
F 1 "100R" V 9100 1750 50  0000 C CNN
F 2 "Footprints:R_0603_Standard" V 9030 1750 50  0001 C CNN
F 3 "~" H 9100 1750 50  0001 C CNN
F 4 " " H 1100 50  50  0001 C CNN "DNP"
	1    9100 1750
	0    1    1    0   
$EndComp
Wire Wire Line
	8950 1750 8650 1750
$Comp
L device:R R?
U 1 1 5B5C912B
P 8850 1850
AR Path="/5B2FB0D8/5B5C912B" Ref="R?"  Part="1" 
AR Path="/5BC21A84/5B5C912B" Ref="R?"  Part="1" 
AR Path="/5C8FC872/5C8FC88B/5B5C912B" Ref="R?"  Part="1" 
AR Path="/5C8FC872/5C8FC88D/5B5C912B" Ref="R?"  Part="1" 
AR Path="/5B64EF74/5B5C912B" Ref="R?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5B5C912B" Ref="R187"  Part="1" 
F 0 "R187" V 8750 1850 50  0000 C CNN
F 1 "100R" V 8850 1850 50  0000 C CNN
F 2 "Footprints:R_0603_Standard" V 8780 1850 50  0001 C CNN
F 3 "~" H 8850 1850 50  0001 C CNN
F 4 " " H 850 150 50  0001 C CNN "DNP"
	1    8850 1850
	0    1    1    0   
$EndComp
Wire Wire Line
	8700 1850 8650 1850
$Comp
L power:PWR_FLAG #FLG?
U 1 1 5B60E1EA
P 2150 1750
AR Path="/5B64EF74/5B60E1EA" Ref="#FLG?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5B60E1EA" Ref="#FLG03"  Part="1" 
F 0 "#FLG03" H 2150 1825 50  0001 C CNN
F 1 "PWR_FLAG" H 2150 1924 50  0000 C CNN
F 2 "" H 2150 1750 50  0001 C CNN
F 3 "~" H 2150 1750 50  0001 C CNN
	1    2150 1750
	1    0    0    -1  
$EndComp
Connection ~ 2150 1750
Wire Wire Line
	2150 1750 1900 1750
Text GLabel 2400 3350 0    50   Input ~ 0
BANK_12
Text GLabel 2400 3450 0    50   Input ~ 0
BANK_11
Text GLabel 2400 3550 0    50   Input ~ 0
BANK_10
Text GLabel 2400 3650 0    50   Input ~ 0
BANK_09
Text GLabel 2400 3750 0    50   Input ~ 0
BANK_08
Text GLabel 2400 3850 0    50   Input ~ 0
BANK_07
Text GLabel 2400 3950 0    50   Input ~ 0
BANK_06
Text GLabel 2400 4050 0    50   Input ~ 0
BANK_05
Text GLabel 2400 4150 0    50   Input ~ 0
BANK_04
Text GLabel 2400 4250 0    50   Input ~ 0
BANK_03
Text GLabel 2400 4350 0    50   Input ~ 0
BANK_02
Text GLabel 2400 4450 0    50   Input ~ 0
BANK_01
Text GLabel 2350 1850 0    50   Input ~ 0
BANK_24
Text GLabel 2350 1950 0    50   Input ~ 0
BANK_23
Text GLabel 2350 2150 0    50   Input ~ 0
BANK_21
Text GLabel 2350 2350 0    50   Input ~ 0
BANK_19
Text GLabel 2350 2450 0    50   Input ~ 0
BANK_18
Text GLabel 2350 2550 0    50   Input ~ 0
BANK_17
Text GLabel 2350 2650 0    50   Input ~ 0
BANK_16
Text GLabel 2350 2750 0    50   Input ~ 0
BANK_15
Text GLabel 2350 2850 0    50   Input ~ 0
BANK_14
Text GLabel 2350 2950 0    50   Input ~ 0
BANK_13
Text GLabel 2350 2050 0    50   Input ~ 0
BANK_22
Text GLabel 2350 2250 0    50   Input ~ 0
BANK_20
Text GLabel 5650 4350 0    59   Input ~ 0
NTC_18_OUT
Text GLabel 6150 4350 2    59   Output ~ 0
NTC_18_RTN
Text GLabel 5650 4450 0    59   Input ~ 0
NTC_19_OUT
Text GLabel 6150 4450 2    59   Output ~ 0
NTC_19_RTN
Text GLabel 5650 4550 0    59   Input ~ 0
NTC_20_OUT
Text GLabel 6150 4550 2    59   Output ~ 0
NTC_20_RTN
Text GLabel 5650 4650 0    59   Input ~ 0
NTC_21_OUT
Text GLabel 6150 4650 2    59   Output ~ 0
NTC_21_RTN
Text GLabel 5650 4750 0    59   Input ~ 0
NTC_22_OUT
Text GLabel 6150 4750 2    59   Output ~ 0
NTC_22_RTN
Text GLabel 5650 4850 0    59   Input ~ 0
NTC_23_OUT
Text GLabel 6150 4850 2    59   Output ~ 0
NTC_23_RTN
Text GLabel 5650 4950 0    59   Input ~ 0
NTC_24_OUT
Text GLabel 6150 4950 2    59   Output ~ 0
NTC_24_RTN
Text GLabel 5650 4250 0    59   Input ~ 0
NTC_17_OUT
Text GLabel 6150 4250 2    59   Output ~ 0
NTC_17_RTN
Text GLabel 5600 3050 0    59   Output ~ 0
NTC_9_OUT
Text GLabel 6100 3050 2    59   Input ~ 0
NTC_9_RTN
Text GLabel 5600 3150 0    59   Output ~ 0
NTC_10_OUT
Text GLabel 6100 3150 2    59   Input ~ 0
NTC_10_RTN
Text GLabel 5600 3250 0    59   Output ~ 0
NTC_11_OUT
Text GLabel 6100 3250 2    59   Input ~ 0
NTC_11_RTN
Text GLabel 5600 3350 0    59   Output ~ 0
NTC_12_OUT
Text GLabel 6100 3350 2    59   Input ~ 0
NTC_12_RTN
Text GLabel 5650 3850 0    59   Input ~ 0
NTC_13_OUT
Text GLabel 6150 3850 2    59   Output ~ 0
NTC_13_RTN
Text GLabel 5650 3950 0    59   Input ~ 0
NTC_14_OUT
Text GLabel 6150 3950 2    59   Output ~ 0
NTC_14_RTN
Text GLabel 5650 4050 0    59   Input ~ 0
NTC_15_OUT
Text GLabel 6150 4050 2    59   Output ~ 0
NTC_15_RTN
Text GLabel 5650 4150 0    59   Input ~ 0
NTC_16_OUT
Text GLabel 6150 4150 2    59   Output ~ 0
NTC_16_RTN
$Comp
L Connector_Generic:Conn_02x12_Top_Bottom J3
U 1 1 5B6301EE
P 5800 2750
F 0 "J3" H 5850 3467 50  0000 C CNN
F 1 "Conn_02x12_Top_Bottom" H 5850 3376 50  0000 C CNN
F 2 "Connectors_Molex:Molex_Microfit3_Header_02x12_Angled_43045-240x" H 5800 2750 50  0001 C CNN
F 3 "~" H 5800 2750 50  0001 C CNN
	1    5800 2750
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x12_Top_Bottom J4
U 1 1 5B630250
P 5850 4350
F 0 "J4" H 5900 5067 50  0000 C CNN
F 1 "Conn_02x12_Top_Bottom" H 5900 4976 50  0000 C CNN
F 2 "Connectors_Molex:Molex_Microfit3_Header_02x12_Angled_43045-240x" H 5850 4350 50  0001 C CNN
F 3 "~" H 5850 4350 50  0001 C CNN
	1    5850 4350
	1    0    0    -1  
$EndComp
Text GLabel 5600 2250 0    59   Output ~ 0
NTC_1_OUT
Text GLabel 6100 2250 2    59   Input ~ 0
NTC_1_RTN
Text GLabel 5600 2350 0    59   Output ~ 0
NTC_2_OUT
Text GLabel 6100 2350 2    59   Input ~ 0
NTC_2_RTN
Text GLabel 5600 2450 0    59   Output ~ 0
NTC_3_OUT
Text GLabel 6100 2450 2    59   Input ~ 0
NTC_3_RTN
Text GLabel 5600 2550 0    59   Output ~ 0
NTC_4_OUT
Text GLabel 6100 2550 2    59   Input ~ 0
NTC_4_RTN
Text GLabel 5600 2650 0    59   Output ~ 0
NTC_5_OUT
Text GLabel 6100 2650 2    59   Input ~ 0
NTC_5_RTN
Text GLabel 5600 2750 0    59   Output ~ 0
NTC_6_OUT
Text GLabel 6100 2750 2    59   Input ~ 0
NTC_6_RTN
Text GLabel 5600 2850 0    59   Output ~ 0
NTC_7_OUT
Text GLabel 6100 2850 2    59   Input ~ 0
NTC_7_RTN
Text GLabel 5600 2950 0    59   Output ~ 0
NTC_8_OUT
Text GLabel 6100 2950 2    59   Input ~ 0
NTC_8_RTN
Text Notes 5250 1650 0    295  ~ 0
NTCs
$Comp
L Connector_Generic:Conn_01x13 J2
U 1 1 5B63ADB2
P 2600 3950
F 0 "J2" H 2680 3992 50  0000 L CNN
F 1 "Conn_01x13" H 2680 3901 50  0000 L CNN
F 2 "Connectors_Molex:Molex_KK-6410-13_13x2.54mm_Straight" H 2600 3950 50  0001 C CNN
F 3 "~" H 2600 3950 50  0001 C CNN
	1    2600 3950
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x13 J1
U 1 1 5B63AE04
P 2550 2350
F 0 "J1" H 2630 2392 50  0000 L CNN
F 1 "Conn_01x13" H 2630 2301 50  0000 L CNN
F 2 "Connectors_Molex:Molex_KK-6410-13_13x2.54mm_Straight" H 2550 2350 50  0001 C CNN
F 3 "~" H 2550 2350 50  0001 C CNN
	1    2550 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 4550 1900 4550
Connection ~ 2100 4550
NoConn ~ 9700 5850
Text GLabel 1900 4550 0    59   UnSpc ~ 0
GND
Text GLabel 3250 5600 0    59   Input ~ 0
Transducer_Ref
Text GLabel 3250 5700 0    59   Input ~ 0
Transducer_Pwr
Text GLabel 3250 5500 0    59   Output ~ 0
Transducer_Input
$Comp
L power:GND #PWR?
U 1 1 5B7C6A36
P 2850 6100
AR Path="/5B64EF74/5B7C6A36" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5B7C6A36" Ref="#PWR0156"  Part="1" 
F 0 "#PWR0156" H 2850 5850 50  0001 C CNN
F 1 "GND" H 2855 5927 50  0000 C CNN
F 2 "" H 2850 6100 50  0001 C CNN
F 3 "" H 2850 6100 50  0001 C CNN
	1    2850 6100
	0    1    1    0   
$EndComp
Wire Wire Line
	2850 6100 3250 6100
$Comp
L Connector_Generic:Conn_01x06 J?
U 1 1 5C490563
P 9900 1650
AR Path="/5B64EF74/5C490563" Ref="J?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD61E/5C490563" Ref="J6"  Part="1" 
F 0 "J6" H 9980 1642 50  0000 L CNN
F 1 "Conn_01x06" H 9980 1551 50  0000 L CNN
F 2 "Connectors_Molex:Molex_KK-6410-06_06x2.54mm_Straight" H 9900 1650 50  0001 C CNN
F 3 "~" H 9900 1650 50  0001 C CNN
F 4 " " H 1100 -250 50  0001 C CNN "DNP"
	1    9900 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 1750 2350 1750
$Comp
L Connector_Generic:Conn_01x16 J7
U 1 1 5B7DF3D5
P 3450 6200
F 0 "J7" H 3530 6192 50  0000 L CNN
F 1 "Conn_01x16" H 3530 6101 50  0000 L CNN
F 2 "" H 3450 6200 50  0001 C CNN
F 3 "~" H 3450 6200 50  0001 C CNN
	1    3450 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 5800 3250 5800
NoConn ~ 3250 6200
NoConn ~ 3250 6300
NoConn ~ 3250 6400
NoConn ~ 3250 6500
NoConn ~ 3250 6600
NoConn ~ 3250 6700
NoConn ~ 3250 6800
NoConn ~ 3250 6900
NoConn ~ 3250 7000
$EndSCHEMATC
