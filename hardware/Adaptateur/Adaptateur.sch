EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Adaptateur USB XBEE"
Date "2020-09-14"
Rev "1.0"
Comp "INSA GEI"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Interface_USB:FT232RL U1
U 1 1 5F5B8385
P 4150 4650
F 0 "U1" H 3550 5700 50  0000 C CNN
F 1 "FT232RL" H 3650 5600 50  0000 C CNN
F 2 "Package_SO:SSOP-28_5.3x10.2mm_P0.65mm" H 5250 3750 50  0001 C CNN
F 3 "https://www.ftdichip.com/Support/Documents/DataSheets/ICs/DS_FT232R.pdf" H 4150 4650 50  0001 C CNN
	1    4150 4650
	1    0    0    -1  
$EndComp
$Comp
L Connector:USB_B J1
U 1 1 5F5B8C99
P 1350 1600
F 0 "J1" H 1407 2067 50  0000 C CNN
F 1 "USB_B" H 1407 1976 50  0000 C CNN
F 2 "Connector_USB:USB_B_Lumberg_2411_02_Horizontal" H 1500 1550 50  0001 C CNN
F 3 " ~" H 1500 1550 50  0001 C CNN
	1    1350 1600
	1    0    0    -1  
$EndComp
$Comp
L Insa:XBee U2
U 1 1 5F5B9952
P 8900 4550
F 0 "U2" H 9350 5700 50  0000 C CNN
F 1 "XBee" H 9300 5600 50  0000 C CNN
F 2 "Insa:Xbee3" H 8900 4350 50  0001 C CNN
F 3 "" H 8900 4350 50  0001 C CNN
	1    8900 4550
	1    0    0    -1  
$EndComp
$Comp
L Device:C C6
U 1 1 5F5B9798
P 8150 3250
F 0 "C6" V 8402 3250 50  0000 C CNN
F 1 "100nF" V 8311 3250 50  0000 C CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8188 3100 50  0001 C CNN
F 3 "~" H 8150 3250 50  0001 C CNN
	1    8150 3250
	-1   0    0    1   
$EndComp
$Comp
L Device:C C4
U 1 1 5F5BA3DC
P 5250 1450
F 0 "C4" H 5365 1496 50  0000 L CNN
F 1 "100nF" H 5365 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5288 1300 50  0001 C CNN
F 3 "~" H 5250 1450 50  0001 C CNN
	1    5250 1450
	-1   0    0    1   
$EndComp
$Comp
L Device:C C1
U 1 1 5F5BA884
P 3250 1550
F 0 "C1" H 3365 1596 50  0000 L CNN
F 1 "100nF" H 3365 1505 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 3288 1400 50  0001 C CNN
F 3 "~" H 3250 1550 50  0001 C CNN
	1    3250 1550
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5F5BAD59
P 2500 1550
F 0 "C2" H 2615 1596 50  0000 L CNN
F 1 "10nF" H 2615 1505 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2538 1400 50  0001 C CNN
F 3 "~" H 2500 1550 50  0001 C CNN
	1    2500 1550
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Polarized C3
U 1 1 5F5BBA65
P 3700 1550
F 0 "C3" H 3818 1596 50  0000 L CNN
F 1 "2.2µF" H 3818 1505 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3528-21_Kemet-B_Pad1.50x2.35mm_HandSolder" H 3738 1400 50  0001 C CNN
F 3 "~" H 3700 1550 50  0001 C CNN
	1    3700 1550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0102
U 1 1 5F5CA692
P 10100 1050
F 0 "#PWR0102" H 10100 900 50  0001 C CNN
F 1 "+5V" H 10115 1223 50  0000 C CNN
F 2 "" H 10100 1050 50  0001 C CNN
F 3 "" H 10100 1050 50  0001 C CNN
	1    10100 1050
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5F5CB4FF
P 10450 1050
F 0 "#FLG0102" H 10450 1125 50  0001 C CNN
F 1 "PWR_FLAG" H 10450 1223 50  0000 C CNN
F 2 "" H 10450 1050 50  0001 C CNN
F 3 "~" H 10450 1050 50  0001 C CNN
	1    10450 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	10100 1050 10450 1050
$Comp
L power:GND #PWR0104
U 1 1 5F5D6C7D
P 8150 3400
F 0 "#PWR0104" H 8150 3150 50  0001 C CNN
F 1 "GND" H 8155 3227 50  0000 C CNN
F 2 "" H 8150 3400 50  0001 C CNN
F 3 "" H 8150 3400 50  0001 C CNN
	1    8150 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 5F5D70FB
P 8250 4750
F 0 "#PWR0105" H 8250 4500 50  0001 C CNN
F 1 "GND" H 8255 4577 50  0000 C CNN
F 2 "" H 8250 4750 50  0001 C CNN
F 3 "" H 8250 4750 50  0001 C CNN
	1    8250 4750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 5F5D79FD
P 4150 5900
F 0 "#PWR0106" H 4150 5650 50  0001 C CNN
F 1 "GND" H 4155 5727 50  0000 C CNN
F 2 "" H 4150 5900 50  0001 C CNN
F 3 "" H 4150 5900 50  0001 C CNN
	1    4150 5900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 5F5D8487
P 1350 2250
F 0 "#PWR0107" H 1350 2000 50  0001 C CNN
F 1 "GND" H 1355 2077 50  0000 C CNN
F 2 "" H 1350 2250 50  0001 C CNN
F 3 "" H 1350 2250 50  0001 C CNN
	1    1350 2250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 5F5D867D
P 3700 2250
F 0 "#PWR0108" H 3700 2000 50  0001 C CNN
F 1 "GND" H 3705 2077 50  0000 C CNN
F 2 "" H 3700 2250 50  0001 C CNN
F 3 "" H 3700 2250 50  0001 C CNN
	1    3700 2250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0109
U 1 1 5F5DA8CE
P 3700 1250
F 0 "#PWR0109" H 3700 1100 50  0001 C CNN
F 1 "+5V" H 3715 1423 50  0000 C CNN
F 2 "" H 3700 1250 50  0001 C CNN
F 3 "" H 3700 1250 50  0001 C CNN
	1    3700 1250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0110
U 1 1 5F5DB96C
P 4250 3350
F 0 "#PWR0110" H 4250 3200 50  0001 C CNN
F 1 "+5V" H 4265 3523 50  0000 C CNN
F 2 "" H 4250 3350 50  0001 C CNN
F 3 "" H 4250 3350 50  0001 C CNN
	1    4250 3350
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0111
U 1 1 5F5DE489
P 6450 1100
F 0 "#PWR0111" H 6450 950 50  0001 C CNN
F 1 "+3.3V" H 6465 1273 50  0000 C CNN
F 2 "" H 6450 1100 50  0001 C CNN
F 3 "" H 6450 1100 50  0001 C CNN
	1    6450 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 1600 2150 1600
Wire Wire Line
	1650 1700 2150 1700
Wire Wire Line
	3350 4250 2600 4250
Wire Wire Line
	3350 4350 2600 4350
Wire Wire Line
	4950 3950 5750 3950
Wire Wire Line
	4950 4050 5750 4050
Wire Wire Line
	4950 4150 5750 4150
Wire Wire Line
	4950 4250 5750 4250
Wire Wire Line
	8350 3750 7550 3750
Wire Wire Line
	8350 3850 7550 3850
Wire Wire Line
	8350 4450 7550 4450
Wire Wire Line
	9500 4450 10300 4450
Wire Wire Line
	9500 4050 10300 4050
Wire Wire Line
	8350 3650 8350 3100
Wire Wire Line
	8350 4550 8250 4550
Wire Wire Line
	8250 4550 8250 4750
NoConn ~ 9500 3650
NoConn ~ 9500 3750
NoConn ~ 9500 3850
NoConn ~ 9500 3950
NoConn ~ 9500 4150
NoConn ~ 9500 4350
NoConn ~ 8350 4250
NoConn ~ 8350 4150
NoConn ~ 8350 4050
NoConn ~ 8350 3950
NoConn ~ 9500 4550
NoConn ~ 4950 4550
NoConn ~ 4950 4650
NoConn ~ 4950 4950
NoConn ~ 4950 5050
NoConn ~ 4950 5150
NoConn ~ 4950 5250
NoConn ~ 4950 5350
NoConn ~ 3350 5050
NoConn ~ 3350 4850
NoConn ~ 3350 4650
Wire Wire Line
	4350 5650 4250 5650
Connection ~ 4150 5650
Wire Wire Line
	4150 5650 3950 5650
Connection ~ 4250 5650
Wire Wire Line
	4250 5650 4150 5650
Wire Wire Line
	3950 5650 2900 5650
Wire Wire Line
	2900 5650 2900 5350
Connection ~ 3950 5650
Wire Wire Line
	3350 5350 2900 5350
Wire Wire Line
	4150 5650 4150 5900
Wire Wire Line
	1250 2000 1350 2000
Wire Wire Line
	1350 2250 1350 2000
Connection ~ 1350 2000
Wire Wire Line
	1650 1400 2500 1400
Wire Wire Line
	3250 1400 3700 1400
Wire Wire Line
	3700 1250 3700 1400
Wire Wire Line
	2500 1700 3250 1700
Connection ~ 3250 1700
Wire Wire Line
	3250 1700 3700 1700
Wire Wire Line
	3700 2250 3700 1700
Wire Wire Line
	4050 3650 4250 3650
Wire Wire Line
	4250 3350 4250 3650
Connection ~ 4250 3650
Text Label 2150 1600 2    50   ~ 0
USB_DP
Text Label 2150 1700 2    50   ~ 0
USB_DM
Text Label 2600 4250 0    50   ~ 0
USB_DP
Text Label 2600 4350 0    50   ~ 0
USB_DM
Text Label 5750 3950 2    50   ~ 0
USART_TXD
Text Label 5750 4050 2    50   ~ 0
USART_RXD
Text Label 5750 4150 2    50   ~ 0
USART_RTS
Text Label 5750 4250 2    50   ~ 0
USART_CTS
Text Label 7550 3750 0    50   ~ 0
USART_RXD
Text Label 7550 3850 0    50   ~ 0
USART_TXD
Text Label 7550 4450 0    50   ~ 0
USART_DCR
Text Label 10300 4050 2    50   ~ 0
USART_CTS
Text Label 10300 4450 2    50   ~ 0
USART_RTS
Wire Wire Line
	4950 4450 5750 4450
Text Label 5750 4450 2    50   ~ 0
USART_DCR
NoConn ~ 4950 4350
$Comp
L Regulator_Linear:IFX27001TFV33 U3
U 1 1 5F601092
P 5850 1200
F 0 "U3" H 5850 1467 50  0000 C CNN
F 1 "IFX27001TFV33" H 5850 1376 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-252-3_TabPin2" H 5850 1150 50  0001 C CNN
F 3 "https://static6.arrow.com/aropdfconversion/dc75757ae45a88e5f69bdce3f2a651a5fe0ca07d/ifx27001_ds_10.pdf" H 5850 1150 50  0001 C CNN
	1    5850 1200
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 5F601731
P 2250 4150
F 0 "D1" V 2289 4032 50  0000 R CNN
F 1 "LED" V 2198 4032 50  0000 R CNN
F 2 "LED_SMD:LED_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2250 4150 50  0001 C CNN
F 3 "~" H 2250 4150 50  0001 C CNN
	1    2250 4150
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D2
U 1 1 5F601F6A
P 7400 1150
F 0 "D2" V 7439 1032 50  0000 R CNN
F 1 "LED" V 7348 1032 50  0000 R CNN
F 2 "LED_SMD:LED_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 7400 1150 50  0001 C CNN
F 3 "~" H 7400 1150 50  0001 C CNN
	1    7400 1150
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 5F602CC2
P 7400 1700
F 0 "R2" H 7330 1654 50  0000 R CNN
F 1 "1K" H 7330 1745 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 7330 1700 50  0001 C CNN
F 3 "~" H 7400 1700 50  0001 C CNN
	1    7400 1700
	-1   0    0    1   
$EndComp
$Comp
L Device:R R1
U 1 1 5F6032C9
P 2250 4650
F 0 "R1" H 2180 4604 50  0000 R CNN
F 1 "1K" H 2180 4695 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 2180 4650 50  0001 C CNN
F 3 "~" H 2250 4650 50  0001 C CNN
	1    2250 4650
	-1   0    0    1   
$EndComp
$Comp
L Device:FerriteBead FB1
U 1 1 5F603B02
P 2900 1400
F 0 "FB1" V 3174 1400 50  0000 C CNN
F 1 "MI1210K600R-10" V 3083 1400 50  0000 C CNN
F 2 "Resistor_SMD:R_1210_3225Metric_Pad1.42x2.65mm_HandSolder" V 2830 1400 50  0001 C CNN
F 3 "~" H 2900 1400 50  0001 C CNN
	1    2900 1400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2500 1400 2750 1400
Connection ~ 2500 1400
Wire Wire Line
	3050 1400 3250 1400
Connection ~ 3250 1400
$Comp
L power:VCC #PWR0112
U 1 1 5F6150A2
P 2500 1250
F 0 "#PWR0112" H 2500 1100 50  0001 C CNN
F 1 "VCC" H 2515 1423 50  0000 C CNN
F 2 "" H 2500 1250 50  0001 C CNN
F 3 "" H 2500 1250 50  0001 C CNN
	1    2500 1250
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0113
U 1 1 5F615A29
P 5250 1100
F 0 "#PWR0113" H 5250 950 50  0001 C CNN
F 1 "VCC" H 5265 1273 50  0000 C CNN
F 2 "" H 5250 1100 50  0001 C CNN
F 3 "" H 5250 1100 50  0001 C CNN
	1    5250 1100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0115
U 1 1 5F644B30
P 5850 1850
F 0 "#PWR0115" H 5850 1600 50  0001 C CNN
F 1 "GND" H 5855 1677 50  0000 C CNN
F 2 "" H 5850 1850 50  0001 C CNN
F 3 "" H 5850 1850 50  0001 C CNN
	1    5850 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 3950 2250 3950
Wire Wire Line
	2250 3950 2250 4000
Wire Wire Line
	2250 4300 2250 4500
Wire Wire Line
	2900 5650 2250 5650
Wire Wire Line
	2250 5650 2250 4800
Connection ~ 2900 5650
Wire Wire Line
	5250 1100 5250 1200
Wire Wire Line
	5550 1200 5250 1200
Wire Wire Line
	5250 1700 5850 1700
Wire Wire Line
	5850 1500 5850 1700
Connection ~ 5850 1700
Wire Wire Line
	5850 1700 6450 1700
Wire Wire Line
	5850 1700 5850 1850
Wire Wire Line
	6150 1200 6450 1200
Wire Wire Line
	6450 1200 6450 1100
Wire Wire Line
	7400 1300 7400 1550
$Comp
L power:GND #PWR0116
U 1 1 5F652189
P 7400 2050
F 0 "#PWR0116" H 7400 1800 50  0001 C CNN
F 1 "GND" H 7405 1877 50  0000 C CNN
F 2 "" H 7400 2050 50  0001 C CNN
F 3 "" H 7400 2050 50  0001 C CNN
	1    7400 2050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0117
U 1 1 5F654B0D
P 7400 850
F 0 "#PWR0117" H 7400 700 50  0001 C CNN
F 1 "+3.3V" H 7415 1023 50  0000 C CNN
F 2 "" H 7400 850 50  0001 C CNN
F 3 "" H 7400 850 50  0001 C CNN
	1    7400 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 850  7400 1000
Wire Wire Line
	7400 1850 7400 2050
Wire Wire Line
	2500 1250 2500 1400
Wire Wire Line
	8150 3100 8350 3100
$Comp
L power:+3.3V #PWR0118
U 1 1 5F6722CF
P 8350 3100
F 0 "#PWR0118" H 8350 2950 50  0001 C CNN
F 1 "+3.3V" H 8365 3273 50  0000 C CNN
F 2 "" H 8350 3100 50  0001 C CNN
F 3 "" H 8350 3100 50  0001 C CNN
	1    8350 3100
	1    0    0    -1  
$EndComp
Connection ~ 8350 3100
$Comp
L Device:C_Polarized C5
U 1 1 5F676238
P 6450 1450
F 0 "C5" H 6568 1496 50  0000 L CNN
F 1 "10µF" H 6568 1405 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-6032-28_Kemet-C_Pad2.25x2.35mm_HandSolder" H 6488 1300 50  0001 C CNN
F 3 "~" H 6450 1450 50  0001 C CNN
	1    6450 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 1200 5250 1300
Connection ~ 5250 1200
Wire Wire Line
	6450 1600 6450 1700
Wire Wire Line
	6450 1200 6450 1300
Connection ~ 6450 1200
Wire Wire Line
	5250 1600 5250 1700
$Comp
L Device:C C7
U 1 1 5F6CD83F
P 1800 4650
F 0 "C7" H 1915 4696 50  0000 L CNN
F 1 "100nF" H 1915 4605 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 1838 4500 50  0001 C CNN
F 3 "~" H 1800 4650 50  0001 C CNN
	1    1800 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 3950 1800 3950
Wire Wire Line
	1800 3950 1800 4500
Connection ~ 2250 3950
Wire Wire Line
	2250 5650 1800 5650
Wire Wire Line
	1800 5650 1800 4800
Connection ~ 2250 5650
$EndSCHEMATC
