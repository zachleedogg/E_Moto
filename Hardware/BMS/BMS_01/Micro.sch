EESchema Schematic File Version 4
LIBS:BMS_01-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 58
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
L device:C C?
U 1 1 59954470
P 1650 6550
AR Path="/5B2FAFE4/59954470" Ref="C?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/59954470" Ref="C?"  Part="1" 
AR Path="/5AAB4D1F/59954470" Ref="C?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/59954470" Ref="C8"  Part="1" 
F 0 "C8" H 1675 6650 50  0000 L CNN
F 1 "10uF" H 1675 6450 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 1688 6400 50  0001 C CNN
F 3 "https://content.kemet.com/datasheets/KEM_C1023_X7R_AUTO_SMD.pdf" H 1650 6550 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "C1206C106M3RACAUTO" H 1675 6750 50  0001 C CNN "MPN"
F 6 "50V" H 0   0   50  0001 C CNN "Voltage"
F 7 "" H 1675 6750 50  0001 C CNN "populate"
	1    1650 6550
	0    1    1    0   
$EndComp
$Comp
L device:Ferrite_Bead L?
U 1 1 59955DBF
P 1250 5900
AR Path="/5B2FAFE4/59955DBF" Ref="L?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/59955DBF" Ref="L?"  Part="1" 
AR Path="/5AAB4D1F/59955DBF" Ref="L?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/59955DBF" Ref="L1"  Part="1" 
F 0 "L1" V 1100 5925 50  0000 C CNN
F 1 "Ferrite_Bead" V 1400 5900 50  0000 C CNN
F 2 "Inductors_SMD:L_0603" V 1180 5900 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/beads_automotive_signal_mmz1608_en.pdf" H 1250 5900 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "MMZ1608B121CTDH5" H 1100 6025 50  0001 C CNN "MPN"
	1    1250 5900
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 59955FD3
P 750 6300
AR Path="/5B2FAFE4/59955FD3" Ref="#PWR?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/59955FD3" Ref="#PWR?"  Part="1" 
AR Path="/5AAB4D1F/59955FD3" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/59955FD3" Ref="#PWR07"  Part="1" 
F 0 "#PWR07" H 750 6050 50  0001 C CNN
F 1 "GND" H 750 6150 50  0000 C CNN
F 2 "" H 750 6300 50  0001 C CNN
F 3 "" H 750 6300 50  0001 C CNN
	1    750  6300
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 599562CF
P 5300 6650
AR Path="/5B2FAFE4/599562CF" Ref="C?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/599562CF" Ref="C?"  Part="1" 
AR Path="/5AAB4D1F/599562CF" Ref="C?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/599562CF" Ref="C10"  Part="1" 
F 0 "C10" H 5325 6750 50  0000 L CNN
F 1 "18pF" H 5325 6550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5338 6500 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2C0G1H180J080AA.pdf" H 5300 6650 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "CGA3E2C0G1H180J080AA" H 5325 6850 50  0001 C CNN "MPN"
F 6 "50V" H 0   0   50  0001 C CNN "Voltage"
F 7 "" H 5325 6850 50  0001 C CNN "populate"
	1    5300 6650
	0    -1   1    0   
$EndComp
$Comp
L device:C C?
U 1 1 59956321
P 5300 7150
AR Path="/5B2FAFE4/59956321" Ref="C?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/59956321" Ref="C?"  Part="1" 
AR Path="/5AAB4D1F/59956321" Ref="C?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/59956321" Ref="C11"  Part="1" 
F 0 "C11" H 5325 7250 50  0000 L CNN
F 1 "18pF" H 5325 7050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5338 7000 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2C0G1H180J080AA.pdf" H 5300 7150 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "CGA3E2C0G1H180J080AA" H 5325 7350 50  0001 C CNN "MPN"
F 6 "50V" H 0   0   50  0001 C CNN "Voltage"
F 7 "" H 5325 7350 50  0001 C CNN "populate"
	1    5300 7150
	0    -1   1    0   
$EndComp
$Comp
L device:R R?
U 1 1 5995635F
P 4150 6900
AR Path="/5B2FAFE4/5995635F" Ref="R?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5995635F" Ref="R?"  Part="1" 
AR Path="/5AAB4D1F/5995635F" Ref="R?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5995635F" Ref="R3"  Part="1" 
F 0 "R3" V 4230 6900 50  0000 C CNN
F 1 "1M" V 4150 6900 50  0000 C CNN
F 2 "Footprints:R_0603_Standard" V 4080 6900 50  0001 C CNN
F 3 "http://www.yageo.com/NewPortal/yageodocoutput?fileName=/pdf/R-Chip/PYu-AC_51_RoHS_L_6.pdf" H 4150 6900 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "AC0603FR-071ML" H 4230 7000 50  0001 C CNN "MPN"
F 6 "" H 4230 7000 50  0001 C CNN "populate"
	1    4150 6900
	-1   0    0    -1  
$EndComp
$Comp
L device:R R?
U 1 1 59967AE3
P 1450 7300
AR Path="/5B2FAFE4/59967AE3" Ref="R?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/59967AE3" Ref="R?"  Part="1" 
AR Path="/5AAB4D1F/59967AE3" Ref="R?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/59967AE3" Ref="R1"  Part="1" 
F 0 "R1" V 1530 7300 50  0000 C CNN
F 1 "10k" V 1450 7300 50  0000 C CNN
F 2 "Footprints:R_0603_Standard" V 1380 7300 50  0001 C CNN
F 3 "http://www.yageo.com/NewPortal/yageodocoutput?fileName=/pdf/R-Chip/PYu-AC_51_RoHS_L_6.pdf" H 1450 7300 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "AC0603FR-0710KL" H 1530 7400 50  0001 C CNN "MPN"
F 6 "" H 1530 7400 50  0001 C CNN "populate"
	1    1450 7300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3VA #PWR?
U 1 1 59977C27
P 1800 5850
AR Path="/5B2FAFE4/59977C27" Ref="#PWR?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/59977C27" Ref="#PWR?"  Part="1" 
AR Path="/5AAB4D1F/59977C27" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/59977C27" Ref="#PWR010"  Part="1" 
F 0 "#PWR010" H 1800 5700 50  0001 C CNN
F 1 "+3.3VA" H 1800 5990 50  0000 C CNN
F 2 "" H 1800 5850 50  0001 C CNN
F 3 "" H 1800 5850 50  0001 C CNN
	1    1800 5850
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG?
U 1 1 59980D1B
P 1500 5750
AR Path="/5B2FAFE4/59980D1B" Ref="#FLG?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/59980D1B" Ref="#FLG?"  Part="1" 
AR Path="/5AAB4D1F/59980D1B" Ref="#FLG?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/59980D1B" Ref="#FLG01"  Part="1" 
F 0 "#FLG01" H 1500 5825 50  0001 C CNN
F 1 "PWR_FLAG" H 1500 5900 50  0000 C CNN
F 2 "" H 1500 5750 50  0001 C CNN
F 3 "" H 1500 5750 50  0001 C CNN
	1    1500 5750
	1    0    0    -1  
$EndComp
$Comp
L micros:PIC33EPxxxMU810 U?
U 1 1 599FC02F
P 8150 2200
AR Path="/5B2FAFE4/599FC02F" Ref="U?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/599FC02F" Ref="U?"  Part="1" 
AR Path="/5AAB4D1F/599FC02F" Ref="U?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/599FC02F" Ref="U1"  Part="1" 
F 0 "U1" H 8900 -1650 60  0000 C CNN
F 1 "PIC33EPxxxMU810" H 8150 -1650 60  0000 C CNN
F 2 "Housings_QFP:TQFP-100_14x14mm_Pitch0.5mm" H 7400 1800 60  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/70616g.pdf" H 7400 1800 60  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "DSPIC33EP512MU810-E/PF" H 8900 -1550 50  0001 C CNN "MPN"
	1    8150 2200
	1    0    0    -1  
$EndComp
$Comp
L micros:PIC33EPxxxMU810 U?
U 2 1 599FC07A
P 2700 3650
AR Path="/5B2FAFE4/599FC07A" Ref="U?"  Part="2" 
AR Path="/5C8FC872/5C8FC883/599FC07A" Ref="U?"  Part="2" 
AR Path="/5AAB4D1F/599FC07A" Ref="U?"  Part="2" 
AR Path="/5B5CD4CD/5B5CD5C1/599FC07A" Ref="U1"  Part="2" 
F 0 "U1" H 3450 -200 60  0000 C CNN
F 1 "PIC33EPxxxMU810" H 2700 -200 60  0000 C CNN
F 2 "Housings_QFP:TQFP-100_14x14mm_Pitch0.5mm" H 1950 3250 60  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/70616g.pdf" H 1950 3250 60  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "DSPIC33EP512MU810-E/PF" H 3450 -100 50  0001 C CNN "MPN"
	2    2700 3650
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 599FDED2
P 1750 4850
AR Path="/5B2FAFE4/599FDED2" Ref="C?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/599FDED2" Ref="C?"  Part="1" 
AR Path="/5AAB4D1F/599FDED2" Ref="C?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/599FDED2" Ref="C9"  Part="1" 
F 0 "C9" H 1775 4950 50  0000 L CNN
F 1 "100nF" H 1775 4750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1788 4700 50  0001 C CNN
F 3 "http://psearch.en.murata.com/capacitor/product/GCM188R71H104KA57%23.pdf" H 1750 4850 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "GCM188R71H104KA57D" H 1775 5050 50  0001 C CNN "MPN"
F 6 "50V" H 0   0   50  0001 C CNN "Voltage"
F 7 "" H 1775 5050 50  0001 C CNN "populate"
	1    1750 4850
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 59B2845D
P 4100 5750
AR Path="/5B2FAFE4/59B2845D" Ref="#PWR?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/59B2845D" Ref="#PWR?"  Part="1" 
AR Path="/5AAB4D1F/59B2845D" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/59B2845D" Ref="#PWR011"  Part="1" 
F 0 "#PWR011" H 4100 5600 50  0001 C CNN
F 1 "+3V3" H 4100 5890 50  0000 C CNN
F 2 "" H 4100 5750 50  0001 C CNN
F 3 "" H 4100 5750 50  0001 C CNN
	1    4100 5750
	1    0    0    -1  
$EndComp
$Comp
L device:Crystal Y?
U 1 1 59BBA021
P 4700 6900
AR Path="/5B2FAFE4/59BBA021" Ref="Y?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/59BBA021" Ref="Y?"  Part="1" 
AR Path="/5AAB4D1F/59BBA021" Ref="Y?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/59BBA021" Ref="Y1"  Part="1" 
F 0 "Y1" V 4825 7100 50  0000 L TNN
F 1 "8MHz" V 4825 7025 50  0000 L BNN
F 2 "Crystals:Crystal_SMD_5032-2pin_5.0x3.2mm" H 4700 6900 50  0001 C CNN
F 3 "https://media.digikey.com/pdf/Data%20Sheets/NDK%20PDFs/NX5032GA_STD-CSU-1,-2.pdf" H 4700 6900 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "NX5032GA-8.000M-STD-CSU-1" H 4825 7200 50  0001 C CNN "MPN"
	1    4700 6900
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5A45E5E7
P 950 7200
AR Path="/5B2FAFE4/5A45E5E7" Ref="SW?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5A45E5E7" Ref="SW?"  Part="1" 
AR Path="/5AAB4D1F/5A45E5E7" Ref="SW?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5A45E5E7" Ref="SW1"  Part="1" 
F 0 "SW1" H 1000 7300 50  0000 L CNN
F 1 "PTS645SM43SMTR92 LFS" H 950 7140 50  0001 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_EVQP0" H 950 7400 50  0001 C CNN
F 3 "https://www.ckswitches.com/media/1471/pts645.pdf" H 950 7400 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "PTS645SM43SMTR92 LFS" H 1000 7400 50  0001 C CNN "MPN"
F 6 "" H 1000 7400 50  0001 C CNN "populate"
	1    950  7200
	1    0    0    -1  
$EndComp
$Comp
L device:R R?
U 1 1 5A870904
P 1700 7600
AR Path="/5B2FAFE4/5A870904" Ref="R?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5A870904" Ref="R?"  Part="1" 
AR Path="/5AAB4D1F/5A870904" Ref="R?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5A870904" Ref="R2"  Part="1" 
F 0 "R2" V 1780 7600 50  0000 C CNN
F 1 "10k" V 1700 7600 50  0000 C CNN
F 2 "Footprints:R_0603_Standard" V 1630 7600 50  0001 C CNN
F 3 "http://www.yageo.com/NewPortal/yageodocoutput?fileName=/pdf/R-Chip/PYu-AC_51_RoHS_L_6.pdf" H 1700 7600 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "AC0603FR-0710KL" H 1780 7700 50  0001 C CNN "MPN"
F 6 "" H 1780 7700 50  0001 C CNN "populate"
	1    1700 7600
	0    1    1    0   
$EndComp
$Comp
L device:C C?
U 1 1 5B784523
P 1550 4850
AR Path="/5B2FAFE4/5B784523" Ref="C?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5B784523" Ref="C?"  Part="1" 
AR Path="/5AAB4D1F/5B784523" Ref="C?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5B784523" Ref="C6"  Part="1" 
F 0 "C6" H 1575 4950 50  0000 L CNN
F 1 "100nF" H 1575 4750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1588 4700 50  0001 C CNN
F 3 "http://psearch.en.murata.com/capacitor/product/GCM188R71H104KA57%23.pdf" H 1550 4850 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "GCM188R71H104KA57D" H 1575 5050 50  0001 C CNN "MPN"
F 6 "50V" H 0   0   50  0001 C CNN "Voltage"
F 7 "" H 1575 5050 50  0001 C CNN "populate"
	1    1550 4850
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5B784555
P 1350 4850
AR Path="/5B2FAFE4/5B784555" Ref="C?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5B784555" Ref="C?"  Part="1" 
AR Path="/5AAB4D1F/5B784555" Ref="C?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5B784555" Ref="C5"  Part="1" 
F 0 "C5" H 1375 4950 50  0000 L CNN
F 1 "100nF" H 1375 4750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1388 4700 50  0001 C CNN
F 3 "http://psearch.en.murata.com/capacitor/product/GCM188R71H104KA57%23.pdf" H 1350 4850 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "GCM188R71H104KA57D" H 1375 5050 50  0001 C CNN "MPN"
F 6 "50V" H 0   0   50  0001 C CNN "Voltage"
F 7 "" H 1375 5050 50  0001 C CNN "populate"
	1    1350 4850
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5B78458D
P 1150 4850
AR Path="/5B2FAFE4/5B78458D" Ref="C?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5B78458D" Ref="C?"  Part="1" 
AR Path="/5AAB4D1F/5B78458D" Ref="C?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5B78458D" Ref="C4"  Part="1" 
F 0 "C4" H 1175 4950 50  0000 L CNN
F 1 "100nF" H 1175 4750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1188 4700 50  0001 C CNN
F 3 "http://psearch.en.murata.com/capacitor/product/GCM188R71H104KA57%23.pdf" H 1150 4850 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "GCM188R71H104KA57D" H 1175 5050 50  0001 C CNN "MPN"
F 6 "50V" H 0   0   50  0001 C CNN "Voltage"
F 7 "" H 1175 5050 50  0001 C CNN "populate"
	1    1150 4850
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5B7845C7
P 950 4850
AR Path="/5B2FAFE4/5B7845C7" Ref="C?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5B7845C7" Ref="C?"  Part="1" 
AR Path="/5AAB4D1F/5B7845C7" Ref="C?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5B7845C7" Ref="C3"  Part="1" 
F 0 "C3" H 975 4950 50  0000 L CNN
F 1 "100nF" H 975 4750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 988 4700 50  0001 C CNN
F 3 "http://psearch.en.murata.com/capacitor/product/GCM188R71H104KA57%23.pdf" H 950 4850 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "GCM188R71H104KA57D" H 975 5050 50  0001 C CNN "MPN"
F 6 "50V" H 0   0   50  0001 C CNN "Voltage"
F 7 "" H 975 5050 50  0001 C CNN "populate"
	1    950  4850
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5B784601
P 750 4850
AR Path="/5B2FAFE4/5B784601" Ref="C?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5B784601" Ref="C?"  Part="1" 
AR Path="/5AAB4D1F/5B784601" Ref="C?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5B784601" Ref="C1"  Part="1" 
F 0 "C1" H 775 4950 50  0000 L CNN
F 1 "100nF" H 775 4750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 788 4700 50  0001 C CNN
F 3 "http://psearch.en.murata.com/capacitor/product/GCM188R71H104KA57%23.pdf" H 750 4850 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "GCM188R71H104KA57D" H 775 5050 50  0001 C CNN "MPN"
F 6 "50V" H 0   0   50  0001 C CNN "Voltage"
F 7 "" H 775 5050 50  0001 C CNN "populate"
	1    750  4850
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5B784684
P 850 6100
AR Path="/5B2FAFE4/5B784684" Ref="C?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5B784684" Ref="C?"  Part="1" 
AR Path="/5AAB4D1F/5B784684" Ref="C?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5B784684" Ref="C2"  Part="1" 
F 0 "C2" H 875 6200 50  0000 L CNN
F 1 "100nF" H 875 6000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 888 5950 50  0001 C CNN
F 3 "http://psearch.en.murata.com/capacitor/product/GCM188R71H104KA57%23.pdf" H 850 6100 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "GCM188R71H104KA57D" H 875 6300 50  0001 C CNN "MPN"
F 6 "50V" H 0   0   50  0001 C CNN "Voltage"
F 7 "" H 875 6300 50  0001 C CNN "populate"
	1    850  6100
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5B7846C6
P 1650 6100
AR Path="/5B2FAFE4/5B7846C6" Ref="C?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5B7846C6" Ref="C?"  Part="1" 
AR Path="/5AAB4D1F/5B7846C6" Ref="C?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5B7846C6" Ref="C7"  Part="1" 
F 0 "C7" H 1675 6200 50  0000 L CNN
F 1 "100nF" H 1675 6000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1688 5950 50  0001 C CNN
F 3 "http://psearch.en.murata.com/capacitor/product/GCM188R71H104KA57%23.pdf" H 1650 6100 50  0001 C CNN
F 4 " " H 0   0   50  0001 C CNN "DNP"
F 5 "GCM188R71H104KA57D" H 1675 6300 50  0001 C CNN "MPN"
F 6 "50V" H 0   0   50  0001 C CNN "Voltage"
F 7 "" H 1675 6300 50  0001 C CNN "populate"
	1    1650 6100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5B7848AF
P 750 5300
AR Path="/5B2FAFE4/5B7848AF" Ref="#PWR?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5B7848AF" Ref="#PWR?"  Part="1" 
AR Path="/5AAB4D1F/5B7848AF" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5B7848AF" Ref="#PWR05"  Part="1" 
F 0 "#PWR05" H 750 5050 50  0001 C CNN
F 1 "GND" H 750 5150 50  0000 C CNN
F 2 "" H 750 5300 50  0001 C CNN
F 3 "" H 750 5300 50  0001 C CNN
	1    750  5300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5B78F263
P 1400 6550
AR Path="/5B2FAFE4/5B78F263" Ref="#PWR?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5B78F263" Ref="#PWR?"  Part="1" 
AR Path="/5AAB4D1F/5B78F263" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5B78F263" Ref="#PWR08"  Part="1" 
F 0 "#PWR08" H 1400 6300 50  0001 C CNN
F 1 "GND" H 1400 6400 50  0000 C CNN
F 2 "" H 1400 6550 50  0001 C CNN
F 3 "" H 1400 6550 50  0001 C CNN
	1    1400 6550
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5B7A4374
P 700 7200
AR Path="/5B2FAFE4/5B7A4374" Ref="#PWR?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5B7A4374" Ref="#PWR?"  Part="1" 
AR Path="/5AAB4D1F/5B7A4374" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5B7A4374" Ref="#PWR03"  Part="1" 
F 0 "#PWR03" H 700 6950 50  0001 C CNN
F 1 "GND" H 700 7050 50  0000 C CNN
F 2 "" H 700 7200 50  0001 C CNN
F 3 "" H 700 7200 50  0001 C CNN
	1    700  7200
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5B7A43A5
P 5600 6900
AR Path="/5B2FAFE4/5B7A43A5" Ref="#PWR?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5B7A43A5" Ref="#PWR?"  Part="1" 
AR Path="/5AAB4D1F/5B7A43A5" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5B7A43A5" Ref="#PWR014"  Part="1" 
F 0 "#PWR014" H 5600 6650 50  0001 C CNN
F 1 "GND" H 5600 6750 50  0000 C CNN
F 2 "" H 5600 6900 50  0001 C CNN
F 3 "" H 5600 6900 50  0001 C CNN
	1    5600 6900
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5B7B965A
P 750 4400
AR Path="/5B2FAFE4/5B7B965A" Ref="#PWR?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5B7B965A" Ref="#PWR?"  Part="1" 
AR Path="/5AAB4D1F/5B7B965A" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5B7B965A" Ref="#PWR04"  Part="1" 
F 0 "#PWR04" H 750 4250 50  0001 C CNN
F 1 "+3V3" H 750 4540 50  0000 C CNN
F 2 "" H 750 4400 50  0001 C CNN
F 3 "" H 750 4400 50  0001 C CNN
	1    750  4400
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5B7B968B
P 750 5900
AR Path="/5B2FAFE4/5B7B968B" Ref="#PWR?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5B7B968B" Ref="#PWR?"  Part="1" 
AR Path="/5AAB4D1F/5B7B968B" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5B7B968B" Ref="#PWR06"  Part="1" 
F 0 "#PWR06" H 750 5750 50  0001 C CNN
F 1 "+3V3" H 750 6040 50  0000 C CNN
F 2 "" H 750 5900 50  0001 C CNN
F 3 "" H 750 5900 50  0001 C CNN
	1    750  5900
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5B7B96BC
P 1450 7050
AR Path="/5B2FAFE4/5B7B96BC" Ref="#PWR?"  Part="1" 
AR Path="/5C8FC872/5C8FC883/5B7B96BC" Ref="#PWR?"  Part="1" 
AR Path="/5AAB4D1F/5B7B96BC" Ref="#PWR?"  Part="1" 
AR Path="/5B5CD4CD/5B5CD5C1/5B7B96BC" Ref="#PWR09"  Part="1" 
F 0 "#PWR09" H 1450 6900 50  0001 C CNN
F 1 "+3V3" H 1450 7190 50  0000 C CNN
F 2 "" H 1450 7050 50  0001 C CNN
F 3 "" H 1450 7050 50  0001 C CNN
	1    1450 7050
	1    0    0    -1  
$EndComp
Text GLabel 9800 2200 2    59   Output ~ 0
SW_EN
Text GLabel 9800 4050 2    59   Output ~ 0
CAN_SLEEP
Text GLabel 9800 3950 2    59   Output ~ 0
CAN_TX
Text GLabel 9800 3850 2    59   Input ~ 0
CAN_RX
Text Notes 1700 950  2    60   ~ 0
TO ADD
Text GLabel 850  7600 0    60   Input ~ 0
nRESET
Text GLabel 5600 4100 0    59   Input ~ 0
PGED
Text GLabel 5600 4200 0    59   Input ~ 0
PGEC
Text GLabel 9800 2550 2    59   Output ~ 0
UART_TX
Text GLabel 9800 2450 2    59   Input ~ 0
UART_RX
Text GLabel 5750 4900 0    60   Output ~ 0
DEBUG_LED
Text GLabel 5750 5000 0    60   BiDi ~ 0
DEBUG_PIN
Wire Wire Line
	950  4700 950  4400
Wire Wire Line
	1150 4700 1150 4400
Wire Wire Line
	1350 4700 1350 4400
Wire Wire Line
	1550 4700 1550 4400
Wire Wire Line
	950  5000 950  5300
Wire Wire Line
	1150 5000 1150 5300
Wire Wire Line
	1350 5000 1350 5300
Wire Wire Line
	1550 5000 1550 5300
Wire Wire Line
	1800 6550 2000 6550
Wire Wire Line
	1500 6550 1400 6550
Wire Wire Line
	1400 5900 1500 5900
Wire Wire Line
	1650 5950 1650 5900
Wire Wire Line
	750  6300 850  6300
Wire Wire Line
	750  5900 850  5900
Wire Wire Line
	850  6300 850  6250
Wire Wire Line
	850  5950 850  5900
Wire Wire Line
	1650 6250 1650 6300
Wire Wire Line
	3950 6650 4150 6650
Wire Wire Line
	3950 7150 4150 7150
Wire Wire Line
	4700 7050 4700 7150
Wire Wire Line
	4150 7050 4150 7150
Wire Wire Line
	4150 6750 4150 6650
Wire Wire Line
	4700 6750 4700 6650
Wire Wire Line
	5500 7150 5450 7150
Wire Wire Line
	5500 6650 5500 6900
Wire Wire Line
	5500 6650 5450 6650
Wire Wire Line
	1450 7050 1450 7150
Wire Wire Line
	1450 7600 1450 7450
Wire Wire Line
	850  7600 1250 7600
Wire Wire Line
	1800 5850 1800 5900
Wire Wire Line
	1500 5750 1500 5900
Wire Wire Line
	750  4400 750  4700
Wire Wire Line
	2000 4400 2000 4500
Wire Wire Line
	2000 5300 2000 5400
Wire Wire Line
	1950 5900 1950 5950
Wire Wire Line
	1950 5950 2000 5950
Wire Wire Line
	1950 6300 1950 6250
Wire Wire Line
	1950 6250 2000 6250
Wire Wire Line
	2000 7100 1900 7100
Wire Wire Line
	1900 7100 1900 7600
Wire Wire Line
	1900 7600 1850 7600
Wire Wire Line
	3950 6650 3950 6750
Wire Wire Line
	3950 6750 3900 6750
Wire Wire Line
	3950 7150 3950 7050
Wire Wire Line
	3950 7050 3900 7050
Wire Wire Line
	1750 4400 1750 4700
Wire Wire Line
	1750 5000 1750 5300
Wire Wire Line
	5750 4200 5600 4200
Wire Wire Line
	5750 4100 5600 4100
Wire Wire Line
	4100 5750 4100 5800
Wire Wire Line
	4100 5800 3900 5800
Wire Wire Line
	5600 6900 5500 6900
Wire Wire Line
	1250 7600 1250 7200
Wire Wire Line
	750  7200 700  7200
Wire Wire Line
	1250 7200 1150 7200
Wire Wire Line
	950  4400 1150 4400
Wire Wire Line
	1150 4400 1350 4400
Wire Wire Line
	1350 4400 1550 4400
Wire Wire Line
	1550 4400 1750 4400
Wire Wire Line
	950  5300 1150 5300
Wire Wire Line
	1150 5300 1350 5300
Wire Wire Line
	1350 5300 1550 5300
Wire Wire Line
	1550 5300 1750 5300
Wire Wire Line
	1650 5900 1800 5900
Wire Wire Line
	850  6300 1650 6300
Wire Wire Line
	850  5900 1100 5900
Wire Wire Line
	1650 6300 1950 6300
Wire Wire Line
	4700 7150 5150 7150
Wire Wire Line
	4150 7150 4700 7150
Wire Wire Line
	4150 6650 4700 6650
Wire Wire Line
	4700 6650 5150 6650
Wire Wire Line
	1450 7600 1550 7600
Wire Wire Line
	1800 5900 1950 5900
Wire Wire Line
	1500 5900 1650 5900
Wire Wire Line
	2000 5400 2000 5500
Wire Wire Line
	2000 5500 2000 5600
Wire Wire Line
	2000 5600 2000 5700
Wire Wire Line
	2000 4800 2000 4900
Wire Wire Line
	2000 4700 2000 4800
Wire Wire Line
	2000 4600 2000 4700
Wire Wire Line
	2000 4500 2000 4600
Wire Wire Line
	1750 4400 2000 4400
Wire Wire Line
	1750 5300 2000 5300
Wire Wire Line
	5500 6900 5500 7150
Wire Wire Line
	1250 7600 1450 7600
Wire Wire Line
	750  5300 950  5300
Wire Wire Line
	750  5000 750  5300
Wire Wire Line
	750  4400 950  4400
Connection ~ 950  4400
Connection ~ 1150 4400
Connection ~ 1350 4400
Connection ~ 1550 4400
Connection ~ 950  5300
Connection ~ 1150 5300
Connection ~ 1350 5300
Connection ~ 1550 5300
Connection ~ 1650 5900
Connection ~ 850  6300
Connection ~ 850  5900
Connection ~ 1650 6300
Connection ~ 4700 7150
Connection ~ 4150 7150
Connection ~ 4150 6650
Connection ~ 4700 6650
Connection ~ 1450 7600
Connection ~ 1800 5900
Connection ~ 1500 5900
Connection ~ 2000 5400
Connection ~ 2000 5500
Connection ~ 2000 5600
Connection ~ 2000 4800
Connection ~ 2000 4700
Connection ~ 2000 4600
Connection ~ 2000 4500
Connection ~ 2000 5300
Connection ~ 1750 4400
Connection ~ 1750 5300
Connection ~ 5500 6900
Connection ~ 2000 4400
Connection ~ 1250 7600
Connection ~ 750  5300
Connection ~ 750  4400
NoConn ~ 3900 6150
Text GLabel 9800 1300 2    59   Output ~ 0
NTC_1_AI
Text GLabel 9800 1400 2    59   Output ~ 0
NTC_2_AI
Text GLabel 5750 3700 0    59   Output ~ 0
NTC_3_AI
Text GLabel 5750 3800 0    59   Output ~ 0
NTC_4_AI
Text GLabel 5750 3900 0    59   Output ~ 0
NTC_5_AI
Text GLabel 5750 4000 0    59   Output ~ 0
NTC_6_AI
Text GLabel 9800 1500 2    59   Output ~ 0
NTC_7_AI
Text GLabel 9800 1600 2    59   Output ~ 0
NTC_8_AI
Text GLabel 5750 2500 0    59   Output ~ 0
NTC_9_AI
Text GLabel 5750 2400 0    59   Output ~ 0
NTC_10_AI
Text GLabel 5750 2300 0    59   Output ~ 0
NTC_11_AI
Text GLabel 5750 2200 0    59   Output ~ 0
NTC_12_AI
Text GLabel 5750 2100 0    59   Output ~ 0
NTC_13_AI
Text GLabel 5750 2000 0    59   Output ~ 0
NTC_14_AI
Text GLabel 5750 2600 0    59   Output ~ 0
NTC_15_AI
Text GLabel 5750 2700 0    59   Output ~ 0
NTC_16_AI
Text GLabel 5750 2800 0    59   Output ~ 0
NTC_17_AI
Text GLabel 5750 2900 0    59   Output ~ 0
NTC_18_AI
Text GLabel 5750 3000 0    59   Output ~ 0
NTC_19_AI
Text GLabel 5750 3100 0    59   Output ~ 0
NTC_20_AI
Text GLabel 5750 3200 0    59   Output ~ 0
NTC_21_AI
Text GLabel 5750 3300 0    59   Output ~ 0
NTC_22_AI
Text GLabel 5750 3400 0    59   Output ~ 0
NTC_23_AI
Text GLabel 5750 3500 0    59   Output ~ 0
NTC_24_AI
Text GLabel 9800 2100 2    59   Output ~ 0
SPI_CS
Text GLabel 9800 1900 2    59   Input ~ 0
SPI_SDO
Text GLabel 9800 2000 2    59   Output ~ 0
SPI_SDI
Text GLabel 9800 1800 2    59   Output ~ 0
SPI_CKL
NoConn ~ 9800 2650
NoConn ~ 9800 2750
NoConn ~ 9800 2850
NoConn ~ 9800 2950
NoConn ~ 9800 3050
NoConn ~ 9800 3150
NoConn ~ 9800 3250
NoConn ~ 9800 3450
NoConn ~ 9800 3550
NoConn ~ 9800 3650
NoConn ~ 9800 3750
NoConn ~ 9800 1200
NoConn ~ 9800 1100
NoConn ~ 9800 1000
NoConn ~ 9800 900 
NoConn ~ 9800 800 
NoConn ~ 5750 1000
NoConn ~ 5750 1100
NoConn ~ 5750 1200
NoConn ~ 5750 1300
NoConn ~ 5750 1400
NoConn ~ 5750 1500
NoConn ~ 5750 1600
NoConn ~ 5750 1700
NoConn ~ 5750 1800
NoConn ~ 5750 4400
NoConn ~ 5750 4500
NoConn ~ 5750 4600
NoConn ~ 5750 4700
NoConn ~ 5750 4800
NoConn ~ 5750 5100
NoConn ~ 5750 5200
NoConn ~ 5750 5300
NoConn ~ 5750 5400
NoConn ~ 5750 5500
NoConn ~ 5750 5600
NoConn ~ 5750 5700
NoConn ~ 5750 5800
NoConn ~ 5750 5900
Text GLabel 9800 700  2    59   Input ~ 0
Transducer_Input_AI
Text GLabel 5750 900  0    59   Input ~ 0
5V_SW_nFAULT
NoConn ~ 5750 700 
NoConn ~ 5750 800 
$EndSCHEMATC
