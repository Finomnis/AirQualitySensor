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
L AirQualitySensor:Waveshare_Core52840 A1
U 1 1 615D64F0
P 4000 2550
F 0 "A1" H 4000 3515 50  0000 C CNN
F 1 "Waveshare_Core52840" H 4000 3424 50  0000 C CNN
F 2 "AirQualitySensor:Waveshare_Core52840" H 4000 3300 50  0001 C CNN
F 3 "https://infocenter.nordicsemi.com/pdf/nRF52840_PS_v1.1.pdf" H 3950 2650 50  0001 C CNN
	1    4000 2550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 615D7685
P 5000 1950
F 0 "#PWR0101" H 5000 1700 50  0001 C CNN
F 1 "GND" V 5005 1822 50  0000 R CNN
F 2 "" H 5000 1950 50  0001 C CNN
F 3 "" H 5000 1950 50  0001 C CNN
	1    5000 1950
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 615D7DF8
P 3000 1950
F 0 "#PWR0102" H 3000 1700 50  0001 C CNN
F 1 "GND" V 3005 1822 50  0000 R CNN
F 2 "" H 3000 1950 50  0001 C CNN
F 3 "" H 3000 1950 50  0001 C CNN
	1    3000 1950
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 615D864F
P 5000 3150
F 0 "#PWR0103" H 5000 2900 50  0001 C CNN
F 1 "GND" V 5005 3022 50  0000 R CNN
F 2 "" H 5000 3150 50  0001 C CNN
F 3 "" H 5000 3150 50  0001 C CNN
	1    5000 3150
	0    -1   -1   0   
$EndComp
NoConn ~ 3000 2050
NoConn ~ 3000 2150
NoConn ~ 3000 2250
NoConn ~ 3000 2350
NoConn ~ 3000 2450
NoConn ~ 3000 2550
NoConn ~ 3000 2650
NoConn ~ 3000 2750
NoConn ~ 3000 2850
NoConn ~ 3000 2950
NoConn ~ 3000 3050
NoConn ~ 3000 3150
NoConn ~ 3600 3450
NoConn ~ 3700 3450
NoConn ~ 3800 3450
NoConn ~ 3900 3450
NoConn ~ 4000 3450
NoConn ~ 4100 3450
NoConn ~ 4200 3450
$Comp
L power:+5V #PWR0104
U 1 1 615DFE75
P 4300 3450
F 0 "#PWR0104" H 4300 3300 50  0001 C CNN
F 1 "+5V" H 4315 3623 50  0000 C CNN
F 2 "" H 4300 3450 50  0001 C CNN
F 3 "" H 4300 3450 50  0001 C CNN
	1    4300 3450
	-1   0    0    1   
$EndComp
$Comp
L power:+3V3 #PWR0105
U 1 1 615E0AF7
P 3500 3450
F 0 "#PWR0105" H 3500 3300 50  0001 C CNN
F 1 "+3V3" H 3515 3623 50  0000 C CNN
F 2 "" H 3500 3450 50  0001 C CNN
F 3 "" H 3500 3450 50  0001 C CNN
	1    3500 3450
	-1   0    0    1   
$EndComp
NoConn ~ 5000 3050
NoConn ~ 5000 2950
NoConn ~ 5000 2850
NoConn ~ 5000 2750
NoConn ~ 5000 2650
NoConn ~ 5000 2350
NoConn ~ 5000 2250
NoConn ~ 5000 2150
NoConn ~ 5000 2050
$Comp
L AirQualitySensor:EA_DOGM132W-5 DS1
U 1 1 615E0422
P 7350 2150
F 0 "DS1" H 7950 2415 50  0000 C CNN
F 1 "EA_DOGM132W-5" H 7950 2324 50  0000 C CNN
F 2 "AirQualitySensor:EA_DOGM132W-5" H 8400 2250 50  0001 L CNN
F 3 "" H 8400 2150 50  0001 L CNN
F 4 "LCD Graphic Display Modules & Accessories DOGM Grapic 132x32 White Background" H 8400 2050 50  0001 L CNN "Description"
F 5 "2" H 8400 1950 50  0001 L CNN "Height"
F 6 "790-EADOGM132W5" H 8400 1850 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/ELECTRONIC-ASSEMBLY/EA-DOGM132W-5?qs=s9z6UkyjM7o689ToJkW0yA%3D%3D" H 8400 1750 50  0001 L CNN "Mouser Price/Stock"
F 8 "ELECTRONIC ASSEMBLY" H 8400 1650 50  0001 L CNN "Manufacturer_Name"
F 9 "EA DOGM132W-5" H 8400 1550 50  0001 L CNN "Manufacturer_Part_Number"
	1    7350 2150
	1    0    0    -1  
$EndComp
$Comp
L AirQualitySensor:004-0-0053 U1
U 1 1 615E61D2
P 3950 5000
F 0 "U1" H 3950 5581 50  0000 C CNN
F 1 "004-0-0053" H 3950 5490 50  0000 C CNN
F 2 "AirQualitySensor:Senseair_S8_Up" H 3950 5000 50  0001 C CNN
F 3 "https://rmtplusstoragesenseair.blob.core.windows.net/docs/publicerat/PSP126.pdf" H 3950 5000 50  0001 C CNN
	1    3950 5000
	1    0    0    -1  
$EndComp
$Comp
L AirQualitySensor:DHT22 U2
U 1 1 615E6A6B
P 5750 4550
F 0 "U2" H 6278 4446 50  0000 L CNN
F 1 "DHT22" H 6278 4355 50  0000 L CNN
F 2 "AirQualitySensor:DHT22" H 6500 4050 50  0001 C CNN
F 3 "https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf" H 6500 4050 50  0001 C CNN
	1    5750 4550
	1    0    0    -1  
$EndComp
$EndSCHEMATC
