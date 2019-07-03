EESchema Schematic File Version 4
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 8
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
L power:PRI_HI #PWR?
U 1 1 5D47053F
P 2500 4950
AR Path="/5D47053F" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D47053F" Ref="#PWR047"  Part="1" 
F 0 "#PWR047" H 2500 4800 50  0001 C CNN
F 1 "PRI_HI" H 2517 5123 50  0000 C CNN
F 2 "" H 2500 4950 50  0001 C CNN
F 3 "" H 2500 4950 50  0001 C CNN
	1    2500 4950
	1    0    0    -1  
$EndComp
$Comp
L power:PRI_LO #PWR?
U 1 1 5D470545
P 2700 5150
AR Path="/5D470545" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D470545" Ref="#PWR048"  Part="1" 
F 0 "#PWR048" H 2700 5000 50  0001 C CNN
F 1 "PRI_LO" H 2717 5323 50  0000 C CNN
F 2 "" H 2700 5150 50  0001 C CNN
F 3 "" H 2700 5150 50  0001 C CNN
	1    2700 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 5150 2700 5200
Wire Wire Line
	2700 5200 2050 5200
Wire Wire Line
	2500 4950 2500 5000
Wire Wire Line
	2500 5000 2050 5000
Text GLabel 2400 5750 2    50   Input ~ 0
MOT_+
Text GLabel 2400 6950 2    50   Input ~ 0
WELD_BUTTON_OUT
$Comp
L power:GND #PWR?
U 1 1 5D470551
P 2050 7050
AR Path="/5D470551" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D470551" Ref="#PWR044"  Part="1" 
F 0 "#PWR044" H 2050 6800 50  0001 C CNN
F 1 "GND" V 2055 6922 50  0000 R CNN
F 2 "" H 2050 7050 50  0001 C CNN
F 3 "" H 2050 7050 50  0001 C CNN
	1    2050 7050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2400 5750 2050 5750
$Comp
L Connector:Screw_Terminal_01x03 J?
U 1 1 5D470559
P 1850 5100
AR Path="/5D470559" Ref="J?"  Part="1" 
AR Path="/5D4704B3/5D470559" Ref="J2"  Part="1" 
F 0 "J2" H 1770 4775 50  0000 C CNN
F 1 "Screw_Terminal_01x03" H 1770 4866 50  0000 C CNN
F 2 "Connectors_Phoenix:PhoenixContact_MSTBVA-G_03x5.00mm_Vertical" H 1850 5100 50  0001 C CNN
F 3 "http://www.on-shore.com/wp-content/uploads/OSTTCXX2162.pdf" H 1850 5100 50  0001 C CNN
F 4 "OSTTC032162" H 1850 5100 50  0001 C CNN "PartNumber"
	1    1850 5100
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D470560
P 2050 5100
AR Path="/5D470560" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D470560" Ref="#PWR043"  Part="1" 
F 0 "#PWR043" H 2050 4850 50  0001 C CNN
F 1 "GND" V 2055 4972 50  0000 R CNN
F 2 "" H 2050 5100 50  0001 C CNN
F 3 "" H 2050 5100 50  0001 C CNN
	1    2050 5100
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 5D470567
P 1850 5750
AR Path="/5D470567" Ref="J?"  Part="1" 
AR Path="/5D4704B3/5D470567" Ref="J3"  Part="1" 
F 0 "J3" H 1770 5425 50  0000 C CNN
F 1 "Screw_Terminal_01x02" H 1770 5516 50  0000 C CNN
F 2 "Connectors_Phoenix:PhoenixContact_MSTBVA-G_02x5.00mm_Vertical" H 1850 5750 50  0001 C CNN
F 3 "http://www.on-shore.com/wp-content/uploads/OSTTCXX2162.pdf" H 1850 5750 50  0001 C CNN
F 4 "OSTTC022162" H 650 1200 50  0001 C CNN "PartNumber"
	1    1850 5750
	-1   0    0    1   
$EndComp
$Comp
L power:PRI_LO #PWR?
U 1 1 5D47056E
P 2700 5600
AR Path="/5D47056E" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D47056E" Ref="#PWR049"  Part="1" 
F 0 "#PWR049" H 2700 5450 50  0001 C CNN
F 1 "PRI_LO" H 2717 5773 50  0000 C CNN
F 2 "" H 2700 5600 50  0001 C CNN
F 3 "" H 2700 5600 50  0001 C CNN
	1    2700 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 5600 2700 5650
Wire Wire Line
	2700 5650 2050 5650
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 5D470577
P 1850 7050
AR Path="/5D470577" Ref="J?"  Part="1" 
AR Path="/5D4704B3/5D470577" Ref="J4"  Part="1" 
F 0 "J4" H 1770 6725 50  0000 C CNN
F 1 "Screw_Terminal_01x02" H 1770 6816 50  0000 C CNN
F 2 "Connectors_Phoenix:PhoenixContact_MSTBVA-G_02x5.00mm_Vertical" H 1850 7050 50  0001 C CNN
F 3 "http://www.on-shore.com/wp-content/uploads/OSTTCXX2162.pdf" H 1850 7050 50  0001 C CNN
F 4 "OSTTC022162" H 650 2500 50  0001 C CNN "PartNumber"
	1    1850 7050
	-1   0    0    1   
$EndComp
Wire Wire Line
	2050 6950 2400 6950
$Comp
L Connector_Generic:Conn_01x08 J?
U 1 1 5D47057F
P 1850 3400
AR Path="/5D47057F" Ref="J?"  Part="1" 
AR Path="/5D4704B3/5D47057F" Ref="J1"  Part="1" 
F 0 "J1" H 1770 2775 50  0000 C CNN
F 1 "Conn_01x08" H 1770 2866 50  0000 C CNN
F 2 "Connectors_Samtec:SL-108-X-XX_1x08" H 1850 3400 50  0001 C CNN
F 3 "~" H 1850 3400 50  0001 C CNN
	1    1850 3400
	-1   0    0    1   
$EndComp
Text GLabel 2650 3600 2    50   BiDi ~ 0
SCL
Text GLabel 2650 3500 2    50   BiDi ~ 0
SDA
$Comp
L power:GND #PWR?
U 1 1 5D470588
P 2100 3750
AR Path="/5D470588" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D470588" Ref="#PWR045"  Part="1" 
F 0 "#PWR045" H 2100 3500 50  0001 C CNN
F 1 "GND" V 2105 3622 50  0000 R CNN
F 2 "" H 2100 3750 50  0001 C CNN
F 3 "" H 2100 3750 50  0001 C CNN
	1    2100 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 3750 2100 3700
Wire Wire Line
	2100 3700 2050 3700
$Comp
L power:+5V #PWR?
U 1 1 5D470590
P 2350 2850
AR Path="/5D470590" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D470590" Ref="#PWR046"  Part="1" 
F 0 "#PWR046" H 2350 2700 50  0001 C CNN
F 1 "+5V" H 2365 3023 50  0000 C CNN
F 2 "" H 2350 2850 50  0001 C CNN
F 3 "" H 2350 2850 50  0001 C CNN
	1    2350 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 2850 2350 3000
Wire Wire Line
	2350 3000 2050 3000
Text GLabel 2650 3400 2    50   Input ~ 0
CS
Text GLabel 2650 3300 2    50   Input ~ 0
MOSI
Text GLabel 2650 3200 2    50   Output ~ 0
MISO
Text GLabel 2650 3100 2    50   Input ~ 0
CLK
Wire Wire Line
	2650 3100 2050 3100
Wire Wire Line
	2650 3200 2050 3200
Wire Wire Line
	2650 3300 2050 3300
Wire Wire Line
	2650 3400 2050 3400
Wire Wire Line
	2650 3500 2050 3500
Wire Wire Line
	2650 3600 2050 3600
$Comp
L device:D_Schottky_x2_Serial_AKC D?
U 1 1 5D47185C
P 8850 3600
AR Path="/5D47185C" Ref="D?"  Part="1" 
AR Path="/5D4675EF/5D47185C" Ref="D?"  Part="1" 
AR Path="/5D46DEF4/5D47185C" Ref="D?"  Part="1" 
AR Path="/5D4704B3/5D47185C" Ref="D9"  Part="1" 
F 0 "D9" V 8896 3678 50  0000 L CNN
F 1 "D_Schottky_x2_Serial_AKC" V 8805 3678 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 8850 3600 50  0001 C CNN
F 3 "https://www.onsemi.com/pub/Collateral/BAS70-04LT1-D.PDF" H 8850 3600 50  0001 C CNN
F 4 "SBAS70-04LT1G" H 0   0   50  0001 C CNN "PartNumber"
	1    8850 3600
	0    -1   -1   0   
$EndComp
$Comp
L device:D_Schottky_x2_Serial_AKC D?
U 1 1 5D471A46
P 10400 3600
AR Path="/5D471A46" Ref="D?"  Part="1" 
AR Path="/5D4675EF/5D471A46" Ref="D?"  Part="1" 
AR Path="/5D46DEF4/5D471A46" Ref="D?"  Part="1" 
AR Path="/5D4704B3/5D471A46" Ref="D12"  Part="1" 
F 0 "D12" V 10446 3678 50  0000 L CNN
F 1 "D_Schottky_x2_Serial_AKC" V 10355 3678 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 10400 3600 50  0001 C CNN
F 3 "https://www.onsemi.com/pub/Collateral/BAS70-04LT1-D.PDF" H 10400 3600 50  0001 C CNN
F 4 "SBAS70-04LT1G" H 0   0   50  0001 C CNN "PartNumber"
	1    10400 3600
	0    -1   -1   0   
$EndComp
$Comp
L device:D_Schottky_x2_Serial_AKC D?
U 1 1 5D471A9E
P 8800 2300
AR Path="/5D471A9E" Ref="D?"  Part="1" 
AR Path="/5D4675EF/5D471A9E" Ref="D?"  Part="1" 
AR Path="/5D46DEF4/5D471A9E" Ref="D?"  Part="1" 
AR Path="/5D4704B3/5D471A9E" Ref="D8"  Part="1" 
F 0 "D8" V 8846 2378 50  0000 L CNN
F 1 "D_Schottky_x2_Serial_AKC" V 8755 2378 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 8800 2300 50  0001 C CNN
F 3 "https://www.onsemi.com/pub/Collateral/BAS70-04LT1-D.PDF" H 8800 2300 50  0001 C CNN
F 4 "SBAS70-04LT1G" H 0   0   50  0001 C CNN "PartNumber"
	1    8800 2300
	0    -1   -1   0   
$EndComp
$Comp
L device:D_Schottky_x2_Serial_AKC D?
U 1 1 5D471AC4
P 5750 3600
AR Path="/5D471AC4" Ref="D?"  Part="1" 
AR Path="/5D4675EF/5D471AC4" Ref="D?"  Part="1" 
AR Path="/5D46DEF4/5D471AC4" Ref="D?"  Part="1" 
AR Path="/5D4704B3/5D471AC4" Ref="D6"  Part="1" 
F 0 "D6" V 5796 3678 50  0000 L CNN
F 1 "D_Schottky_x2_Serial_AKC" V 5705 3678 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 5750 3600 50  0001 C CNN
F 3 "https://www.onsemi.com/pub/Collateral/BAS70-04LT1-D.PDF" H 5750 3600 50  0001 C CNN
F 4 "SBAS70-04LT1G" H 0   0   50  0001 C CNN "PartNumber"
	1    5750 3600
	0    -1   -1   0   
$EndComp
$Comp
L device:D_Schottky_x2_Serial_AKC D?
U 1 1 5D471B16
P 7250 3600
AR Path="/5D471B16" Ref="D?"  Part="1" 
AR Path="/5D4675EF/5D471B16" Ref="D?"  Part="1" 
AR Path="/5D46DEF4/5D471B16" Ref="D?"  Part="1" 
AR Path="/5D4704B3/5D471B16" Ref="D7"  Part="1" 
F 0 "D7" V 7296 3678 50  0000 L CNN
F 1 "D_Schottky_x2_Serial_AKC" V 7205 3678 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 7250 3600 50  0001 C CNN
F 3 "https://www.onsemi.com/pub/Collateral/BAS70-04LT1-D.PDF" H 7250 3600 50  0001 C CNN
F 4 "SBAS70-04LT1G" H 0   0   50  0001 C CNN "PartNumber"
	1    7250 3600
	0    -1   -1   0   
$EndComp
$Comp
L device:D_Schottky_x2_Serial_AKC D?
U 1 1 5D471D83
P 10350 2300
AR Path="/5D471D83" Ref="D?"  Part="1" 
AR Path="/5D4675EF/5D471D83" Ref="D?"  Part="1" 
AR Path="/5D46DEF4/5D471D83" Ref="D?"  Part="1" 
AR Path="/5D4704B3/5D471D83" Ref="D11"  Part="1" 
F 0 "D11" V 10396 2378 50  0000 L CNN
F 1 "D_Schottky_x2_Serial_AKC" V 10305 2378 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 10350 2300 50  0001 C CNN
F 3 "https://www.onsemi.com/pub/Collateral/BAS70-04LT1-D.PDF" H 10350 2300 50  0001 C CNN
F 4 "SBAS70-04LT1G" H 0   0   50  0001 C CNN "PartNumber"
	1    10350 2300
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D471DBB
P 5750 3900
AR Path="/5D471DBB" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D471DBB" Ref="#PWR051"  Part="1" 
F 0 "#PWR051" H 5750 3650 50  0001 C CNN
F 1 "GND" V 5755 3772 50  0000 R CNN
F 2 "" H 5750 3900 50  0001 C CNN
F 3 "" H 5750 3900 50  0001 C CNN
	1    5750 3900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D471DDA
P 7250 3900
AR Path="/5D471DDA" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D471DDA" Ref="#PWR053"  Part="1" 
F 0 "#PWR053" H 7250 3650 50  0001 C CNN
F 1 "GND" V 7255 3772 50  0000 R CNN
F 2 "" H 7250 3900 50  0001 C CNN
F 3 "" H 7250 3900 50  0001 C CNN
	1    7250 3900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D471DF9
P 8850 3900
AR Path="/5D471DF9" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D471DF9" Ref="#PWR057"  Part="1" 
F 0 "#PWR057" H 8850 3650 50  0001 C CNN
F 1 "GND" V 8855 3772 50  0000 R CNN
F 2 "" H 8850 3900 50  0001 C CNN
F 3 "" H 8850 3900 50  0001 C CNN
	1    8850 3900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D471E18
P 10400 3900
AR Path="/5D471E18" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D471E18" Ref="#PWR063"  Part="1" 
F 0 "#PWR063" H 10400 3650 50  0001 C CNN
F 1 "GND" V 10405 3772 50  0000 R CNN
F 2 "" H 10400 3900 50  0001 C CNN
F 3 "" H 10400 3900 50  0001 C CNN
	1    10400 3900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D471E37
P 8800 2600
AR Path="/5D471E37" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D471E37" Ref="#PWR055"  Part="1" 
F 0 "#PWR055" H 8800 2350 50  0001 C CNN
F 1 "GND" V 8805 2472 50  0000 R CNN
F 2 "" H 8800 2600 50  0001 C CNN
F 3 "" H 8800 2600 50  0001 C CNN
	1    8800 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D471E56
P 10350 2600
AR Path="/5D471E56" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D471E56" Ref="#PWR061"  Part="1" 
F 0 "#PWR061" H 10350 2350 50  0001 C CNN
F 1 "GND" V 10355 2472 50  0000 R CNN
F 2 "" H 10350 2600 50  0001 C CNN
F 3 "" H 10350 2600 50  0001 C CNN
	1    10350 2600
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5D471E75
P 5750 3300
AR Path="/5D471E75" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D471E75" Ref="#PWR050"  Part="1" 
F 0 "#PWR050" H 5750 3150 50  0001 C CNN
F 1 "+5V" H 5765 3473 50  0000 C CNN
F 2 "" H 5750 3300 50  0001 C CNN
F 3 "" H 5750 3300 50  0001 C CNN
	1    5750 3300
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5D471E94
P 7250 3300
AR Path="/5D471E94" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D471E94" Ref="#PWR052"  Part="1" 
F 0 "#PWR052" H 7250 3150 50  0001 C CNN
F 1 "+5V" H 7265 3473 50  0000 C CNN
F 2 "" H 7250 3300 50  0001 C CNN
F 3 "" H 7250 3300 50  0001 C CNN
	1    7250 3300
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5D471EB3
P 8850 3300
AR Path="/5D471EB3" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D471EB3" Ref="#PWR056"  Part="1" 
F 0 "#PWR056" H 8850 3150 50  0001 C CNN
F 1 "+5V" H 8865 3473 50  0000 C CNN
F 2 "" H 8850 3300 50  0001 C CNN
F 3 "" H 8850 3300 50  0001 C CNN
	1    8850 3300
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5D471ED2
P 10400 3300
AR Path="/5D471ED2" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D471ED2" Ref="#PWR062"  Part="1" 
F 0 "#PWR062" H 10400 3150 50  0001 C CNN
F 1 "+5V" H 10415 3473 50  0000 C CNN
F 2 "" H 10400 3300 50  0001 C CNN
F 3 "" H 10400 3300 50  0001 C CNN
	1    10400 3300
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5D471EF1
P 8800 2000
AR Path="/5D471EF1" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D471EF1" Ref="#PWR054"  Part="1" 
F 0 "#PWR054" H 8800 1850 50  0001 C CNN
F 1 "+5V" H 8815 2173 50  0000 C CNN
F 2 "" H 8800 2000 50  0001 C CNN
F 3 "" H 8800 2000 50  0001 C CNN
	1    8800 2000
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5D471F10
P 10350 2000
AR Path="/5D471F10" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D471F10" Ref="#PWR060"  Part="1" 
F 0 "#PWR060" H 10350 1850 50  0001 C CNN
F 1 "+5V" H 10365 2173 50  0000 C CNN
F 2 "" H 10350 2000 50  0001 C CNN
F 3 "" H 10350 2000 50  0001 C CNN
	1    10350 2000
	1    0    0    -1  
$EndComp
Text GLabel 5950 3600 2    50   Input ~ 0
CLK
Text GLabel 7450 3600 2    50   Output ~ 0
MISO
Text GLabel 9050 3600 2    50   Input ~ 0
MOSI
Text GLabel 10600 3600 2    50   Input ~ 0
CS
Text GLabel 9000 2300 2    50   BiDi ~ 0
SDA
Text GLabel 10550 2300 2    50   BiDi ~ 0
SCL
Text GLabel 9950 5100 2    50   Input ~ 0
WELD_BUTTON_OUT
$Comp
L device:D_Schottky_x2_Serial_AKC D?
U 1 1 5D473F71
P 9750 5100
AR Path="/5D473F71" Ref="D?"  Part="1" 
AR Path="/5D4675EF/5D473F71" Ref="D?"  Part="1" 
AR Path="/5D46DEF4/5D473F71" Ref="D?"  Part="1" 
AR Path="/5D4704B3/5D473F71" Ref="D10"  Part="1" 
F 0 "D10" V 9796 5178 50  0000 L CNN
F 1 "D_Schottky_x2_Serial_AKC" V 9705 5178 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 9750 5100 50  0001 C CNN
F 3 "https://www.onsemi.com/pub/Collateral/BAS70-04LT1-D.PDF" H 9750 5100 50  0001 C CNN
F 4 "SBAS70-04LT1G" H 0   0   50  0001 C CNN "PartNumber"
	1    9750 5100
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D473F78
P 9750 5400
AR Path="/5D473F78" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D473F78" Ref="#PWR059"  Part="1" 
F 0 "#PWR059" H 9750 5150 50  0001 C CNN
F 1 "GND" V 9755 5272 50  0000 R CNN
F 2 "" H 9750 5400 50  0001 C CNN
F 3 "" H 9750 5400 50  0001 C CNN
	1    9750 5400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5D473F7E
P 9750 4800
AR Path="/5D473F7E" Ref="#PWR?"  Part="1" 
AR Path="/5D4704B3/5D473F7E" Ref="#PWR058"  Part="1" 
F 0 "#PWR058" H 9750 4650 50  0001 C CNN
F 1 "+5V" H 9765 4973 50  0000 C CNN
F 2 "" H 9750 4800 50  0001 C CNN
F 3 "" H 9750 4800 50  0001 C CNN
	1    9750 4800
	1    0    0    -1  
$EndComp
Text Notes 7550 1400 0    197  ~ 0
ESD Protection
Text Notes 1550 1400 0    197  ~ 0
Connectors
Wire Notes Line
	1150 4500 3300 4500
Wire Notes Line
	3300 4500 3300 6050
Wire Notes Line
	3300 6050 1150 6050
Wire Notes Line
	1150 4500 1150 6050
Text Notes 1150 4450 0    98   ~ 0
HIGH VOLTAGE
$EndSCHEMATC
