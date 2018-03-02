EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
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
LIBS:Connectors
LIBS:HSD
LIBS:ICs
LIBS:micros
LIBS:battery_management
LIBS:ftdi
LIBS:leds
LIBS:LEM
LIBS:microchip_dspic33dsc
LIBS:modules
LIBS:rfcom
LIBS:RFSolutions
LIBS:sensors
LIBS:IntrefaceController_01-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 7
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
L +3V3 #PWR?
U 1 1 59953D12
P 850 4400
F 0 "#PWR?" H 850 4250 50  0001 C CNN
F 1 "+3V3" H 850 4540 50  0000 C CNN
F 2 "" H 850 4400 50  0001 C CNN
F 3 "" H 850 4400 50  0001 C CNN
	1    850  4400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 59953D98
P 850 5300
F 0 "#PWR?" H 850 5050 50  0001 C CNN
F 1 "GND" H 850 5150 50  0000 C CNN
F 2 "" H 850 5300 50  0001 C CNN
F 3 "" H 850 5300 50  0001 C CNN
	1    850  5300
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 59954183
P 950 4850
F 0 "C?" H 975 4950 50  0000 L CNN
F 1 "100nF" H 975 4750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 988 4700 50  0001 C CNN
F 3 "http://psearch.en.murata.com/capacitor/product/GCM188R71H104KA57%23.pdf" H 950 4850 50  0001 C CNN
F 4 "GCM188R71H104KA57D" H 975 5050 50  0001 C CNN "MPN"
F 5 "y" H 975 5050 50  0001 C CNN "populate"
	1    950  4850
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 599541B1
P 1150 4850
F 0 "C?" H 1175 4950 50  0000 L CNN
F 1 "100nF" H 1175 4750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1188 4700 50  0001 C CNN
F 3 "http://psearch.en.murata.com/capacitor/product/GCM188R71H104KA57%23.pdf" H 1150 4850 50  0001 C CNN
F 4 "GCM188R71H104KA57D" H 1175 5050 50  0001 C CNN "MPN"
F 5 "y" H 1175 5050 50  0001 C CNN "populate"
	1    1150 4850
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 599541DB
P 1350 4850
F 0 "C?" H 1375 4950 50  0000 L CNN
F 1 "100nF" H 1375 4750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1388 4700 50  0001 C CNN
F 3 "http://psearch.en.murata.com/capacitor/product/GCM188R71H104KA57%23.pdf" H 1350 4850 50  0001 C CNN
F 4 "GCM188R71H104KA57D" H 1375 5050 50  0001 C CNN "MPN"
F 5 "y" H 1375 5050 50  0001 C CNN "populate"
	1    1350 4850
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 599541FC
P 1550 4850
F 0 "C?" H 1575 4950 50  0000 L CNN
F 1 "100nF" H 1575 4750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1588 4700 50  0001 C CNN
F 3 "http://psearch.en.murata.com/capacitor/product/GCM188R71H104KA57%23.pdf" H 1550 4850 50  0001 C CNN
F 4 "GCM188R71H104KA57D" H 1575 5050 50  0001 C CNN "MPN"
F 5 "y" H 1575 5050 50  0001 C CNN "populate"
	1    1550 4850
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 59954470
P 1650 6550
F 0 "C?" H 1675 6650 50  0000 L CNN
F 1 "10uF" H 1675 6450 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 1688 6400 50  0001 C CNN
F 3 "https://content.kemet.com/datasheets/KEM_C1023_X7R_AUTO_SMD.pdf" H 1650 6550 50  0001 C CNN
F 4 "C1206C106M3RACAUTO" H 1675 6750 50  0001 C CNN "MPN"
F 5 "y" H 1675 6750 50  0001 C CNN "populate"
	1    1650 6550
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5995450B
P 1400 6550
F 0 "#PWR?" H 1400 6300 50  0001 C CNN
F 1 "GND" H 1400 6400 50  0000 C CNN
F 2 "" H 1400 6550 50  0001 C CNN
F 3 "" H 1400 6550 50  0001 C CNN
	1    1400 6550
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 59954578
P 1650 6100
F 0 "C?" H 1675 6200 50  0000 L CNN
F 1 "100nF" H 1675 6000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1688 5950 50  0001 C CNN
F 3 "http://psearch.en.murata.com/capacitor/product/GCM188R71H104KA57%23.pdf" H 1650 6100 50  0001 C CNN
F 4 "GCM188R71H104KA57D" H 1675 6300 50  0001 C CNN "MPN"
F 5 "y" H 1675 6300 50  0001 C CNN "populate"
	1    1650 6100
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 59955AC7
P 850 6100
F 0 "C?" H 875 6200 50  0000 L CNN
F 1 "100nF" H 875 6000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 888 5950 50  0001 C CNN
F 3 "http://psearch.en.murata.com/capacitor/product/GCM188R71H104KA57%23.pdf" H 850 6100 50  0001 C CNN
F 4 "GCM188R71H104KA57D" H 875 6300 50  0001 C CNN "MPN"
F 5 "y" H 875 6300 50  0001 C CNN "populate"
	1    850  6100
	1    0    0    -1  
$EndComp
$Comp
L Ferrite_Bead L?
U 1 1 59955DBF
P 1250 5900
F 0 "L?" V 1100 5925 50  0000 C CNN
F 1 "Ferrite_Bead" V 1400 5900 50  0000 C CNN
F 2 "Inductors_SMD:L_0603" V 1180 5900 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/beads_automotive_power_mpz1608_en.pdf" H 1250 5900 50  0001 C CNN
F 4 "MPZ1608S300ATDH5" H 1100 6025 50  0001 C CNN "MPN"
F 5 "y" H 1100 6025 50  0001 C CNN "populate"
	1    1250 5900
	0    1    1    0   
$EndComp
$Comp
L +3V3 #PWR?
U 1 1 59955FA7
P 750 5900
F 0 "#PWR?" H 750 5750 50  0001 C CNN
F 1 "+3V3" H 750 6040 50  0000 C CNN
F 2 "" H 750 5900 50  0001 C CNN
F 3 "" H 750 5900 50  0001 C CNN
	1    750  5900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 59955FD3
P 750 6300
F 0 "#PWR?" H 750 6050 50  0001 C CNN
F 1 "GND" H 750 6150 50  0000 C CNN
F 2 "" H 750 6300 50  0001 C CNN
F 3 "" H 750 6300 50  0001 C CNN
	1    750  6300
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 599562CF
P 5300 6650
F 0 "C?" H 5325 6750 50  0000 L CNN
F 1 "18pF" H 5325 6550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5338 6500 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2C0G1H180J080AA.pdf" H 5300 6650 50  0001 C CNN
F 4 "CGA3E2C0G1H180J080AA" H 5325 6850 50  0001 C CNN "MPN"
F 5 "y" H 5325 6850 50  0001 C CNN "populate"
	1    5300 6650
	0    -1   1    0   
$EndComp
$Comp
L C C?
U 1 1 59956321
P 5300 7150
F 0 "C?" H 5325 7250 50  0000 L CNN
F 1 "18pF" H 5325 7050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5338 7000 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/documents/chara_sheet/CGA3E2C0G1H180J080AA.pdf" H 5300 7150 50  0001 C CNN
F 4 "CGA3E2C0G1H180J080AA" H 5325 7350 50  0001 C CNN "MPN"
F 5 "y" H 5325 7350 50  0001 C CNN "populate"
	1    5300 7150
	0    -1   1    0   
$EndComp
$Comp
L R R?
U 1 1 5995635F
P 4150 6900
F 0 "R?" V 4230 6900 50  0000 C CNN
F 1 "1M" V 4150 6900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4080 6900 50  0001 C CNN
F 3 "http://www.yageo.com/NewPortal/yageodocoutput?fileName=/pdf/R-Chip/PYu-AC_51_RoHS_L_6.pdf" H 4150 6900 50  0001 C CNN
F 4 "AC0603FR-071ML" H 4230 7000 50  0001 C CNN "MPN"
F 5 "y" H 4230 7000 50  0001 C CNN "populate"
	1    4150 6900
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5995664C
P 5600 6900
F 0 "#PWR?" H 5600 6650 50  0001 C CNN
F 1 "GND" H 5600 6750 50  0000 C CNN
F 2 "" H 5600 6900 50  0001 C CNN
F 3 "" H 5600 6900 50  0001 C CNN
	1    5600 6900
	0    -1   1    0   
$EndComp
$Comp
L +3V3 #PWR?
U 1 1 59967AA0
P 1450 7050
F 0 "#PWR?" H 1450 6900 50  0001 C CNN
F 1 "+3V3" H 1450 7190 50  0000 C CNN
F 2 "" H 1450 7050 50  0001 C CNN
F 3 "" H 1450 7050 50  0001 C CNN
	1    1450 7050
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 59967AE3
P 1450 7300
F 0 "R?" V 1530 7300 50  0000 C CNN
F 1 "10k" V 1450 7300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1380 7300 50  0001 C CNN
F 3 "http://www.yageo.com/NewPortal/yageodocoutput?fileName=/pdf/R-Chip/PYu-AC_51_RoHS_L_6.pdf" H 1450 7300 50  0001 C CNN
F 4 "AC0603FR-0710KL" H 1530 7400 50  0001 C CNN "MPN"
F 5 "y" H 1530 7400 50  0001 C CNN "populate"
	1    1450 7300
	1    0    0    -1  
$EndComp
$Comp
L +3.3VA #PWR?
U 1 1 59977C27
P 1800 5850
F 0 "#PWR?" H 1800 5700 50  0001 C CNN
F 1 "+3.3VA" H 1800 5990 50  0000 C CNN
F 2 "" H 1800 5850 50  0001 C CNN
F 3 "" H 1800 5850 50  0001 C CNN
	1    1800 5850
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG?
U 1 1 59980D1B
P 1500 5750
F 0 "#FLG?" H 1500 5825 50  0001 C CNN
F 1 "PWR_FLAG" H 1500 5900 50  0000 C CNN
F 2 "" H 1500 5750 50  0001 C CNN
F 3 "" H 1500 5750 50  0001 C CNN
	1    1500 5750
	1    0    0    -1  
$EndComp
$Comp
L PIC33EPxxxMU810 U?
U 1 1 599FC02F
P 8150 2200
F 0 "U?" H 8900 -1650 60  0000 C CNN
F 1 "PIC33EPxxxMU810" H 8150 -1650 60  0000 C CNN
F 2 "Housings_QFP:TQFP-100_14x14mm_Pitch0.5mm" H 7400 1800 60  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/70616g.pdf" H 7400 1800 60  0001 C CNN
F 4 "DSPIC33EP512MU810-E/PF" H 8900 -1550 50  0001 C CNN "MPN"
F 5 "y" H 8900 -1550 50  0001 C CNN "populate"
	1    8150 2200
	1    0    0    -1  
$EndComp
$Comp
L PIC33EPxxxMU810 U?
U 2 1 599FC07A
P 2700 3650
F 0 "U?" H 3450 -200 60  0000 C CNN
F 1 "PIC33EPxxxMU810" H 2700 -200 60  0000 C CNN
F 2 "Housings_QFP:TQFP-100_14x14mm_Pitch0.5mm" H 1950 3250 60  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/70616g.pdf" H 1950 3250 60  0001 C CNN
F 4 "DSPIC33EP512MU810-E/PF" H 3450 -100 50  0001 C CNN "MPN"
F 5 "y" H 3450 -100 50  0001 C CNN "populate"
	2    2700 3650
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 599FDE83
P 750 4850
F 0 "C?" H 775 4950 50  0000 L CNN
F 1 "100nF" H 775 4750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 788 4700 50  0001 C CNN
F 3 "http://psearch.en.murata.com/capacitor/product/GCM188R71H104KA57%23.pdf" H 750 4850 50  0001 C CNN
F 4 "GCM188R71H104KA57D" H 775 5050 50  0001 C CNN "MPN"
F 5 "y" H 775 5050 50  0001 C CNN "populate"
	1    750  4850
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 599FDED2
P 1750 4850
F 0 "C?" H 1775 4950 50  0000 L CNN
F 1 "100nF" H 1775 4750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1788 4700 50  0001 C CNN
F 3 "http://psearch.en.murata.com/capacitor/product/GCM188R71H104KA57%23.pdf" H 1750 4850 50  0001 C CNN
F 4 "GCM188R71H104KA57D" H 1775 5050 50  0001 C CNN "MPN"
F 5 "y" H 1775 5050 50  0001 C CNN "populate"
	1    1750 4850
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR?
U 1 1 59B2845D
P 4100 5750
F 0 "#PWR?" H 4100 5600 50  0001 C CNN
F 1 "+3V3" H 4100 5890 50  0000 C CNN
F 2 "" H 4100 5750 50  0001 C CNN
F 3 "" H 4100 5750 50  0001 C CNN
	1    4100 5750
	1    0    0    -1  
$EndComp
$Comp
L Crystal_GND24 Y?
U 1 1 59BBA021
P 4700 6900
F 0 "Y?" V 4825 7100 50  0000 L TNN
F 1 "8MHz" V 4825 7025 50  0000 L BNN
F 2 "Crystals:Crystal_SMD_7050-4pin_7.0x5.0mm" H 4700 6900 50  0001 C CNN
F 3 "http://www.ecsxtal.com/store/pdf/CSM-8Q.pdf" H 4700 6900 50  0001 C CNN
F 4 "ECS-80-18-20BQ-DS" H 4825 7200 50  0001 C CNN "MPN"
F 5 "y" H 4825 7200 50  0001 C CNN "populate"
	1    4700 6900
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 59BBA3D3
P 4950 6900
F 0 "#PWR?" H 4950 6650 50  0001 C CNN
F 1 "GND" H 4950 6750 50  0000 C CNN
F 2 "" H 4950 6900 50  0001 C CNN
F 3 "" H 4950 6900 50  0001 C CNN
	1    4950 6900
	0    -1   1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 59BBA417
P 4450 6900
F 0 "#PWR?" H 4450 6650 50  0001 C CNN
F 1 "GND" H 4450 6750 50  0000 C CNN
F 2 "" H 4450 6900 50  0001 C CNN
F 3 "" H 4450 6900 50  0001 C CNN
	1    4450 6900
	0    1    -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5A45E91E
P 700 7200
F 0 "#PWR?" H 700 6950 50  0001 C CNN
F 1 "GND" H 700 7050 50  0000 C CNN
F 2 "" H 700 7200 50  0001 C CNN
F 3 "" H 700 7200 50  0001 C CNN
	1    700  7200
	0    1    1    0   
$EndComp
$Comp
L SW_Push SW?
U 1 1 5A45E5E7
P 950 7200
F 0 "SW?" H 1000 7300 50  0000 L CNN
F 1 "PTS645SM43SMTR92 LFS" H 950 7140 50  0001 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_EVQP0" H 950 7400 50  0001 C CNN
F 3 "https://www.ckswitches.com/media/1471/pts645.pdf" H 950 7400 50  0001 C CNN
F 4 "PTS645SM43SMTR92 LFS" H 1000 7400 50  0001 C CNN "MPN"
F 5 "y" H 1000 7400 50  0001 C CNN "populate"
	1    950  7200
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5A870904
P 1700 7600
F 0 "R?" V 1780 7600 50  0000 C CNN
F 1 "10k" V 1700 7600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1630 7600 50  0001 C CNN
F 3 "http://www.yageo.com/NewPortal/yageodocoutput?fileName=/pdf/R-Chip/PYu-AC_51_RoHS_L_6.pdf" H 1700 7600 50  0001 C CNN
F 4 "AC0603FR-0710KL" H 1780 7700 50  0001 C CNN "MPN"
F 5 "y" H 1780 7700 50  0001 C CNN "populate"
	1    1700 7600
	0    1    1    0   
$EndComp
Text Notes 1700 950  2    60   ~ 0
TO ADD
Text GLabel 850  7600 0    60   Input ~ 0
~RESET
Text GLabel 5600 700  0    60   Output ~ 0
DEBUG_LED
Text GLabel 5600 800  0    60   BiDi ~ 0
DEBUG_PIN
Wire Wire Line
	750  5300 2000 5300
Wire Wire Line
	750  4400 2000 4400
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
	1400 5900 1950 5900
Wire Wire Line
	1650 5950 1650 5900
Wire Wire Line
	750  6300 1950 6300
Wire Wire Line
	750  5900 1100 5900
Wire Wire Line
	850  6300 850  6250
Wire Wire Line
	850  5950 850  5900
Wire Wire Line
	1650 6250 1650 6300
Wire Wire Line
	3950 6650 5150 6650
Wire Wire Line
	3950 7150 5150 7150
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
	5500 6650 5500 7150
Wire Wire Line
	5500 6650 5450 6650
Wire Wire Line
	1450 7050 1450 7150
Wire Wire Line
	1450 7600 1450 7450
Wire Wire Line
	850  7600 1550 7600
Wire Wire Line
	1800 5850 1800 5900
Wire Wire Line
	1500 5750 1500 5900
Wire Wire Line
	750  4400 750  4700
Wire Wire Line
	750  5000 750  5300
Wire Wire Line
	2000 4400 2000 4900
Wire Wire Line
	2000 5300 2000 5700
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
	5750 700  5600 700 
Wire Wire Line
	4100 5750 4100 5800
Wire Wire Line
	4100 5800 3900 5800
Wire Wire Line
	4500 6900 4450 6900
Wire Wire Line
	4950 6900 4900 6900
Wire Wire Line
	5600 6900 5500 6900
Wire Wire Line
	1250 7600 1250 7200
Wire Wire Line
	750  7200 700  7200
Wire Wire Line
	1250 7200 1150 7200
Wire Wire Line
	5750 800  5600 800 
Connection ~ 950  4400
Connection ~ 1150 4400
Connection ~ 1350 4400
Connection ~ 1550 4400
Connection ~ 950  5300
Connection ~ 1150 5300
Connection ~ 1350 5300
Connection ~ 1550 5300
Connection ~ 850  4400
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
Connection ~ 850  5300
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
NoConn ~ 3900 6150
$EndSCHEMATC
