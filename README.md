
# Wearable Personal Air Monitoring

## Daftar Isi
 - [Daftar Isi](#Daftar-isi)  
 - [Deskripsi](#Deskripsi) 
 - [PCB Design](#PCB-Design) 
 - [Bill of Materials](#BOMs) 

## Deskripsi
Alat ini dirancang menyerupai sebuah jam tangan bagi penderita sinusitis yang sensitif terhadap perubahan udara. Terdapat beberapa sensor yang digunakan diantaranya  sensor BME280 sebagai pengukuran suhu, kelembapan, dan tekanan udara secara real time, dan MAX30105 sebagai sensor tambahan yang berfungsi untuk mengukur detak jantung. Alat ini akan memberikan notifikasi ketika kondisi udara tidak ideal bagi penderita sinusitis.

## PCB Design
<p align="center"> 
  <img width="400" src="https://raw.githubusercontent.com/hendrafauzii/Wearable-Personal-Air-Monitoring/master/Images/layout_board.png">
</p>

## BOMs
No | Parts          | Value       | **Package**              | Qty |
-- | -------------- | ----------- | ------------------------ | --- |
1  | LED1           | LED         | CHIPLED_0805             |  1  |
2  | SV1	           | MA05        | MA05-2	                  |  1  |
3  | JP1            | PIN HEAD    | 1X06/90                  |  1  |
4  | U$3, U$4       | SIDE SWITCH | SIDE_SWITCH              |  2  |
5  | R1, R2, R4, R5 | 10K	        | R0805                    |  4  |
6  | R3	            | 1K	         | R0805                    |  1  |
7  | U$1	           | BME280	     | BME280                   |	 1  |
8  | OLED-DISPLAY   | OLED-128X64 | 128X64-I2C               |	 1  |
9  | U1	            | ESP32	      | ESP32-WROOM              |  1  |
10 | J1	            | MICRO USB	  | GCT_USB3065-XX-00-A_REVE |  1  |
11 | Q1, Q2	        | MMBT2222A	  | SOT23                    |  2  |
12 | X1	            | POWER       | B2B-PH-SM4-TB            |  1  |
13 | U$2	           | SWITCH	     | MSK-12C02                |  1  |
=======================================================================
No | Parts              | Value    | Package          | Qty |
-- | ------------------ | -------- | ---------------- | --- |
1  | CHRG, STBY	        | LED      | CHIPLED_0805     |	 2  |
2  | MAX30100	          | MAX30100 |	MA04-1           |	 1  |
3  | SV1                | MA05-2   |	MA05-2           |	 1  |
4  | 0 (Not Used)       | 0        |	R0805            |	 1  |
5  | R1, R2, R3, R4, R8 | 10K      |	R0805            |	 5  |
6  | C1, C2, C3         | 10uF     |	C0805            |  3  |
7  | R5, R6             | 1K       |	R0805            |  2  |
8  | C4                 | 1uF      |	C0805            |  1  |
9  | R7                 | 2K       |	R0805            |  1  |
10 | U2                 | AP2112K  |	SOT95P285X140-5N |  1  |
11 | S1                 |	BUT1     |	SIDE_SWITCH      |  1  |
12 | S2                 |	BUT2     |	SIDE_SWITCH	     |  1  |
13 | S3                 |	BUT3     |	SIDE_SWITCH	     |  1  |
14 | S4                 |	BUT4     |	SIDE_SWITCH	     |  1  |
15 | IC1                |	TP4056   |	SOP-8	           |  1  |

