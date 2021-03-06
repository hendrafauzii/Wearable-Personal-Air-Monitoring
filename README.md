
# Wearable Personal Air Monitoring

## Daftar Isi
 - [Daftar Isi](#Daftar-isi)  
 - [Deskripsi](#Deskripsi) 
 - [Library Depedency](#Library-depedency)
 - [PCB Design](#PCB-Design) 
 - [Bill of Materials](#BOMs) 
 - [Dashboard](#dasboard-pada-platform-iot-xl-axiata-flex-iot)
 - [Galeri](#Galeri)


## Deskripsi
Perangkat ini dirancang pada tahun 2019 yang dibuat menyerupai sebuah jam tangan bagi penderita sinusitis yang sensitif terhadap perubahan udara. Terdapat beberapa sensor yang digunakan diantaranya  sensor BME280 sebagai pengukuran suhu, kelembapan, dan tekanan udara secara real time, dan MAX30105 sebagai sensor tambahan yang berfungsi untuk mengukur detak jantung. Alat ini akan memberikan notifikasi ketika kondisi udara tidak ideal bagi penderita sinusitis.


## Library Depedency
Perangkat ini diprogram menggunakan ArduinoIDE yang berbasis C/C++, dengan memilih ESP32 Dev Module pada bagian board di ArduinoIDE.
Adapun library depedency yang digunakan diantaranya :

**• Adafruit Sensor** https://github.com/adafruit/Adafruit_Sensor

**• BME280** https://downloads.arduino.cc/libraries/github.com/adafruit/Adafruit_BME280_Library-1.1.0.zip

**• MAX3010x** https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library

**• SSD1306 Display** https://github.com/ThingPulse/esp8266-oled-ssd1306

**• PubSub Client** https://github.com/knolleary/pubsubclient

**• ArduinoJson v5** https://github.com/bblanchon/ArduinoJson/tree/5.x


## PCB Design
<p align="left"> 
  <img width="600" src="https://raw.githubusercontent.com/hendrafauzii/Wearable-Personal-Air-Monitoring/master/Images/layout_board.png">
</p>


## BOMs
**• Bagian Atas**
No | Parts          | Value       | Package                  | Qty 
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

**• Bagian Bawah**
No | Parts              | Value    | Package          | Qty 
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


## Dasboard pada Platform IoT XL Axiata (Flex-IoT)
Perangkat ini dihubungkan dengan Platform IoT XL Axiata [(Flex-IoT)](https://portal.flexiot.xl.co.id/) menggunakan WiFi dengan Protokol MQTT, sehingga dapat dibuat dashboard untuk monitoring seperti yang terlihat pada gambar berikut. 

<p align="left"> 
  <img width="600" src="https://raw.githubusercontent.com/hendrafauzii/Wearable-Personal-Air-Monitoring/master/Images/dashboard.png">
</p>


## Galeri
No |  Gambar                                                                                                                     | Keterangan         
-- |  -------------------------------------------------------------------------------------------------------------------------- | ------------------------- |
1  |  <img src="https://github.com/hendrafauzii/Wearable-Personal-Air-Monitoring/blob/master/Images/20190819_1.jpg" width="150"> | Menghubungkan ke WiFi     |
2  |  <img src="https://github.com/hendrafauzii/Wearable-Personal-Air-Monitoring/blob/master/Images/20190819_2.jpg" width="150"> |	Menghubungkan dengan MQTT |
3  |  <img src="https://github.com/hendrafauzii/Wearable-Personal-Air-Monitoring/blob/master/Images/20190819_3.jpg" width="150"> |	Tampilan utama berupa suhu lingkungan sekitar,      kelembapan udara, dan jam digital.          |
4  |  <img src="https://github.com/hendrafauzii/Wearable-Personal-Air-Monitoring/blob/master/Images/20190819_4.jpg" width="150"> | Slide kedua merupakan ***altitude*** atau ketinggian dari permukaan bumi.     |
5  |  <img src="https://github.com/hendrafauzii/Wearable-Personal-Air-Monitoring/blob/master/Images/20190819_5.jpg" width="150"> |	Slide ketiga merupakan tekanan udara sekitar. |
6  |  <img src="https://github.com/hendrafauzii/Wearable-Personal-Air-Monitoring/blob/master/Images/20190819_6.jpg" width="150"> |	Pengaturan zona wktu dan batas suhu maksimal dan minimum |

