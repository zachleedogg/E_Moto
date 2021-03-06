EESchema Schematic File Version 4
LIBS:BodyController_01-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 8 13
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
L BodyController_01-rescue:GND-RESCUE-BodyController_01 #PWR0125
U 1 1 5995D681
P 2000 2150
F 0 "#PWR0125" H 2000 1900 50  0001 C CNN
F 1 "GND" H 2000 2000 50  0000 C CNN
F 2 "" H 2000 2150 50  0001 C CNN
F 3 "" H 2000 2150 50  0001 C CNN
	1    2000 2150
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:+BATT-RESCUE-BodyController_01 #PWR0126
U 1 1 5995D6C1
P 2000 1100
F 0 "#PWR0126" H 2000 950 50  0001 C CNN
F 1 "+BATT" H 2000 1240 50  0000 C CNN
F 2 "" H 2000 1100 50  0001 C CNN
F 3 "" H 2000 1100 50  0001 C CNN
	1    2000 1100
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:CONN_01X06-RESCUE-BodyController_01 J7
U 1 1 599715F6
P 2100 3150
F 0 "J7" H 2100 3500 50  0000 C CNN
F 1 "CONN_01X06" V 2200 3150 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06_Pitch2.54mm" H 2100 3150 50  0001 C CNN
F 3 "" H 2100 3150 50  0001 C CNN
F 4 "n" H 2100 3600 50  0001 C CNN "populate"
	1    2100 3150
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:+3V3-RESCUE-BodyController_01 #PWR0127
U 1 1 5997230B
P 650 2950
F 0 "#PWR0127" H 650 2800 50  0001 C CNN
F 1 "+3V3" H 650 3090 50  0000 C CNN
F 2 "" H 650 2950 50  0001 C CNN
F 3 "" H 650 2950 50  0001 C CNN
	1    650  2950
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:GND-RESCUE-BodyController_01 #PWR0128
U 1 1 5997232D
P 650 3150
F 0 "#PWR0128" H 650 2900 50  0001 C CNN
F 1 "GND" H 650 3000 50  0000 C CNN
F 2 "" H 650 3150 50  0001 C CNN
F 3 "" H 650 3150 50  0001 C CNN
	1    650  3150
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:PWR_FLAG-RESCUE-BodyController_01 #FLG0129
U 1 1 59980CA4
P 1650 1150
F 0 "#FLG0129" H 1650 1225 50  0001 C CNN
F 1 "PWR_FLAG" H 1650 1300 50  0000 C CNN
F 2 "" H 1650 1150 50  0001 C CNN
F 3 "" H 1650 1150 50  0001 C CNN
	1    1650 1150
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:PWR_FLAG-RESCUE-BodyController_01 #FLG0130
U 1 1 59980CC6
P 1650 2050
F 0 "#FLG0130" H 1650 2125 50  0001 C CNN
F 1 "PWR_FLAG" H 1650 2200 50  0000 C CNN
F 2 "" H 1650 2050 50  0001 C CNN
F 3 "" H 1650 2050 50  0001 C CNN
	1    1650 2050
	-1   0    0    1   
$EndComp
$Comp
L BodyController_01-rescue:USB_OTG-RESCUE-BodyController_01 J8
U 1 1 59A8218E
P 1600 5200
F 0 "J8" H 1400 5650 50  0000 L CNN
F 1 "USB_OTG" H 1400 5550 50  0000 L CNN
F 2 "Connectors_USB:USB_Micro-B_Vertical_Molex-105133-0001" H 1750 5150 50  0001 C CNN
F 3 "http://www.molex.com/pdm_docs/sd/1051330001_sd.pdf" H 1750 5150 50  0001 C CNN
F 4 "1051330001" H 1400 5750 50  0001 C CNN "MPN"
F 5 "y" H 1400 5750 50  0001 C CNN "populate"
	1    1600 5200
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:GND-RESCUE-BodyController_01 #PWR0131
U 1 1 59A82EC9
P 1600 5650
F 0 "#PWR0131" H 1600 5400 50  0001 C CNN
F 1 "GND" H 1600 5500 50  0000 C CNN
F 2 "" H 1600 5650 50  0001 C CNN
F 3 "" H 1600 5650 50  0001 C CNN
	1    1600 5650
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:Conn_01x14-RESCUE-BodyController_01 J2
U 1 1 5A30D0E9
P 3800 4800
F 0 "J2" H 3800 5500 50  0000 C CNN
F 1 "Conn_01x14" H 3800 4000 50  0000 C CNN
F 2 "Footprints:34772-0140" H 3800 4800 50  0001 C CNN
F 3 "" H 3800 4800 50  0001 C CNN
F 4 "y" H 3800 5600 50  0001 C CNN "populate"
F 5 "34772-0140" H 3800 4800 60  0001 C CNN "MPN"
	1    3800 4800
	0    -1   1    0   
$EndComp
$Comp
L BodyController_01-rescue:Conn_02x10_Top_Bottom-RESCUE-BodyController_01 J3
U 1 1 5A314BE2
P 5550 4800
F 0 "J3" H 5600 5300 50  0000 C CNN
F 1 "Conn_02x10_Top_Bottom" H 5600 4200 50  0000 C CNN
F 2 "Footprints:34690-0200" H 5550 4800 50  0001 C CNN
F 3 "" H 5550 4800 50  0001 C CNN
F 4 "y" H 5600 5400 50  0001 C CNN "populate"
F 5 "34690-0201" H 5550 4800 60  0001 C CNN "MPN"
	1    5550 4800
	0    -1   1    0   
$EndComp
$Comp
L BodyController_01-rescue:Conn_02x10_Top_Bottom-RESCUE-BodyController_01 J4
U 1 1 5A314C2C
P 7000 4800
F 0 "J4" H 7050 5300 50  0000 C CNN
F 1 "Conn_02x10_Top_Bottom" H 7050 4200 50  0000 C CNN
F 2 "Footprints:34690-0200" H 7000 4800 50  0001 C CNN
F 3 "" H 7000 4800 50  0001 C CNN
F 4 "y" H 7050 5400 50  0001 C CNN "populate"
F 5 "34690-0203" H 7000 4800 60  0001 C CNN "MPN"
	1    7000 4800
	0    -1   1    0   
$EndComp
$Comp
L BodyController_01-rescue:7796-RESCUE-BodyController_01 J1
U 1 1 5A393F4B
P 1400 1250
F 0 "J1" H 1240 1500 60  0000 C CNN
F 1 "7796" H 1260 1200 60  0000 C CNN
F 2 "Footprints:7796" H 1360 1280 60  0001 C CNN
F 3 "http://www.keyelco.com/product-pdf.cfm?p=1114" H 1360 1280 60  0001 C CNN
F 4 "7796" H 1240 1600 50  0001 C CNN "MPN"
F 5 "y" H 1240 1600 50  0001 C CNN "populate"
	1    1400 1250
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:7796-RESCUE-BodyController_01 J5
U 1 1 5A394530
P 1400 1700
F 0 "J5" H 1240 1950 60  0000 C CNN
F 1 "7796" H 1260 1650 60  0000 C CNN
F 2 "Footprints:7796" H 1360 1730 60  0001 C CNN
F 3 "http://www.keyelco.com/product-pdf.cfm?p=1114" H 1360 1730 60  0001 C CNN
F 4 "7796" H 1240 2050 50  0001 C CNN "MPN"
F 5 "y" H 1240 2050 50  0001 C CNN "populate"
	1    1400 1700
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:7796-RESCUE-BodyController_01 J6
U 1 1 5A39455B
P 1400 2150
F 0 "J6" H 1240 2400 60  0000 C CNN
F 1 "7796" H 1260 2100 60  0000 C CNN
F 2 "Footprints:7796" H 1360 2180 60  0001 C CNN
F 3 "http://www.keyelco.com/product-pdf.cfm?p=1114" H 1360 2180 60  0001 C CNN
F 4 "7796" H 1240 2500 50  0001 C CNN "MPN"
F 5 "y" H 1240 2500 50  0001 C CNN "populate"
	1    1400 2150
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:Conn_01x01-RESCUE-BodyController_01 J9
U 1 1 5A465A33
P 10550 850
F 0 "J9" H 10550 950 50  0000 C CNN
F 1 "Conn_01x01" H 10550 750 50  0000 C CNN
F 2 "Footprints:MountingHole_3.2mm_M3_Pad_Via" H 10550 850 50  0001 C CNN
F 3 "" H 10550 850 50  0001 C CNN
F 4 "n" H 10550 1050 50  0001 C CNN "populate"
	1    10550 850 
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:Conn_01x01-RESCUE-BodyController_01 J10
U 1 1 5A465C82
P 10550 1150
F 0 "J10" H 10550 1250 50  0000 C CNN
F 1 "Conn_01x01" H 10550 1050 50  0000 C CNN
F 2 "Footprints:MountingHole_3.2mm_M3_Pad_Via" H 10550 1150 50  0001 C CNN
F 3 "" H 10550 1150 50  0001 C CNN
F 4 "n" H 10550 1350 50  0001 C CNN "populate"
	1    10550 1150
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:Conn_01x01-RESCUE-BodyController_01 J11
U 1 1 5A465CB6
P 10550 1450
F 0 "J11" H 10550 1550 50  0000 C CNN
F 1 "Conn_01x01" H 10550 1350 50  0000 C CNN
F 2 "Footprints:MountingHole_3.2mm_M3_Pad_Via" H 10550 1450 50  0001 C CNN
F 3 "" H 10550 1450 50  0001 C CNN
F 4 "n" H 10550 1650 50  0001 C CNN "populate"
	1    10550 1450
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:Conn_01x01-RESCUE-BodyController_01 J12
U 1 1 5A465CE9
P 10550 1750
F 0 "J12" H 10550 1850 50  0000 C CNN
F 1 "Conn_01x01" H 10550 1650 50  0000 C CNN
F 2 "Footprints:MountingHole_3.2mm_M3_Pad_Via" H 10550 1750 50  0001 C CNN
F 3 "" H 10550 1750 50  0001 C CNN
F 4 "n" H 10550 1950 50  0001 C CNN "populate"
	1    10550 1750
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:Conn_01x01-RESCUE-BodyController_01 J13
U 1 1 5A4665B9
P 10550 2050
F 0 "J13" H 10550 2150 50  0000 C CNN
F 1 "Conn_01x01" H 10550 1950 50  0000 C CNN
F 2 "Footprints:MountingHole_3.2mm_M3_Pad_Via" H 10550 2050 50  0001 C CNN
F 3 "" H 10550 2050 50  0001 C CNN
F 4 "n" H 10550 2250 50  0001 C CNN "populate"
	1    10550 2050
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:GND-RESCUE-BodyController_01 #PWR0132
U 1 1 5A49F5A7
P 4800 3500
F 0 "#PWR0132" H 4800 3250 50  0001 C CNN
F 1 "GND" H 4800 3350 50  0000 C CNN
F 2 "" H 4800 3500 50  0001 C CNN
F 3 "" H 4800 3500 50  0001 C CNN
	1    4800 3500
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:GND-RESCUE-BodyController_01 #PWR0133
U 1 1 5A49F6E5
P 5850 6900
F 0 "#PWR0133" H 5850 6650 50  0001 C CNN
F 1 "GND" H 5850 6750 50  0000 C CNN
F 2 "" H 5850 6900 50  0001 C CNN
F 3 "" H 5850 6900 50  0001 C CNN
	1    5850 6900
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:GND-RESCUE-BodyController_01 #PWR0134
U 1 1 5A49F80B
P 6400 4600
F 0 "#PWR0134" H 6400 4350 50  0001 C CNN
F 1 "GND" H 6400 4450 50  0000 C CNN
F 2 "" H 6400 4600 50  0001 C CNN
F 3 "" H 6400 4600 50  0001 C CNN
	1    6400 4600
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:+5VP-RESCUE-BodyController_01 #PWR0135
U 1 1 5A49F8B9
P 6400 5100
F 0 "#PWR0135" H 6400 4950 50  0001 C CNN
F 1 "+5VP" H 6400 5240 50  0000 C CNN
F 2 "" H 6400 5100 50  0001 C CNN
F 3 "" H 6400 5100 50  0001 C CNN
	1    6400 5100
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:R-RESCUE-BodyController_01 R110
U 1 1 5A4AD53E
P 1600 3200
F 0 "R110" V 1680 3200 50  0000 C CNN
F 1 "100R" V 1600 3200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1530 3200 50  0001 C CNN
F 3 "http://www.yageo.com/NewPortal/yageodocoutput?fileName=/pdf/R-Chip/PYu-AC_51_RoHS_L_6.pdf" H 1600 3200 50  0001 C CNN
F 4 "AC0603FR-07100RL" H 1680 3300 50  0001 C CNN "MPN"
F 5 "y" H 1680 3300 50  0001 C CNN "populate"
	1    1600 3200
	0    1    1    0   
$EndComp
$Comp
L BodyController_01-rescue:R-RESCUE-BodyController_01 R109
U 1 1 5A4AD59D
P 1350 3300
F 0 "R109" V 1430 3300 50  0000 C CNN
F 1 "100R" V 1350 3300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1280 3300 50  0001 C CNN
F 3 "http://www.yageo.com/NewPortal/yageodocoutput?fileName=/pdf/R-Chip/PYu-AC_51_RoHS_L_6.pdf" H 1350 3300 50  0001 C CNN
F 4 "AC0603FR-07100RL" H 1430 3400 50  0001 C CNN "MPN"
F 5 "y" H 1430 3400 50  0001 C CNN "populate"
	1    1350 3300
	0    1    1    0   
$EndComp
$Comp
L BodyController_01-rescue:C-RESCUE-BodyController_01 C115
U 1 1 5A4AD7E0
P 1800 3650
F 0 "C115" H 1825 3750 50  0000 L CNN
F 1 "10nF" H 1825 3550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1838 3500 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2X7R1H103K080AA.pdf" H 1800 3650 50  0001 C CNN
F 4 "CGA3E2X7R1H103K080AA" H 1825 3850 50  0001 C CNN "MPN"
F 5 "y" H 1825 3850 50  0001 C CNN "populate"
	1    1800 3650
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:C-RESCUE-BodyController_01 C114
U 1 1 5A4AD849
P 1550 3650
F 0 "C114" H 1575 3750 50  0000 L CNN
F 1 "10nF" H 1575 3550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1588 3500 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2X7R1H103K080AA.pdf" H 1550 3650 50  0001 C CNN
F 4 "CGA3E2X7R1H103K080AA" H 1575 3850 50  0001 C CNN "MPN"
F 5 "y" H 1575 3850 50  0001 C CNN "populate"
	1    1550 3650
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:GND-RESCUE-BodyController_01 #PWR0136
U 1 1 5A4AD94A
P 1550 3850
F 0 "#PWR0136" H 1550 3600 50  0001 C CNN
F 1 "GND" H 1550 3700 50  0000 C CNN
F 2 "" H 1550 3850 50  0001 C CNN
F 3 "" H 1550 3850 50  0001 C CNN
	1    1550 3850
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:GND-RESCUE-BodyController_01 #PWR0137
U 1 1 5A4AD988
P 1800 3850
F 0 "#PWR0137" H 1800 3600 50  0001 C CNN
F 1 "GND" H 1800 3700 50  0000 C CNN
F 2 "" H 1800 3850 50  0001 C CNN
F 3 "" H 1800 3850 50  0001 C CNN
	1    1800 3850
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:TEST_1P-RESCUE-BodyController_01 TP6
U 1 1 5A527FBB
P 9400 1100
F 0 "TP6" H 9400 1370 50  0000 C CNN
F 1 "TEST_1P" H 9400 1300 50  0000 C CNN
F 2 "Measurement_Points:Test_Point_Keystone_5019_Minature" H 9600 1100 50  0001 C CNN
F 3 "http://www.keyelco.com/product-pdf.cfm?p=1353" H 9600 1100 50  0001 C CNN
F 4 "5015" H 9400 1470 50  0001 C CNN "MPN"
F 5 "y" H 9400 1470 50  0001 C CNN "populate"
	1    9400 1100
	-1   0    0    1   
$EndComp
$Comp
L BodyController_01-rescue:TEST_1P-RESCUE-BodyController_01 TP5
U 1 1 5A5280F8
P 8950 1100
F 0 "TP5" H 8950 1370 50  0000 C CNN
F 1 "TEST_1P" H 8950 1300 50  0000 C CNN
F 2 "Measurement_Points:Test_Point_Keystone_5019_Minature" H 9150 1100 50  0001 C CNN
F 3 "http://www.keyelco.com/product-pdf.cfm?p=1353" H 9150 1100 50  0001 C CNN
F 4 "5015" H 8950 1470 50  0001 C CNN "MPN"
F 5 "y" H 8950 1470 50  0001 C CNN "populate"
	1    8950 1100
	-1   0    0    1   
$EndComp
$Comp
L BodyController_01-rescue:TEST_1P-RESCUE-BodyController_01 TP4
U 1 1 5A528147
P 8500 1100
F 0 "TP4" H 8500 1370 50  0000 C CNN
F 1 "TEST_1P" H 8500 1300 50  0000 C CNN
F 2 "Measurement_Points:Test_Point_Keystone_5019_Minature" H 8700 1100 50  0001 C CNN
F 3 "http://www.keyelco.com/product-pdf.cfm?p=1353" H 8700 1100 50  0001 C CNN
F 4 "5015" H 8500 1470 50  0001 C CNN "MPN"
F 5 "y" H 8500 1470 50  0001 C CNN "populate"
	1    8500 1100
	-1   0    0    1   
$EndComp
$Comp
L BodyController_01-rescue:TEST_1P-RESCUE-BodyController_01 TP3
U 1 1 5A5281E7
P 8050 1100
F 0 "TP3" H 8050 1370 50  0000 C CNN
F 1 "TEST_1P" H 8050 1300 50  0000 C CNN
F 2 "Measurement_Points:Test_Point_Keystone_5019_Minature" H 8250 1100 50  0001 C CNN
F 3 "http://www.keyelco.com/product-pdf.cfm?p=1353" H 8250 1100 50  0001 C CNN
F 4 "5015" H 8050 1470 50  0001 C CNN "MPN"
F 5 "y" H 8050 1470 50  0001 C CNN "populate"
	1    8050 1100
	-1   0    0    1   
$EndComp
$Comp
L BodyController_01-rescue:TEST_1P-RESCUE-BodyController_01 TP2
U 1 1 5A528244
P 7600 1100
F 0 "TP2" H 7600 1370 50  0000 C CNN
F 1 "TEST_1P" H 7600 1300 50  0000 C CNN
F 2 "Measurement_Points:Test_Point_Keystone_5019_Minature" H 7800 1100 50  0001 C CNN
F 3 "http://www.keyelco.com/product-pdf.cfm?p=1353" H 7800 1100 50  0001 C CNN
F 4 "5015" H 7600 1470 50  0001 C CNN "MPN"
F 5 "y" H 7600 1470 50  0001 C CNN "populate"
	1    7600 1100
	-1   0    0    1   
$EndComp
$Comp
L BodyController_01-rescue:+5VP-RESCUE-BodyController_01 #PWR0138
U 1 1 5A52849A
P 9400 1100
F 0 "#PWR0138" H 9400 950 50  0001 C CNN
F 1 "+5VP" H 9400 1240 50  0000 C CNN
F 2 "" H 9400 1100 50  0001 C CNN
F 3 "" H 9400 1100 50  0001 C CNN
	1    9400 1100
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:+3.3VP-RESCUE-BodyController_01 #PWR0139
U 1 1 5A528725
P 8950 1100
F 0 "#PWR0139" H 9100 1050 50  0001 C CNN
F 1 "+3.3VP" H 8950 1200 50  0000 C CNN
F 2 "" H 8950 1100 50  0001 C CNN
F 3 "" H 8950 1100 50  0001 C CNN
	1    8950 1100
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:+5V-RESCUE-BodyController_01 #PWR0140
U 1 1 5A528765
P 8500 1100
F 0 "#PWR0140" H 8500 950 50  0001 C CNN
F 1 "+5V" H 8500 1240 50  0000 C CNN
F 2 "" H 8500 1100 50  0001 C CNN
F 3 "" H 8500 1100 50  0001 C CNN
	1    8500 1100
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:+3.3V-RESCUE-BodyController_01 #PWR0141
U 1 1 5A5287A5
P 8050 1100
F 0 "#PWR0141" H 8050 950 50  0001 C CNN
F 1 "+3.3V" H 8050 1240 50  0000 C CNN
F 2 "" H 8050 1100 50  0001 C CNN
F 3 "" H 8050 1100 50  0001 C CNN
	1    8050 1100
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:+12P-RESCUE-BodyController_01 #PWR0142
U 1 1 5A528E69
P 7600 1100
F 0 "#PWR0142" H 7600 950 50  0001 C CNN
F 1 "+12P" H 7600 1240 50  0000 C CNN
F 2 "" H 7600 1100 50  0001 C CNN
F 3 "" H 7600 1100 50  0001 C CNN
	1    7600 1100
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:TEST_1P-RESCUE-BodyController_01 TP1
U 1 1 5A57BD55
P 7150 1100
F 0 "TP1" H 7150 1370 50  0000 C CNN
F 1 "TEST_1P" H 7150 1300 50  0000 C CNN
F 2 "Measurement_Points:Test_Point_Keystone_5019_Minature" H 7350 1100 50  0001 C CNN
F 3 "http://www.keyelco.com/product-pdf.cfm?p=1353" H 7350 1100 50  0001 C CNN
F 4 "5015" H 7150 1470 50  0001 C CNN "MPN"
F 5 "y" H 7150 1470 50  0001 C CNN "populate"
	1    7150 1100
	-1   0    0    1   
$EndComp
$Comp
L BodyController_01-rescue:+12V-RESCUE-BodyController_01 #PWR0143
U 1 1 5A57BE42
P 7150 1100
F 0 "#PWR0143" H 7150 950 50  0001 C CNN
F 1 "+12V" H 7150 1240 50  0000 C CNN
F 2 "" H 7150 1100 50  0001 C CNN
F 3 "" H 7150 1100 50  0001 C CNN
	1    7150 1100
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:TEST_1P-RESCUE-BodyController_01 TP7
U 1 1 5A57C95A
P 9800 1100
F 0 "TP7" H 9800 1370 50  0000 C CNN
F 1 "TEST_1P" H 9800 1300 50  0000 C CNN
F 2 "Measurement_Points:Test_Point_Keystone_5019_Minature" H 10000 1100 50  0001 C CNN
F 3 "http://www.keyelco.com/product-pdf.cfm?p=1353" H 10000 1100 50  0001 C CNN
F 4 "5015" H 9800 1470 50  0001 C CNN "MPN"
F 5 "y" H 9800 1470 50  0001 C CNN "populate"
	1    9800 1100
	-1   0    0    1   
$EndComp
$Comp
L BodyController_01-rescue:GND-RESCUE-BodyController_01 #PWR0144
U 1 1 5A57CA66
P 9800 1100
F 0 "#PWR0144" H 9800 850 50  0001 C CNN
F 1 "GND" H 9800 950 50  0000 C CNN
F 2 "" H 9800 1100 50  0001 C CNN
F 3 "" H 9800 1100 50  0001 C CNN
	1    9800 1100
	-1   0    0    1   
$EndComp
$Comp
L BodyController_01-rescue:GND-RESCUE-BodyController_01 #PWR0145
U 1 1 5A6303D8
P 10300 2450
F 0 "#PWR0145" H 10300 2200 50  0001 C CNN
F 1 "GND" H 10300 2300 50  0000 C CNN
F 2 "" H 10300 2450 50  0001 C CNN
F 3 "" H 10300 2450 50  0001 C CNN
	1    10300 2450
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:GND-RESCUE-BodyController_01 #PWR0146
U 1 1 5A6309A6
P 1300 6050
F 0 "#PWR0146" H 1300 5800 50  0001 C CNN
F 1 "GND" H 1300 5900 50  0000 C CNN
F 2 "" H 1300 6050 50  0001 C CNN
F 3 "" H 1300 6050 50  0001 C CNN
	1    1300 6050
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:C-RESCUE-BodyController_01 C137
U 1 1 5A630A73
P 1300 5850
F 0 "C137" H 1325 5950 50  0000 L CNN
F 1 "10nF" H 1325 5750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1338 5700 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2X7R1H103K080AA.pdf" H 1300 5850 50  0001 C CNN
F 4 "CGA3E2X7R1H103K080AA" H 1325 6050 50  0001 C CNN "MPN"
F 5 "y" H 1325 6050 50  0001 C CNN "populate"
	1    1300 5850
	1    0    0    -1  
$EndComp
$Comp
L BodyController_01-rescue:TEST_1P-RESCUE-BodyController_01 TP8
U 1 1 5A7E886F
P 6700 1100
F 0 "TP8" H 6700 1370 50  0000 C CNN
F 1 "TEST_1P" H 6700 1300 50  0000 C CNN
F 2 "Measurement_Points:Test_Point_Keystone_5019_Minature" H 6900 1100 50  0001 C CNN
F 3 "http://www.keyelco.com/product-pdf.cfm?p=1353" H 6900 1100 50  0001 C CNN
F 4 "5015" H 6700 1470 50  0001 C CNN "MPN"
F 5 "y" H 6700 1470 50  0001 C CNN "populate"
	1    6700 1100
	-1   0    0    1   
$EndComp
Text GLabel 4200 4600 1    59   Input ~ 0
AUX_PORT_OUT
Text GLabel 3500 4600 1    59   Input ~ 0
HEADLIGHT_HI_OUT
Text GLabel 3400 4600 1    59   Input ~ 0
HEADLIGHT_LO_OUT
Text GLabel 4100 4600 1    59   Input ~ 0
TURN_SIGNAL_FR_OUT
Text GLabel 4000 4600 1    59   Input ~ 0
TURN_SIGNAL_RR_OUT
Text GLabel 3900 4600 1    59   Input ~ 0
TURN_SIGNAL_FL_OUT
Text GLabel 3800 4600 1    59   Input ~ 0
TURN_SIGNAL_RL_OUT
Text GLabel 3700 4600 1    59   Input ~ 0
HEATED_GRIPS_OUT
Text GLabel 3600 4600 1    59   Input ~ 0
HEATED_SEAT_OUT
Text GLabel 3300 4600 1    59   Input ~ 0
TAILLIGHT_OUT
Text GLabel 3200 4600 1    59   Input ~ 0
BRAKE_LIGHT_OUT
Text GLabel 4300 4600 1    59   Input ~ 0
HORN_OUT
Text GLabel 5250 4600 1    59   Input ~ 0
FAN_1_OUT
Text GLabel 5150 4600 1    59   Input ~ 0
PUMP_1_OUT
Text GLabel 5650 5100 3    60   Input ~ 0
RADIATOR_OUTPUT_TEMP_OUT
Text GLabel 5450 5100 3    60   Input ~ 0
RADIATOR_INPUT_TEMP_OUT
Text GLabel 5850 4600 1    59   Input ~ 0
CHARGE_PORT_RELEASE_OUT
Text GLabel 5750 4600 1    59   Input ~ 0
STEERING_COLUMN_LOCK_OUT
Text GLabel 5950 5100 3    59   Input ~ 0
DCDC_CONTACTOR_OUT
Text GLabel 6050 5100 3    59   Input ~ 0
BATTERY_CONTACTOR_OUT
Text GLabel 6050 4600 1    59   Input ~ 0
PRECHARGE_RELAY_OUT
Text GLabel 5950 4600 1    59   Input ~ 0
CHARGER_CONTACTOR_OUT
Text GLabel 5450 4600 1    59   Input ~ 0
MOTOR_CONTROLLER_OUT
Text GLabel 5350 4600 1    59   Input ~ 0
CHARGE_CONTROLLER_OUT
Text GLabel 5650 4600 1    59   Input ~ 0
BMS_CONTROLLER_OUT
Text GLabel 4500 4600 1    59   Input ~ 0
IC_CONTROLLER_OUT
Text GLabel 2000 1600 2    60   Output ~ 0
DCDC
Text GLabel 7400 5100 3    60   BiDi ~ 0
CAN_P
Text GLabel 7500 5100 3    60   BiDi ~ 0
CAN_N
Text GLabel 1900 2900 0    60   Input ~ 0
~RESET
Text GLabel 1150 3300 0    60   Input ~ 0
PGEC
Text GLabel 1150 3200 0    60   Input ~ 0
PGED
Text GLabel 5550 4600 1    59   Input ~ 0
J1772_CONTROLLER_OUT
Text GLabel 5150 5100 3    60   Input ~ 0
PUMP_SPEED_1_OUT
Text GLabel 5250 5100 3    60   Input ~ 0
FAN_SPEED_1_OUT
Text GLabel 2050 5000 2    60   Output ~ 0
USB_5V
Text GLabel 2050 5200 2    60   BiDi ~ 0
USB_D+
Text GLabel 2050 5300 2    60   BiDi ~ 0
USB_D-
Text GLabel 5550 5100 3    60   Input ~ 0
RADIATOR_OUTPUT_TEMP_RTN
Text GLabel 5350 5100 3    60   Input ~ 0
RADIATOR_INPUT_TEMP_RTN
Text GLabel 7400 4600 1    60   Output ~ 0
INGITION_SWITCH
Text GLabel 7500 4600 1    60   Output ~ 0
KILL_SWITCH
Text GLabel 6700 4600 1    60   Output ~ 0
TURN_LEFT_SWITCH
Text GLabel 6800 4600 1    60   Output ~ 0
TURN_RIGHT_SWITCH
Text GLabel 6900 4600 1    60   Output ~ 0
BRIGHTS_SWITCH
Text GLabel 7000 4600 1    60   Output ~ 0
HORN_SWITCH
Text GLabel 7100 4600 1    60   Output ~ 0
KICKSTAND_SWITCH
Text GLabel 7200 4600 1    60   Output ~ 0
SPARE_SWITCH_1
Text GLabel 7300 4600 1    60   Output ~ 0
SPARE_SWITCH_2
Text GLabel 6700 5100 3    60   Output ~ 0
THROTTLE_SIGNAL
Text GLabel 7000 5100 3    60   Output ~ 0
BRAKE_SWITCH_1
Text GLabel 7100 5100 3    60   Output ~ 0
BRAKE_SWITCH_2
Text GLabel 2300 2050 2    60   Output ~ 0
GND
Text GLabel 7200 5100 3    60   Input ~ 0
DAC_OUT_1
Text GLabel 7300 5100 3    60   Input ~ 0
DAC_OUT_2
Text GLabel 6800 5100 3    60   Output ~ 0
PILOT
Text GLabel 6900 5100 3    60   Output ~ 0
PROXIMITY
Text GLabel 6500 1050 0    60   BiDi ~ 0
DEBUG_PIN
Wire Wire Line
	2000 2050 2000 2150
Wire Wire Line
	1500 2050 2300 2050
Wire Wire Line
	2000 1150 2000 1100
Wire Wire Line
	1500 1150 2000 1150
Wire Wire Line
	2000 1600 1500 1600
Wire Wire Line
	1900 3100 650  3100
Wire Wire Line
	650  3100 650  3150
Wire Wire Line
	1900 3000 650  3000
Wire Wire Line
	650  3000 650  2950
Wire Wire Line
	2050 5000 1900 5000
Wire Wire Line
	2050 5200 1900 5200
Wire Wire Line
	2050 5300 1900 5300
Wire Wire Line
	4400 4600 4400 3400
Wire Wire Line
	4400 3400 4800 3400
Wire Wire Line
	4800 3400 4800 3500
Wire Wire Line
	5850 5100 5850 6900
Wire Wire Line
	5750 5100 5750 5250
Wire Wire Line
	5750 5250 5850 5250
Wire Wire Line
	6600 5100 6600 5200
Wire Wire Line
	6600 5200 6400 5200
Wire Wire Line
	6400 5200 6400 5100
Wire Wire Line
	1750 3200 1900 3200
Wire Wire Line
	1450 3200 1150 3200
Wire Wire Line
	1200 3300 1150 3300
Wire Wire Line
	1500 3300 1900 3300
Wire Wire Line
	1550 3300 1550 3500
Wire Wire Line
	1800 3850 1800 3800
Wire Wire Line
	1550 3800 1550 3850
Wire Wire Line
	1800 3500 1800 3200
Wire Wire Line
	6600 4600 6600 4450
Wire Wire Line
	6600 4450 6400 4450
Wire Wire Line
	6400 4450 6400 4600
Wire Wire Line
	10300 850  10300 2450
Wire Wire Line
	10300 850  10350 850 
Wire Wire Line
	10350 1150 10300 1150
Wire Wire Line
	10350 1450 10300 1450
Wire Wire Line
	10350 1750 10300 1750
Wire Wire Line
	10350 2050 10300 2050
Wire Wire Line
	1600 5600 1600 5650
Wire Wire Line
	1500 5600 1500 5650
Wire Wire Line
	1500 5650 1300 5650
Wire Wire Line
	1300 6000 1300 6050
Wire Wire Line
	1300 5650 1300 5700
Wire Wire Line
	6500 1050 6700 1050
Wire Wire Line
	6700 1050 6700 1100
Connection ~ 1650 1150
Connection ~ 1650 2050
Connection ~ 2000 2050
Connection ~ 5850 5250
Connection ~ 1550 3300
Connection ~ 1800 3200
Connection ~ 10300 1150
Connection ~ 10300 1450
Connection ~ 10300 1750
Connection ~ 10300 2050
NoConn ~ 1900 3400
NoConn ~ 1900 5400
$EndSCHEMATC
