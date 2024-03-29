EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "AirQualitySensor"
Date ""
Rev "v1"
Comp "Finomnis"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:GND #PWR0101
U 1 1 615D7685
P 7050 3700
F 0 "#PWR0101" H 7050 3450 50  0001 C CNN
F 1 "GND" V 7150 3700 50  0000 R CNN
F 2 "" H 7050 3700 50  0001 C CNN
F 3 "" H 7050 3700 50  0001 C CNN
	1    7050 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 615D7DF8
P 4150 3700
F 0 "#PWR0102" H 4150 3450 50  0001 C CNN
F 1 "GND" V 4050 3700 50  0000 R CNN
F 2 "" H 4150 3700 50  0001 C CNN
F 3 "" H 4150 3700 50  0001 C CNN
	1    4150 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 615D864F
P 7050 4900
F 0 "#PWR0103" H 7050 4650 50  0001 C CNN
F 1 "GND" V 7150 4900 50  0000 R CNN
F 2 "" H 7050 4900 50  0001 C CNN
F 3 "" H 7050 4900 50  0001 C CNN
	1    7050 4900
	1    0    0    -1  
$EndComp
NoConn ~ 4900 4000
NoConn ~ 4900 4100
NoConn ~ 4900 4200
NoConn ~ 6950 4000
$Comp
L power:+3V3 #PWR0105
U 1 1 615E0AF7
P 5350 5350
F 0 "#PWR0105" H 5350 5200 50  0001 C CNN
F 1 "+3V3" H 5200 5450 50  0000 C CNN
F 2 "" H 5350 5350 50  0001 C CNN
F 3 "" H 5350 5350 50  0001 C CNN
	1    5350 5350
	1    0    0    -1  
$EndComp
NoConn ~ 6950 4600
NoConn ~ 6950 4500
NoConn ~ 6950 4400
NoConn ~ 6950 4100
NoConn ~ 6950 3900
NoConn ~ 6950 3800
$Comp
L AirQualitySensor:EA_DOGM132W-5 DS1
U 1 1 615E0422
P 10950 1050
F 0 "DS1" H 11550 1315 50  0000 C CNN
F 1 "EA_DOGM132W-5" H 11550 1224 50  0000 C CNN
F 2 "AirQualitySensor:EA_DOGM132W-5" H 12000 1150 50  0001 L CNN
F 3 "" H 12000 1050 50  0001 L CNN
F 4 "LCD Graphic Display Modules & Accessories DOGM Grapic 132x32 White Background" H 12000 950 50  0001 L CNN "Description"
F 5 "2" H 12000 850 50  0001 L CNN "Height"
F 6 "790-EADOGM132W5" H 12000 750 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/ELECTRONIC-ASSEMBLY/EA-DOGM132W-5?qs=s9z6UkyjM7o689ToJkW0yA%3D%3D" H 12000 650 50  0001 L CNN "Mouser Price/Stock"
F 8 "ELECTRONIC ASSEMBLY" H 12000 550 50  0001 L CNN "Manufacturer_Name"
F 9 "EA DOGM132W-5" H 12000 450 50  0001 L CNN "Manufacturer_Part_Number"
	1    10950 1050
	-1   0    0    -1  
$EndComp
$Comp
L AirQualitySensor:004-0-0053 U1
U 1 1 615E61D2
P 2200 6850
F 0 "U1" H 1750 7200 50  0000 C CNN
F 1 "004-0-0053" H 2500 7200 50  0000 C CNN
F 2 "AirQualitySensor:Senseair_S8_Up" H 2200 6850 50  0001 C CNN
F 3 "https://rmtplusstoragesenseair.blob.core.windows.net/docs/publicerat/PSP126.pdf" H 2200 6850 50  0001 C CNN
	1    2200 6850
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R2
U 1 1 616037F6
P 2100 1350
F 0 "R2" H 2150 1450 50  0000 L CNN
F 1 "5.1k" V 2200 1200 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 2100 1350 50  0001 C CNN
F 3 "~" H 2100 1350 50  0001 C CNN
	1    2100 1350
	0    -1   1    0   
$EndComp
$Comp
L Device:R_Small R1
U 1 1 61604761
P 2100 1250
F 0 "R1" H 2050 1150 50  0000 R CNN
F 1 "5.1k" V 2000 1400 50  0000 R CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 2100 1250 50  0001 C CNN
F 3 "~" H 2100 1250 50  0001 C CNN
	1    2100 1250
	0    1    1    0   
$EndComp
Wire Wire Line
	2200 1350 2400 1350
Wire Wire Line
	2200 1250 2400 1250
Wire Wire Line
	2400 1250 2400 1350
Connection ~ 2400 1350
Wire Wire Line
	2000 1350 1950 1350
Wire Wire Line
	1950 1250 2000 1250
Wire Wire Line
	2000 1650 1950 1650
Wire Wire Line
	2000 1750 1950 1750
Text GLabel 2000 1750 2    50   Input ~ 0
USB_D+
$Comp
L power:GND #PWR0106
U 1 1 616003CC
P 2400 1350
F 0 "#PWR0106" H 2400 1100 50  0001 C CNN
F 1 "GND" H 2500 1200 50  0000 R CNN
F 2 "" H 2400 1350 50  0001 C CNN
F 3 "" H 2400 1350 50  0001 C CNN
	1    2400 1350
	1    0    0    -1  
$EndComp
Text GLabel 2000 1650 2    50   Input ~ 0
USB_D-
Text GLabel 6450 5400 3    50   Input ~ 0
USB_D+
Text GLabel 6350 5400 3    50   Input ~ 0
USB_D-
Wire Wire Line
	6350 5200 6350 5400
Wire Wire Line
	6450 5200 6450 5400
$Comp
L Regulator_Linear:XC6206PxxxMR U3
U 1 1 616297E8
P 3550 1500
F 0 "U3" H 3550 1742 50  0000 C CNN
F 1 "XC6206P332MR" H 3550 1651 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3550 1725 50  0001 C CIN
F 3 "https://www.torexsemi.com/file/xc6206/XC6206.pdf" H 3550 1500 50  0001 C CNN
	1    3550 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C2
U 1 1 6162B213
P 3850 1600
F 0 "C2" H 3942 1646 50  0000 L CNN
F 1 "1u" H 3942 1555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3850 1600 50  0001 C CNN
F 3 "~" H 3850 1600 50  0001 C CNN
	1    3850 1600
	1    0    0    1   
$EndComp
$Comp
L Device:C_Small C1
U 1 1 6162C51B
P 3250 1600
F 0 "C1" H 3159 1554 50  0000 R CNN
F 1 "1u" H 3159 1645 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3250 1600 50  0001 C CNN
F 3 "~" H 3250 1600 50  0001 C CNN
	1    3250 1600
	1    0    0    1   
$EndComp
Wire Wire Line
	3250 1700 3250 1800
Wire Wire Line
	3250 1800 3550 1800
Wire Wire Line
	3550 1800 3850 1800
Wire Wire Line
	3850 1800 3850 1700
Connection ~ 3550 1800
$Comp
L power:GND #PWR0107
U 1 1 6162F135
P 3550 1800
F 0 "#PWR0107" H 3550 1550 50  0001 C CNN
F 1 "GND" H 3650 1650 50  0000 R CNN
F 2 "" H 3550 1800 50  0001 C CNN
F 3 "" H 3550 1800 50  0001 C CNN
	1    3550 1800
	1    0    0    -1  
$EndComp
$Comp
L Device:Ferrite_Bead_Small FB2
U 1 1 616349EE
P 2850 1500
F 0 "FB2" V 2613 1500 50  0000 C CNN
F 1 "100R" V 2704 1500 50  0000 C CNN
F 2 "Inductor_SMD:L_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 2780 1500 50  0001 C CNN
F 3 "~" H 2850 1500 50  0001 C CNN
F 4 "GZ2012D101TF" H 2850 1500 50  0001 C CNN "Description"
	1    2850 1500
	0    1    1    0   
$EndComp
Wire Wire Line
	2950 1500 3050 1500
Connection ~ 3250 1500
$Comp
L Device:Ferrite_Bead_Small FB1
U 1 1 6164DF97
P 2850 1050
F 0 "FB1" V 2613 1050 50  0000 C CNN
F 1 "100R" V 2704 1050 50  0000 C CNN
F 2 "Inductor_SMD:L_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 2780 1050 50  0001 C CNN
F 3 "~" H 2850 1050 50  0001 C CNN
F 4 "GZ2012D101TF" H 2850 1050 50  0001 C CNN "Description"
	1    2850 1050
	0    1    1    0   
$EndComp
Wire Wire Line
	2600 1050 2600 1500
Wire Wire Line
	2600 1500 2750 1500
Connection ~ 2600 1050
Wire Wire Line
	2600 1050 2750 1050
$Comp
L Device:C_Small C3
U 1 1 6164F8D8
P 4250 1150
F 0 "C3" H 4342 1196 50  0000 L CNN
F 1 "4u7" H 4342 1105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4250 1150 50  0001 C CNN
F 3 "~" H 4250 1150 50  0001 C CNN
	1    4250 1150
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C4
U 1 1 6164FE36
P 4250 1600
F 0 "C4" H 4342 1646 50  0000 L CNN
F 1 "4u7" H 4342 1555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4250 1600 50  0001 C CNN
F 3 "~" H 4250 1600 50  0001 C CNN
	1    4250 1600
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 616502A2
P 4250 1250
F 0 "#PWR0108" H 4250 1000 50  0001 C CNN
F 1 "GND" H 4350 1100 50  0000 R CNN
F 2 "" H 4250 1250 50  0001 C CNN
F 3 "" H 4250 1250 50  0001 C CNN
	1    4250 1250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0109
U 1 1 61650924
P 4250 1700
F 0 "#PWR0109" H 4250 1450 50  0001 C CNN
F 1 "GND" H 4350 1550 50  0000 R CNN
F 2 "" H 4250 1700 50  0001 C CNN
F 3 "" H 4250 1700 50  0001 C CNN
	1    4250 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 1050 4550 1050
Connection ~ 4250 1050
$Comp
L power:+3V3 #PWR0110
U 1 1 616516F0
P 4550 1500
F 0 "#PWR0110" H 4550 1350 50  0001 C CNN
F 1 "+3V3" H 4565 1673 50  0000 C CNN
F 2 "" H 4550 1500 50  0001 C CNN
F 3 "" H 4550 1500 50  0001 C CNN
	1    4550 1500
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0111
U 1 1 61652D46
P 4550 1050
F 0 "#PWR0111" H 4550 900 50  0001 C CNN
F 1 "+5V" H 4565 1223 50  0000 C CNN
F 2 "" H 4550 1050 50  0001 C CNN
F 3 "" H 4550 1050 50  0001 C CNN
	1    4550 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 1050 2150 1050
$Comp
L power:VBUS #PWR0112
U 1 1 616546B9
P 2150 1050
F 0 "#PWR0112" H 2150 900 50  0001 C CNN
F 1 "VBUS" H 2165 1223 50  0000 C CNN
F 2 "" H 2150 1050 50  0001 C CNN
F 3 "" H 2150 1050 50  0001 C CNN
	1    2150 1050
	1    0    0    -1  
$EndComp
Connection ~ 2150 1050
$Comp
L power:VBUS #PWR0113
U 1 1 6165538D
P 6250 5200
F 0 "#PWR0113" H 6250 5050 50  0001 C CNN
F 1 "VBUS" V 6250 5400 50  0000 C CNN
F 2 "" H 6250 5200 50  0001 C CNN
F 3 "" H 6250 5200 50  0001 C CNN
	1    6250 5200
	-1   0    0    1   
$EndComp
NoConn ~ 1950 2150
NoConn ~ 1950 2250
Connection ~ 1950 1750
Connection ~ 1950 1650
Wire Wire Line
	1950 1850 1950 1750
Wire Wire Line
	1950 1550 1950 1650
NoConn ~ 1050 2550
$Comp
L Connector:USB_C_Receptacle_USB2.0 J1
U 1 1 615F7DD0
P 1350 1650
F 0 "J1" H 1350 2500 50  0000 C CNN
F 1 "USB_C_Receptacle_USB2.0" H 1350 2400 50  0000 C CNN
F 2 "Connector_USB:USB_C_Receptacle_HRO_TYPE-C-31-M-12" H 1500 1650 50  0001 C CNN
F 3 "https://www.usb.org/sites/default/files/documents/usb_type-c.zip" H 1500 1650 50  0001 C CNN
	1    1350 1650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 615FDC6B
P 1350 2550
F 0 "#PWR0104" H 1350 2300 50  0001 C CNN
F 1 "GND" H 1300 2500 50  0000 R CNN
F 2 "" H 1350 2550 50  0001 C CNN
F 3 "" H 1350 2550 50  0001 C CNN
	1    1350 2550
	1    0    0    -1  
$EndComp
NoConn ~ 10950 1350
NoConn ~ 10950 1250
NoConn ~ 10950 1150
NoConn ~ 10950 1050
$Comp
L Device:C_Small C8
U 1 1 6168B9AB
P 8950 2800
F 0 "C8" H 8858 2754 50  0000 R CNN
F 1 "1u" H 8858 2845 50  0000 R CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 8950 2800 50  0001 C CNN
F 3 "~" H 8950 2800 50  0001 C CNN
	1    8950 2800
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C6
U 1 1 6168CE5F
P 8700 2700
F 0 "C6" H 8608 2654 50  0000 R CNN
F 1 "1u" H 8608 2745 50  0000 R CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 8700 2700 50  0001 C CNN
F 3 "~" H 8700 2700 50  0001 C CNN
	1    8700 2700
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C10
U 1 1 6168E68A
P 9200 2900
F 0 "C10" H 9108 2854 50  0000 R CNN
F 1 "1u" H 9108 2945 50  0000 R CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 9200 2900 50  0001 C CNN
F 3 "~" H 9200 2900 50  0001 C CNN
	1    9200 2900
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C11
U 1 1 6168EA00
P 9450 3000
F 0 "C11" H 9358 2954 50  0000 R CNN
F 1 "1u" H 9358 3045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 9450 3000 50  0001 C CNN
F 3 "~" H 9450 3000 50  0001 C CNN
	1    9450 3000
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C13
U 1 1 6168ECFD
P 9700 3100
F 0 "C13" H 9608 3054 50  0000 R CNN
F 1 "1u" H 9608 3145 50  0000 R CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 9700 3100 50  0001 C CNN
F 3 "~" H 9700 3100 50  0001 C CNN
	1    9700 3100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9750 2950 9700 2950
Wire Wire Line
	9700 2950 9700 3000
Wire Wire Line
	9750 2850 9450 2850
Wire Wire Line
	9450 2850 9450 2900
Wire Wire Line
	9750 2750 9200 2750
Wire Wire Line
	9200 2750 9200 2800
Wire Wire Line
	9750 2650 8950 2650
Wire Wire Line
	8950 2650 8950 2700
Wire Wire Line
	9750 2550 8700 2550
Wire Wire Line
	8700 2550 8700 2600
Wire Wire Line
	8700 3200 8950 3200
Wire Wire Line
	8700 2800 8700 3200
Wire Wire Line
	8950 2900 8950 3200
Connection ~ 8950 3200
Wire Wire Line
	8950 3200 9200 3200
Wire Wire Line
	9200 3000 9200 3200
Connection ~ 9200 3200
Wire Wire Line
	9200 3200 9450 3200
Wire Wire Line
	9450 3100 9450 3200
Connection ~ 9450 3200
Wire Wire Line
	9450 3200 9700 3200
$Comp
L power:GND #PWR0114
U 1 1 6169BE98
P 9200 3250
F 0 "#PWR0114" H 9200 3000 50  0001 C CNN
F 1 "GND" H 9150 3200 50  0000 R CNN
F 2 "" H 9200 3250 50  0001 C CNN
F 3 "" H 9200 3250 50  0001 C CNN
	1    9200 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 3250 9200 3200
$Comp
L power:GND #PWR0115
U 1 1 616A14EB
P 9650 2400
F 0 "#PWR0115" H 9650 2150 50  0001 C CNN
F 1 "GND" H 9550 2350 50  0000 R CNN
F 2 "" H 9650 2400 50  0001 C CNN
F 3 "" H 9650 2400 50  0001 C CNN
	1    9650 2400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0116
U 1 1 616A1A83
P 9350 1750
F 0 "#PWR0116" H 9350 1500 50  0001 C CNN
F 1 "GND" H 9400 1600 50  0000 R CNN
F 2 "" H 9350 1750 50  0001 C CNN
F 3 "" H 9350 1750 50  0001 C CNN
	1    9350 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C12
U 1 1 616A1FA8
P 9500 1850
F 0 "C12" H 9408 1804 50  0000 R CNN
F 1 "1u" H 9408 1895 50  0000 R CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 9500 1850 50  0001 C CNN
F 3 "~" H 9500 1850 50  0001 C CNN
	1    9500 1850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9750 1650 9750 1550
$Comp
L power:+3V3 #PWR0117
U 1 1 616AA191
P 9650 1650
F 0 "#PWR0117" H 9650 1500 50  0001 C CNN
F 1 "+3V3" H 9500 1750 50  0000 C CNN
F 2 "" H 9650 1650 50  0001 C CNN
F 3 "" H 9650 1650 50  0001 C CNN
	1    9650 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9750 1950 9750 1850
Wire Wire Line
	9750 1950 9500 1950
Connection ~ 9750 1950
$Comp
L Device:C_Small C9
U 1 1 616B32FE
P 9100 2050
F 0 "C9" V 8871 2050 50  0000 C CNN
F 1 "1u" V 8962 2050 50  0000 C CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 9100 2050 50  0001 C CNN
F 3 "~" H 9100 2050 50  0001 C CNN
	1    9100 2050
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C7
U 1 1 616B8AAF
P 8850 2250
F 0 "C7" V 9079 2250 50  0000 C CNN
F 1 "1u" V 8988 2250 50  0000 C CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 8850 2250 50  0001 C CNN
F 3 "~" H 8850 2250 50  0001 C CNN
	1    8850 2250
	0    1    -1   0   
$EndComp
Wire Wire Line
	9200 2050 9750 2050
Wire Wire Line
	9000 2150 9000 2050
Wire Wire Line
	9750 2250 8950 2250
Wire Wire Line
	8750 2250 8750 2350
Wire Wire Line
	9750 1650 9650 1650
Connection ~ 9750 1650
Wire Wire Line
	9650 2400 9750 2400
Wire Wire Line
	9750 2400 9750 2450
Text GLabel 9700 1450 0    50   Input ~ 0
DISP_SI
Wire Wire Line
	9700 1450 9750 1450
Text GLabel 9700 1350 0    50   Input ~ 0
DISP_SCL
Wire Wire Line
	9700 1350 9750 1350
Text GLabel 9700 1250 0    50   Input ~ 0
DISP_A0
Wire Wire Line
	9700 1250 9750 1250
Text GLabel 9700 1150 0    50   Input ~ 0
DISP_RST
Text GLabel 9700 1050 0    50   Input ~ 0
DISP_CS
Wire Wire Line
	9700 1050 9750 1050
Wire Wire Line
	9750 1150 9700 1150
$Comp
L power:+5V #PWR0118
U 1 1 616E3C93
P 3750 6700
F 0 "#PWR0118" H 3750 6550 50  0001 C CNN
F 1 "+5V" H 3765 6873 50  0000 C CNN
F 2 "" H 3750 6700 50  0001 C CNN
F 3 "" H 3750 6700 50  0001 C CNN
	1    3750 6700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 6750 3800 6750
$Comp
L power:GND #PWR0119
U 1 1 616E5BE7
P 3750 7050
F 0 "#PWR0119" H 3750 6800 50  0001 C CNN
F 1 "GND" H 3850 6900 50  0000 R CNN
F 2 "" H 3750 7050 50  0001 C CNN
F 3 "" H 3750 7050 50  0001 C CNN
	1    3750 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 7050 3800 7050
Text GLabel 3400 6850 0    50   Input ~ 0
DHT22
$Comp
L power:+5V #PWR0120
U 1 1 616EB72C
P 2200 6450
F 0 "#PWR0120" H 2200 6300 50  0001 C CNN
F 1 "+5V" H 2215 6623 50  0000 C CNN
F 2 "" H 2200 6450 50  0001 C CNN
F 3 "" H 2200 6450 50  0001 C CNN
	1    2200 6450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0121
U 1 1 616EBED1
P 2200 7250
F 0 "#PWR0121" H 2200 7000 50  0001 C CNN
F 1 "GND" H 2300 7100 50  0000 R CNN
F 2 "" H 2200 7250 50  0001 C CNN
F 3 "" H 2200 7250 50  0001 C CNN
	1    2200 7250
	1    0    0    -1  
$EndComp
Text GLabel 1150 6750 0    50   Input ~ 0
S8_RXD
Text GLabel 1150 6850 0    50   Input ~ 0
S8_TXD
Wire Wire Line
	3400 6850 3550 6850
$Comp
L Device:R_Small R5
U 1 1 616F0998
P 3550 6700
F 0 "R5" H 3500 6650 50  0000 R CNN
F 1 "10k" H 3500 6750 50  0000 R CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 3550 6700 50  0001 C CNN
F 3 "~" H 3550 6700 50  0001 C CNN
	1    3550 6700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 6800 3550 6850
Connection ~ 3550 6850
Wire Wire Line
	3750 6700 3750 6750
Wire Wire Line
	3550 6850 3800 6850
$Comp
L power:+3V3 #PWR0122
U 1 1 61700F29
P 3550 6550
F 0 "#PWR0122" H 3550 6400 50  0001 C CNN
F 1 "+3V3" H 3565 6723 50  0000 C CNN
F 2 "" H 3550 6550 50  0001 C CNN
F 3 "" H 3550 6550 50  0001 C CNN
	1    3550 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 6550 3550 6600
$Comp
L Device:R_Small R3
U 1 1 61704DB9
P 1300 6500
F 0 "R3" H 1250 6450 50  0000 R CNN
F 1 "10k" H 1250 6550 50  0000 R CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 1300 6500 50  0001 C CNN
F 3 "~" H 1300 6500 50  0001 C CNN
	1    1300 6500
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R4
U 1 1 6170564C
P 1400 6500
F 0 "R4" H 1350 6450 50  0000 R CNN
F 1 "10k" H 1350 6550 50  0000 R CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 1400 6500 50  0001 C CNN
F 3 "~" H 1400 6500 50  0001 C CNN
	1    1400 6500
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1150 6750 1300 6750
Wire Wire Line
	1300 6750 1300 6600
Connection ~ 1300 6750
Wire Wire Line
	1300 6750 1600 6750
Wire Wire Line
	1150 6850 1400 6850
Wire Wire Line
	1400 6850 1400 6600
Connection ~ 1400 6850
Wire Wire Line
	1400 6850 1600 6850
Wire Wire Line
	1300 6400 1300 6350
Wire Wire Line
	1300 6350 1350 6350
Wire Wire Line
	1400 6350 1400 6400
$Comp
L power:+3V3 #PWR0123
U 1 1 6170FA49
P 1350 6350
F 0 "#PWR0123" H 1350 6200 50  0001 C CNN
F 1 "+3V3" H 1365 6523 50  0000 C CNN
F 2 "" H 1350 6350 50  0001 C CNN
F 3 "" H 1350 6350 50  0001 C CNN
	1    1350 6350
	1    0    0    -1  
$EndComp
Connection ~ 1350 6350
Wire Wire Line
	1350 6350 1400 6350
NoConn ~ 1600 6650
NoConn ~ 1600 7050
NoConn ~ 2800 7050
NoConn ~ 2800 6850
NoConn ~ 2800 6650
$Comp
L Device:C_Small C5
U 1 1 61732450
P 5350 5450
F 0 "C5" H 5442 5496 50  0000 L CNN
F 1 "100n" H 5442 5405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 5350 5450 50  0001 C CNN
F 3 "~" H 5350 5450 50  0001 C CNN
	1    5350 5450
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0124
U 1 1 61746A91
P 5350 5550
F 0 "#PWR0124" H 5350 5300 50  0001 C CNN
F 1 "GND" H 5300 5500 50  0000 R CNN
F 2 "" H 5350 5550 50  0001 C CNN
F 3 "" H 5350 5550 50  0001 C CNN
	1    5350 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 3700 6950 3700
Wire Wire Line
	4150 3700 4900 3700
Wire Wire Line
	6950 4900 7050 4900
Wire Wire Line
	5450 5200 5450 5350
Wire Wire Line
	5350 5350 5450 5350
$Comp
L AirQualitySensor:TC2030 J2
U 1 1 61607D51
P 10200 4450
F 0 "J2" H 10728 4246 50  0000 L CNN
F 1 "SWD" H 10728 4155 50  0000 L CNN
F 2 "AirQualitySensor:TC2030-IDC-FP" H 10200 4700 50  0001 C CNN
F 3 "https://www.tag-connect.com/wp-content/uploads/bsk-pdf-manager/TC2030-CTX_1.pdf" H 10200 4450 50  0001 C CNN
	1    10200 4450
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0125
U 1 1 61609FA3
P 10150 4400
F 0 "#PWR0125" H 10150 4250 50  0001 C CNN
F 1 "+3V3" H 10165 4573 50  0000 C CNN
F 2 "" H 10150 4400 50  0001 C CNN
F 3 "" H 10150 4400 50  0001 C CNN
	1    10150 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	10150 4400 10150 4450
Wire Wire Line
	10150 4450 10200 4450
$Comp
L power:GND #PWR0126
U 1 1 6160CB76
P 10150 5000
F 0 "#PWR0126" H 10150 4750 50  0001 C CNN
F 1 "GND" H 10250 4850 50  0000 R CNN
F 2 "" H 10150 5000 50  0001 C CNN
F 3 "" H 10150 5000 50  0001 C CNN
	1    10150 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10150 5000 10150 4850
Wire Wire Line
	10150 4850 10200 4850
Text GLabel 9950 4750 0    50   Input ~ 0
SWD_SWCLK
Text GLabel 9950 4550 0    50   Input ~ 0
SWD_SWDIO
Text GLabel 9950 4950 0    50   Input ~ 0
SWD_SWO
Text GLabel 9950 4650 0    50   Input ~ 0
RESET
Wire Wire Line
	9950 4550 10200 4550
Wire Wire Line
	9950 4650 10200 4650
Wire Wire Line
	10200 4750 9950 4750
Wire Wire Line
	9950 4950 10200 4950
Text GLabel 7150 4300 2    50   Input ~ 0
SWD_SWCLK
Wire Wire Line
	7150 4300 6950 4300
Text GLabel 7150 4200 2    50   Input ~ 0
SWD_SWDIO
Wire Wire Line
	7150 4200 6950 4200
Text GLabel 6150 5400 3    50   Input ~ 0
RESET
Wire Wire Line
	6150 5400 6150 5200
$Comp
L AirQualitySensor:Waveshare_Core52840 A1
U 1 1 615D64F0
P 5950 4300
F 0 "A1" H 5950 5265 50  0000 C CNN
F 1 "Waveshare_Core52840" H 5950 5174 50  0000 C CNN
F 2 "AirQualitySensor:Waveshare_Core52840" H 5950 5050 50  0001 C CNN
F 3 "https://infocenter.nordicsemi.com/pdf/nRF52840_PS_v1.1.pdf" H 5900 4400 50  0001 C CNN
	1    5950 4300
	1    0    0    -1  
$EndComp
Text GLabel 7150 4700 2    50   Input ~ 0
SWD_SWO
Wire Wire Line
	7150 4700 6950 4700
$Comp
L AirQualitySensor:UART_ftdi J3
U 1 1 6163D4B3
P 10200 5500
F 0 "J3" H 10628 5296 50  0000 L CNN
F 1 "UART" H 10628 5205 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 10200 5900 50  0001 C CNN
F 3 "https://ftdichip.com/wp-content/uploads/2021/02/DS_TTL-232R_CABLES.pdf" H 10200 5500 50  0001 C CNN
	1    10200 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0127
U 1 1 6163E807
P 10150 6050
F 0 "#PWR0127" H 10150 5800 50  0001 C CNN
F 1 "GND" H 10250 5900 50  0000 R CNN
F 2 "" H 10150 6050 50  0001 C CNN
F 3 "" H 10150 6050 50  0001 C CNN
	1    10150 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 5500 10150 5500
Wire Wire Line
	10150 5500 10150 6050
$Comp
L power:VBUS #PWR0128
U 1 1 616426FB
P 10100 5450
F 0 "#PWR0128" H 10100 5300 50  0001 C CNN
F 1 "VBUS" H 10115 5623 50  0000 C CNN
F 2 "" H 10100 5450 50  0001 C CNN
F 3 "" H 10100 5450 50  0001 C CNN
	1    10100 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 5700 10100 5700
Wire Wire Line
	10100 5700 10100 5450
Text GLabel 9650 5800 0    50   Input ~ 0
UART_RXD
Wire Wire Line
	9650 5800 9750 5800
Text GLabel 9650 5900 0    50   Input ~ 0
UART_TXD
Wire Wire Line
	9650 5900 9850 5900
NoConn ~ 10200 5600
NoConn ~ 10200 6000
Wire Notes Line
	11050 6350 11050 4100
Wire Notes Line
	9100 6350 11050 6350
Wire Notes Line
	11050 4100 9100 4100
Wire Notes Line
	9100 4100 9100 6350
Text Notes 9100 4100 0    50   ~ 0
Programming Connectors
Wire Notes Line
	750  6100 4650 6100
Wire Notes Line
	4650 6100 4650 7500
Wire Notes Line
	4650 7500 750  7500
Wire Notes Line
	750  7500 750  6100
Text Notes 750  6100 0    50   ~ 0
Sensors
Wire Notes Line
	800  700  4700 700 
Wire Notes Line
	4700 700  4700 2750
Wire Notes Line
	4700 2750 800  2750
Wire Notes Line
	800  2750 800  700 
Text Notes 800  700  0    50   ~ 0
Power & USB
Wire Notes Line
	11050 3450 11050 700 
Wire Notes Line
	11050 700  8550 700 
Wire Notes Line
	8550 700  8550 3450
Wire Notes Line
	8550 3450 11050 3450
Text Notes 8550 700  0    50   ~ 0
Display
$Comp
L Device:C_Small C14
U 1 1 617462A1
P 8850 1150
F 0 "C14" H 8942 1196 50  0000 L CNN
F 1 "100n" H 8942 1105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 8850 1150 50  0001 C CNN
F 3 "~" H 8850 1150 50  0001 C CNN
	1    8850 1150
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0129
U 1 1 617473D8
P 8850 1050
F 0 "#PWR0129" H 8850 900 50  0001 C CNN
F 1 "+3V3" H 8700 1150 50  0000 C CNN
F 2 "" H 8850 1050 50  0001 C CNN
F 3 "" H 8850 1050 50  0001 C CNN
	1    8850 1050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0130
U 1 1 6174788F
P 8850 1250
F 0 "#PWR0130" H 8850 1000 50  0001 C CNN
F 1 "GND" H 8900 1100 50  0000 R CNN
F 2 "" H 8850 1250 50  0001 C CNN
F 3 "" H 8850 1250 50  0001 C CNN
	1    8850 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Small D1
U 1 1 61768156
P 1050 5600
F 0 "D1" V 1096 5530 50  0000 R CNN
F 1 "Green" V 1005 5530 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" V 1050 5600 50  0001 C CNN
F 3 "~" V 1050 5600 50  0001 C CNN
	1    1050 5600
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED_Small D2
U 1 1 61768FD8
P 1500 5600
F 0 "D2" V 1546 5530 50  0000 R CNN
F 1 "Yellow" V 1455 5530 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" V 1500 5600 50  0001 C CNN
F 3 "~" V 1500 5600 50  0001 C CNN
	1    1500 5600
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED_Small D3
U 1 1 617694C1
P 1950 5600
F 0 "D3" V 1996 5530 50  0000 R CNN
F 1 "Red" V 1905 5530 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" V 1950 5600 50  0001 C CNN
F 3 "~" V 1950 5600 50  0001 C CNN
	1    1950 5600
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R8
U 1 1 61776347
P 1950 5400
F 0 "R8" H 1891 5354 50  0000 R CNN
F 1 "10k" H 1891 5445 50  0000 R CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 1950 5400 50  0001 C CNN
F 3 "~" H 1950 5400 50  0001 C CNN
	1    1950 5400
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R7
U 1 1 61776CEE
P 1500 5400
F 0 "R7" H 1441 5354 50  0000 R CNN
F 1 "10k" H 1441 5445 50  0000 R CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 1500 5400 50  0001 C CNN
F 3 "~" H 1500 5400 50  0001 C CNN
	1    1500 5400
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R6
U 1 1 61776FAB
P 1050 5400
F 0 "R6" H 991 5354 50  0000 R CNN
F 1 "100k" H 991 5445 50  0000 R CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 1050 5400 50  0001 C CNN
F 3 "~" H 1050 5400 50  0001 C CNN
	1    1050 5400
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0131
U 1 1 617BCF08
P 1050 5700
F 0 "#PWR0131" H 1050 5450 50  0001 C CNN
F 1 "GND" H 1000 5650 50  0000 R CNN
F 2 "" H 1050 5700 50  0001 C CNN
F 3 "" H 1050 5700 50  0001 C CNN
	1    1050 5700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0132
U 1 1 617BD3BE
P 1500 5700
F 0 "#PWR0132" H 1500 5450 50  0001 C CNN
F 1 "GND" H 1450 5650 50  0000 R CNN
F 2 "" H 1500 5700 50  0001 C CNN
F 3 "" H 1500 5700 50  0001 C CNN
	1    1500 5700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0133
U 1 1 617BD55D
P 1950 5700
F 0 "#PWR0133" H 1950 5450 50  0001 C CNN
F 1 "GND" H 1900 5650 50  0000 R CNN
F 2 "" H 1950 5700 50  0001 C CNN
F 3 "" H 1950 5700 50  0001 C CNN
	1    1950 5700
	1    0    0    -1  
$EndComp
Text GLabel 1050 5250 1    50   Input ~ 0
LED_GREEN
Text GLabel 1500 5250 1    50   Input ~ 0
LED_YELLOW
Text GLabel 1950 5250 1    50   Input ~ 0
LED_RED
Wire Wire Line
	1950 5250 1950 5300
Wire Wire Line
	1500 5250 1500 5300
Wire Wire Line
	1050 5250 1050 5300
Wire Notes Line
	750  4650 3650 4650
Wire Notes Line
	3650 4650 3650 5900
Wire Notes Line
	3650 5900 750  5900
Wire Notes Line
	750  5900 750  4650
Text Notes 750  4650 0    50   ~ 0
LEDs
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 6181BCA6
P 950 3800
F 0 "H1" V 904 3950 50  0000 L CNN
F 1 "MountingHole" V 995 3950 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad" H 950 3800 50  0001 C CNN
F 3 "~" H 950 3800 50  0001 C CNN
	1    950  3800
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 6181ED3E
P 950 4000
F 0 "H2" V 904 4150 50  0000 L CNN
F 1 "MountingHole" V 995 4150 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad" H 950 4000 50  0001 C CNN
F 3 "~" H 950 4000 50  0001 C CNN
	1    950  4000
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 6181F050
P 950 4200
F 0 "H3" V 904 4350 50  0000 L CNN
F 1 "MountingHole" V 995 4350 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad" H 950 4200 50  0001 C CNN
F 3 "~" H 950 4200 50  0001 C CNN
	1    950  4200
	0    1    1    0   
$EndComp
NoConn ~ 850  3800
NoConn ~ 850  4000
NoConn ~ 850  4200
Wire Wire Line
	9000 2150 9750 2150
Wire Wire Line
	8750 2350 9750 2350
Wire Wire Line
	9350 1750 9500 1750
Connection ~ 9500 1750
Wire Wire Line
	9500 1750 9750 1750
Text GLabel 7150 4800 2    50   Input ~ 0
DHT22
Wire Wire Line
	7150 4800 6950 4800
Text GLabel 6050 5400 3    50   Input ~ 0
DISP_SI
Text GLabel 5950 5400 3    50   Input ~ 0
DISP_SCL
Text GLabel 5850 5400 3    50   Input ~ 0
DISP_A0
Text GLabel 5750 5400 3    50   Input ~ 0
DISP_RST
Wire Wire Line
	5650 5400 5650 5200
Wire Wire Line
	5750 5400 5750 5200
Wire Wire Line
	5850 5400 5850 5200
Wire Wire Line
	5950 5400 5950 5200
Wire Wire Line
	6050 5400 6050 5200
Text GLabel 5650 5400 3    50   Input ~ 0
DISP_CS
$Comp
L Device:R_Small R9
U 1 1 619F8BB8
P 9750 5550
F 0 "R9" H 9700 5500 50  0000 R CNN
F 1 "10k" H 9700 5600 50  0000 R CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 9750 5550 50  0001 C CNN
F 3 "~" H 9750 5550 50  0001 C CNN
	1    9750 5550
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R10
U 1 1 619F8BC2
P 9850 5550
F 0 "R10" H 9800 5500 50  0000 R CNN
F 1 "10k" H 9800 5600 50  0000 R CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 9850 5550 50  0001 C CNN
F 3 "~" H 9850 5550 50  0001 C CNN
	1    9850 5550
	-1   0    0    1   
$EndComp
Wire Wire Line
	9750 5800 9750 5650
Wire Wire Line
	9850 5900 9850 5650
Wire Wire Line
	9750 5450 9750 5400
Wire Wire Line
	9750 5400 9800 5400
Wire Wire Line
	9850 5400 9850 5450
$Comp
L power:+3V3 #PWR0134
U 1 1 619F8BD1
P 9800 5400
F 0 "#PWR0134" H 9800 5250 50  0001 C CNN
F 1 "+3V3" H 9815 5573 50  0000 C CNN
F 2 "" H 9800 5400 50  0001 C CNN
F 3 "" H 9800 5400 50  0001 C CNN
	1    9800 5400
	1    0    0    -1  
$EndComp
Connection ~ 9800 5400
Wire Wire Line
	9800 5400 9850 5400
Connection ~ 9750 5800
Wire Wire Line
	9750 5800 10200 5800
Connection ~ 9850 5900
Wire Wire Line
	9850 5900 10200 5900
Text GLabel 4700 3900 0    50   Input ~ 0
UART_RXD
Wire Wire Line
	4700 3800 4900 3800
Text GLabel 4700 3800 0    50   Input ~ 0
UART_TXD
Wire Wire Line
	4700 3900 4900 3900
Text GLabel 4700 4800 0    50   Input ~ 0
S8_RXD
Text GLabel 4700 4900 0    50   Input ~ 0
S8_TXD
Wire Wire Line
	4700 4900 4900 4900
Wire Wire Line
	4900 4800 4700 4800
Text GLabel 4700 4400 0    50   Input ~ 0
LED_YELLOW
Wire Wire Line
	4900 4300 4700 4300
Wire Wire Line
	4700 4400 4900 4400
Wire Wire Line
	4900 4500 4700 4500
Connection ~ 3850 1500
Wire Wire Line
	3850 1500 4250 1500
Connection ~ 4250 1500
Wire Wire Line
	4250 1500 4550 1500
$Comp
L AirQualitySensor:DHT22 U2
U 1 1 615E6A6B
P 3800 6750
F 0 "U2" H 4328 6646 50  0000 L CNN
F 1 "DHT22" H 4328 6555 50  0000 L CNN
F 2 "AirQualitySensor:DHT22" H 4550 6250 50  0001 C CNN
F 3 "https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf" H 4550 6250 50  0001 C CNN
	1    3800 6750
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_Small D4
U 1 1 6161A228
P 2700 5600
F 0 "D4" V 2746 5530 50  0000 R CNN
F 1 "Status0" V 2655 5530 50  0000 R CNN
F 2 "LED_SMD:LED_0805_2012Metric" V 2700 5600 50  0001 C CNN
F 3 "~" V 2700 5600 50  0001 C CNN
	1    2700 5600
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R11
U 1 1 6161A246
P 2700 5400
F 0 "R11" H 2641 5354 50  0000 R CNN
F 1 "20k" H 2641 5445 50  0000 R CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 2700 5400 50  0001 C CNN
F 3 "~" H 2700 5400 50  0001 C CNN
	1    2700 5400
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0135
U 1 1 6161A250
P 2700 5700
F 0 "#PWR0135" H 2700 5450 50  0001 C CNN
F 1 "GND" H 2650 5650 50  0000 R CNN
F 2 "" H 2700 5700 50  0001 C CNN
F 3 "" H 2700 5700 50  0001 C CNN
	1    2700 5700
	1    0    0    -1  
$EndComp
Text GLabel 2700 5250 1    50   Input ~ 0
LED_STATUS0
Wire Wire Line
	2700 5250 2700 5300
$Comp
L Device:LED_Small D5
U 1 1 61620D58
P 3200 5600
F 0 "D5" V 3246 5530 50  0000 R CNN
F 1 "Status1" V 3155 5530 50  0000 R CNN
F 2 "LED_SMD:LED_0805_2012Metric" V 3200 5600 50  0001 C CNN
F 3 "~" V 3200 5600 50  0001 C CNN
	1    3200 5600
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R12
U 1 1 61620D76
P 3200 5400
F 0 "R12" H 3141 5354 50  0000 R CNN
F 1 "20k" H 3141 5445 50  0000 R CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 3200 5400 50  0001 C CNN
F 3 "~" H 3200 5400 50  0001 C CNN
	1    3200 5400
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0136
U 1 1 61620D80
P 3200 5700
F 0 "#PWR0136" H 3200 5450 50  0001 C CNN
F 1 "GND" H 3150 5650 50  0000 R CNN
F 2 "" H 3200 5700 50  0001 C CNN
F 3 "" H 3200 5700 50  0001 C CNN
	1    3200 5700
	1    0    0    -1  
$EndComp
Text GLabel 3200 5250 1    50   Input ~ 0
LED_STATUS1
Wire Wire Line
	3200 5250 3200 5300
Text GLabel 4700 4600 0    50   Input ~ 0
LED_STATUS0
Text GLabel 4700 4700 0    50   Input ~ 0
LED_STATUS1
Wire Wire Line
	4700 4700 4900 4700
Wire Wire Line
	4700 4600 4900 4600
Connection ~ 5350 5350
NoConn ~ 5550 5200
Text GLabel 4700 4300 0    50   Input ~ 0
LED_RED
Text GLabel 4700 4500 0    50   Input ~ 0
LED_GREEN
Wire Wire Line
	2150 1050 2600 1050
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 6170CD2E
P 3150 850
F 0 "#FLG0101" H 3150 925 50  0001 C CNN
F 1 "PWR_FLAG" V 3150 978 50  0000 L CNN
F 2 "" H 3150 850 50  0001 C CNN
F 3 "~" H 3150 850 50  0001 C CNN
	1    3150 850 
	0    1    1    0   
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 6170DB9F
P 3150 950
F 0 "#FLG0102" H 3150 1025 50  0001 C CNN
F 1 "PWR_FLAG" V 3150 1078 50  0000 L CNN
F 2 "" H 3150 950 50  0001 C CNN
F 3 "~" H 3150 950 50  0001 C CNN
	1    3150 950 
	0    1    1    0   
$EndComp
Wire Wire Line
	3150 950  3100 950 
Wire Wire Line
	3100 950  3100 1050
Wire Wire Line
	2950 1050 3100 1050
Connection ~ 3100 1050
Wire Wire Line
	3100 1050 4250 1050
Wire Wire Line
	3150 850  3050 850 
Wire Wire Line
	3050 850  3050 1500
Connection ~ 3050 1500
Wire Wire Line
	3050 1500 3250 1500
$EndSCHEMATC
