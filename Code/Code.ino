#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "SSD1306Wire.h" 
#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "EEPROM.h"
#include "time.h" 
#include "font.h"
#include "logo.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define I2C_SDA 21
#define I2C_SCL 22
#define BME280_ADD 0x76
#define OLED_ADD 0x3c
#define butt1 18
#define butt2 19
#define butt3 23
#define butt4 17

/*
 * Configure MQTT
String SUB_TOPIC_STRING = 
const char* topic_pub = 
const char* mqtt_server = 
#define mqtt_user ""
#define mqtt_password ""
*/

const char* ntpServer = "pool.ntp.org";
const int   daylightOffset_sec = 3600;
long  gmtOffset_sec;

SSD1306Wire  display(OLED_ADD, I2C_SDA, I2C_SCL);
Adafruit_BME280 bme;
MAX30105 particleSensor;
WiFiClient espClient;
PubSubClient client(espClient);

char hurufKecil[64] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 
                       'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                       's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '}', '[', ']', '|', '$', 
                       '&', '*', '(', ')', '-', '_', '+', '=', '#', '&', '@', '!', ';', '^', 
                       '<', '>', ',', '.', '/', '?', '%', ':'
                      }; // 0 - 63 data

char hurufBesar[64] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 
                       'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                       'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '{', '}', '[', ']', '|', '$', 
                       '&', '*', '(', ')', '-', '_', '+', '=', '#', '&', '@', '!', ';', '^', 
                       '<', '>', ',', '.', '/', '?', '%',':'
                      }; // 0 - 63 data

const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;
long irValue = 0;
int beatsPerMinute = 0;
int beatAvg;

double temp;
int pressure, altitude, humidity;
char tempHum[32];

unsigned long startMillis;
unsigned long start1Millis;
unsigned long currentMillis;
const unsigned long interval = 1000;

char dayDate[32];
char hourNow[3];
char minuteNow[3];
char secondNow[3];

char ssid[32];
char password[32];
char bufferPass[32];
char saveSSID[32];
char savePASSWORD[32];
char jumlah_network[32];
int numSSID;

char msg[300];
                      
struct tm timeinfo;
int counter;
int progress;

int menu, menuSetting, kondisi;
bool state1, state2, state3, state4;
int waktu;
int x,y,d,e;
int a,c;
int pathIndex;
int pilihanHuruf;
int slide;

int setMinTemp;
int setMaxTemp;
int settTime;
int setAltitude;

int addSSID = 0;
int addPass = 32;
int addTime = 64;
int addMinTemp = 74;
int addMaxTemp = 84;
int addAltitude = 94;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  EEPROM.begin(128);
  display.init();
  display.flipScreenVertically();
  bme.begin(BME280_ADD);
  particleSensor.begin(Wire, I2C_SPEED_FAST);
  particleSensor.setup(); 
  particleSensor.setPulseAmplitudeRed(0x0A); 
  particleSensor.setPulseAmplitudeGreen(0);
  
  for(int i=1; i<=64; i++){
    display.clear();
    display.drawXbm(0, 0, 128, i, isinus_image); 
    display.display();
    delay(10);
  }
  delay(1000);

  for(int i=0; i<=64; i++){
    display.clear();
    display.drawXbm(0, i, 128, 64, isinus_image); 
    display.drawXbm(0, 0, 128, i, xcamp_image); 
    display.display();
    delay(5);
  }
  delay(1000);

  for(int i=0; i<=64; i++){
    display.clear();
    display.drawXbm(0-(i*4), 0, 128, 64, xcamp_image); 
    display.drawXbm(0, 0, 128, i, upi_image); 
    display.display();
    delay(5);
  }
  delay(1000);

  for(int i=1; i<=64; i++){
    display.clear();
    display.drawXbm(0-(i*2), 0, 128, 64, upi_image); 
    display.display();
    delay(5);
  }

  setup_wifi();
  configTime((EEPROM.readInt(addTime) * 60 * 60) - 3600, daylightOffset_sec, ntpServer);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  pinMode(butt1, INPUT);
  pinMode(butt2, INPUT);
  pinMode(butt3, INPUT);
  pinMode(butt4, INPUT);
  delay(100);
  
  startMillis = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  c:
  pilihan(0, 3);
  client.loop();
  if(WiFi.status() != WL_CONNECTED) setup_wifi();   
  if(WiFi.status() == WL_CONNECTED && !client.connected()) reconnect();
  
  BMP280_read();
  currentMillis = millis();
  if(currentMillis - startMillis >= interval){ 
    startMillis = currentMillis;
    sendData_iot("Secure");
  }
  
  getLocalTime(&timeinfo);
  snprintf(tempHum, sizeof(tempHum), "T=%.1f°C  H=%02d%s", temp, humidity, "%");

  if((temp <= EEPROM.readInt(addMinTemp) || temp >= EEPROM.readInt(addMaxTemp)) && state4 == false){
    char warningTime[32];
    char tempAndTime[32];
    double warningTemp = bme.readTemperature();
    strftime(warningTime, sizeof(warningTime), "%H:%M", &timeinfo);
    snprintf(tempAndTime, sizeof(tempAndTime), "%.1f°C in %s", temp, warningTime);
    waktu=0;
    while(1){
      if(millis() - start1Millis >= 500){ 
        start1Millis = millis();
        sendData_iot("Danger");
      }           
      waktu++;
      Serial.println(waktu);
      display.clear();
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.setFont(DejaVu_Sans_Mono_16);
      display.drawString(64, 1, "Warning!!");
      display.setFont(DejaVu_Sans_Mono_9);
      display.drawString(64, 20, "The temperature reaches");
      display.setFont(DejaVu_Sans_Mono_11);
      display.drawString(64, 33, String(tempAndTime));
      display.setTextAlignment(TEXT_ALIGN_RIGHT);
      display.setFont(DejaVu_Sans_Mono_9);
      display.drawString(120, 50, "BUTT4 to Close");
      display.display();  
      delay(1);  
      if(digitalRead(butt4) == 1 || waktu >= 8000){
        while(digitalRead(butt4) == 1);
        break;      
      }
    }
    delay(100);
    state4 = true;
  }
  
  if(temp > EEPROM.readInt(addMinTemp) && temp < EEPROM.readInt(addMaxTemp)) state4 = false;
  
  switch(menu){
    case 0  : if(!getLocalTime(&timeinfo)){
                display.clear();
                display.setTextAlignment(TEXT_ALIGN_CENTER);
                display.setFont(DejaVu_Sans_Mono_16);
                display.drawString(64, 1, "Failled get");
                display.drawString(64, 20, "Time");
                display.setFont(DejaVu_Sans_Mono_9);
                display.drawString(64, 45, "Please reset your device");
                display.display();
              }

              else{
                strftime(dayDate, sizeof(dayDate), "%A, %d %b %Y", &timeinfo);
                strftime(hourNow, sizeof(hourNow), "%H", &timeinfo);
                strftime(minuteNow, sizeof(minuteNow), "%M", &timeinfo);
                strftime(secondNow, sizeof(secondNow), "%S", &timeinfo);
   
                display.clear();
                display.drawRect(0, 52, 128, 1);
                display.setTextAlignment(TEXT_ALIGN_LEFT);
                display.setFont(DejaVu_Sans_Mono_8);
                display.drawString(1, 0, String(dayDate));
                display.setFont(DejaVu_Sans_Mono_10);
                display.drawString(1, 53, "1/4");
                display.setTextAlignment(TEXT_ALIGN_RIGHT);
                display.setFont(DejaVu_Sans_Mono_10);
                display.drawString(127, 53, String(tempHum));
                display.setTextAlignment(TEXT_ALIGN_CENTER);
                display.setFont(DSEG14_Classic_Bold_16);
                display.drawString(64, 12, ":");
                display.setFont(DejaVu_Sans_Mono_11);
                display.drawString(64, 32, String(secondNow));
                display.setFont(DSEG14_Classic_Regular_26);
                display.drawString(30, 14, String(hourNow));
                display.drawString(98, 14, String(minuteNow));
                display.display();
              } 
    break;

    case 1  : display.clear();
              display.drawRect(0, 52, 128, 1);
              display.setTextAlignment(TEXT_ALIGN_LEFT);
              display.setFont(DejaVu_Sans_Mono_9);
              display.drawString(1, 0, "Altitude");
              display.setFont(DejaVu_Sans_Mono_10);
              display.drawString(1, 53, "2/4");
              display.setTextAlignment(TEXT_ALIGN_RIGHT);
              display.setFont(DejaVu_Sans_Mono_10);
              display.drawString(127, 53, String(tempHum));

              display.drawXbm(10, 7, 46, 46, altitude_image);
              display.setTextAlignment(TEXT_ALIGN_LEFT);
              display.setFont(ArialMT_Plain_24);
              display.drawString(60, 10, String(altitude));
              display.setFont(DejaVu_Sans_Mono_12);
              display.drawString(85, 36, "meter");
              display.display();
              
              display.display();
    break;

    case 2  : display.clear();
              display.drawRect(0, 52, 128, 1);
              display.setTextAlignment(TEXT_ALIGN_LEFT);
              display.setFont(DejaVu_Sans_Mono_9);
              display.drawString(1, 0, "Pressure");
              display.setFont(DejaVu_Sans_Mono_10);
              display.drawString(1, 53, "3/4");
              display.setTextAlignment(TEXT_ALIGN_RIGHT);
              display.setFont(DejaVu_Sans_Mono_10);
              display.drawString(127, 53, String(tempHum));

              display.drawXbm(15, 11, 37, 37, pressure_image);
              display.setTextAlignment(TEXT_ALIGN_LEFT);
              display.setFont(ArialMT_Plain_24);
              display.drawString(58, 10, String(pressure));
              display.setFont(DejaVu_Sans_Mono_12);
              display.drawString(90, 36, "hPa");
              display.display();
    break;

    case 3  : read_heart();
              display.clear();
              display.drawRect(0, 52, 128, 1);
              display.setTextAlignment(TEXT_ALIGN_LEFT);
              display.setFont(DejaVu_Sans_Mono_9);
              display.drawString(1, 0, "Heart Rate");
              display.setFont(DejaVu_Sans_Mono_10);
              display.drawString(1, 53, "4/4");
              display.setTextAlignment(TEXT_ALIGN_RIGHT);
              display.setFont(DejaVu_Sans_Mono_10);
              display.drawString(127, 53, String(tempHum));

              if(irValue < 50000){
               beatsPerMinute = 0;
               display.setTextAlignment(TEXT_ALIGN_CENTER);
               display.setFont(DejaVu_Sans_Mono_12);
               display.drawString(64, 23, "Not Detection");
              }

              else{ 
               display.setTextAlignment(TEXT_ALIGN_LEFT);
               display.setFont(ArialMT_Plain_24);
               display.drawString(75, 10, String(beatsPerMinute));
               display.drawXbm(0, 0, 128, 64, heart_image);
              }  
              display.display();
    break;
  }

  if(digitalRead(butt4) == 1){
    while(digitalRead(butt4) == 1);
    char TimeSet[32];
    char minTempSett[32];
    char maxTempSett[32]; 
    settTime = EEPROM.readInt(addTime);
    setMinTemp = EEPROM.readInt(addMinTemp);
    setMaxTemp = EEPROM.readInt(addMaxTemp);
    setAltitude = EEPROM.readInt(addAltitude); 
    menuSetting = 0;
    slide = 0;
    
    while(1){
      snprintf(minTempSett, sizeof(minTempSett), "%d°C", setMinTemp);
      snprintf(maxTempSett, sizeof(maxTempSett), "%d°C", setMaxTemp);

      if(settTime > 0) snprintf(TimeSet, sizeof(TimeSet), "UTC+%d", settTime);
      if(settTime < 0) snprintf(TimeSet, sizeof(TimeSet), "UTC%d", settTime);
      altitude = bme.readAltitude(setAltitude);
      
      display.clear();
      display.drawRect(0, 14, 127, 1);
      display.drawRect(0, 53, 127, 1);     
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.setFont(DejaVu_Sans_Mono_10);
      display.drawString(64, 1, "Setting");
      switch(slide){
        case 0  : display.setTextAlignment(TEXT_ALIGN_LEFT);
                  display.setFont(DejaVu_Sans_Mono_11);
                  display.drawString(17, 15, "Time");
                  display.drawString(17, 26, "Min Temp");
                  display.drawString(17, 37, "Max Temp");
                  
                  display.setTextAlignment(TEXT_ALIGN_RIGHT);
                  display.setFont(DejaVu_Sans_Mono_11);
                  display.drawString(127, 15, String(TimeSet));
                  display.drawString(127, 26, String(minTempSett));
                  display.drawString(127, 37, String(maxTempSett));
        break;

        case 1  : display.setTextAlignment(TEXT_ALIGN_LEFT);
                  display.setFont(DejaVu_Sans_Mono_11);
                  display.drawString(17, 15, "Set Alt");
                  display.setTextAlignment(TEXT_ALIGN_RIGHT);
                  display.setFont(DejaVu_Sans_Mono_11);
                  display.drawString(127, 15, String(altitude));
        break;
      } 
      display.setTextAlignment(TEXT_ALIGN_LEFT);     
      display.setFont(DejaVu_Sans_Mono_8);
      display.drawString(10, 55, "Cancel");
      display.setTextAlignment(TEXT_ALIGN_RIGHT);
      display.drawString(118, 55, "Save");
      
      switch(menuSetting){
        case  0 : slide = 0;
                  display.setTextAlignment(TEXT_ALIGN_LEFT);
                  display.setFont(DejaVu_Sans_Mono_11);
                  display.drawString(2, 15, ">");
                  display.display();

                  if(digitalRead(butt3) == 1){
                    delay(150);
                    settTime++;
                    if(settTime == 0) settTime = 1;
                  }

                  if(digitalRead(butt4) == 1){
                    delay(150);
                    settTime--;
                    if(settTime == 0) settTime = -1;
                  }
        break;

        case  1 : slide = 0;
                  display.setTextAlignment(TEXT_ALIGN_LEFT);
                  display.setFont(DejaVu_Sans_Mono_11);
                  display.drawString(2, 26, ">");
                  display.display();

                  if(digitalRead(butt3) == 1){
                    delay(150);
                    setMinTemp++;
                  }

                  if(digitalRead(butt4) == 1){
                    delay(150);
                    setMinTemp--;
                  }
        break;

        case  2 : slide = 0;
                  display.setTextAlignment(TEXT_ALIGN_LEFT);
                  display.setFont(DejaVu_Sans_Mono_11);
                  display.drawString(2, 37, ">");
                  display.display();

                  if(digitalRead(butt3) == 1){
                    delay(150);
                    setMaxTemp++;
                  }

                  if(digitalRead(butt4) == 1){
                    delay(150);
                    setMaxTemp--;
                  }
        break;

        case  3 : slide = 1;
                  display.setTextAlignment(TEXT_ALIGN_LEFT);
                  display.setFont(DejaVu_Sans_Mono_11);
                  display.drawString(2, 15, ">");
                  display.display();

                  if(digitalRead(butt3) == 1){
                    delay(150);
                    setAltitude = setAltitude + 1;
                  }

                  if(digitalRead(butt4) == 1){
                    delay(150);
                    setAltitude = setAltitude - 1;
                  }
        break;

        case  4 : slide = 1;
                  display.setTextAlignment(TEXT_ALIGN_LEFT);
                  display.setFont(DejaVu_Sans_Mono_8);
                  display.drawString(43, 54, "<");
                  display.display();
                  
                  if(digitalRead(butt4) == 1){
                    while(digitalRead(butt4) == 1);
                    menu = 0;
                    goto c;
                  }
        break;

        case  5 : slide = 1;
                  display.setTextAlignment(TEXT_ALIGN_LEFT);
                  display.setFont(DejaVu_Sans_Mono_8);
                  display.drawString(90, 54, ">");
                  display.display();
                  
                  if(digitalRead(butt4) == 1){
                    while(digitalRead(butt4) == 1);
                    EEPROM.writeInt(addTime, settTime);
                    EEPROM.commit();
                    EEPROM.writeInt(addMinTemp, setMinTemp);
                    EEPROM.commit();
                    EEPROM.writeInt(addMaxTemp, setMaxTemp); 
                    EEPROM.commit(); 
                    EEPROM.writeInt(addAltitude, setAltitude); 
                    EEPROM.commit();
                    menu = 0;
                    slide = 0;
                    configTime((EEPROM.readInt(addTime) * 60 * 60) - 3600, daylightOffset_sec, ntpServer);
                    goto c;
                  }
        break;
      }

      if(digitalRead(butt2) == 1){
        while(digitalRead(butt2) == 1);
        menuSetting++;
        if(menuSetting > 5) menuSetting = 0;
      }

      if(digitalRead(butt1) == 1){
        while(digitalRead(butt1) == 1);
        menuSetting--;
        if(menuSetting < 0) menuSetting = 5;
      }
    }
  }
}

void setup_wifi(){
  a:
  Serial.println("Mulai Connecting");
  b:
  switch(kondisi){
    case  0 : strcpy(saveSSID,EEPROM.readString(addSSID).c_str());
              strcpy(savePASSWORD,EEPROM.readString(addPass).c_str());
              delay(100);
              WiFi.begin(saveSSID, savePASSWORD);
              while (WiFi.status() != WL_CONNECTED) { 
                progress = (counter) % 100;
                counter++;
                display.clear();
                display.setTextAlignment(TEXT_ALIGN_CENTER);
                display.setFont(ArialMT_Plain_10);
                display.drawString(64, 5, "Connecting To...");
                display.setFont(ArialMT_Plain_16);
                display.drawString(64, 17, saveSSID);
                display.drawProgressBar(15, 37, 96, 8, progress);
                display.setFont(ArialMT_Plain_10);
                display.drawString(64, 51, "BUTT4 to Setting");
                display.display();
                delay(10); 

                if(digitalRead(butt4) == 1){
                  while(digitalRead(butt4) == 1);
                  if(state3 == false){
                    kondisi = 1;
                    delay(100);
                    goto b;               
                  }  
                  state3 = false;    
                }              
              }

              randomSeed(micros());
              display.clear();
              display.setTextAlignment(TEXT_ALIGN_CENTER);
              display.setFont(ArialMT_Plain_10);
              display.drawString(64, 5, "Connecting To...");
              display.setFont(ArialMT_Plain_16);
              display.drawString(64, 17, saveSSID);
              display.drawString(64, 45, "Connected");
              display.display();
  
              Serial.println("WiFi connected");
              Serial.println("IP address: ");
              Serial.println(WiFi.localIP());
              counter = 0;
              delay(1000);
    break;

    case 1  : numSSID = 0;
              a = 0;
              delay(100);
              c:
              for(int i=0; i<3; i++){
                display.clear();
                display.setTextAlignment(TEXT_ALIGN_CENTER);
                display.setFont(ArialMT_Plain_16);
                display.drawString(64, 16, "Scan WiFi");
                display.setFont(ArialMT_Plain_10);
                display.drawString(64, 47, "Please wait..");
                display.display();
                  
                WiFi.disconnect(true);
                WiFi.mode(WIFI_STA);
                numSSID = WiFi.scanNetworks();
              } 
              Serial.print(numSSID);
              Serial.println("   Network found");
              snprintf(jumlah_network, sizeof(jumlah_network), "%d %s", numSSID, "Network Found");

              do{
                if(numSSID == 0){
                  display.clear();
                  display.setTextAlignment(TEXT_ALIGN_CENTER);
                  display.setFont(ArialMT_Plain_16);
                  display.drawString(64, 22, "NETWORK");
                  display.drawString(64, 42, "NOT FOUND");
                  display.display();
                  delay(1000);
                  goto c;
                }

                else{
                  display.clear();
                  display.setTextAlignment(TEXT_ALIGN_CENTER);
                  display.setFont(ArialMT_Plain_10);
                  display.drawString(64, 5, jumlah_network);
                  display.setFont(ArialMT_Plain_16);
                  display.drawString(64, 25, WiFi.SSID(a));
                  display.setFont(ArialMT_Plain_10);
                  display.drawString(64, 51, "BUTT4 to Continue");
                  display.display();
                  delay(10);

                  if(digitalRead(butt1) == 1){
                    while(digitalRead(butt1) == 1);
                    a++;
                    delay(100);
                    if(a >= numSSID) a = 0;
                  }

                  if(digitalRead(butt2) == 1){
                    while(digitalRead(butt2) == 1);
                    a--;
                    delay(100);
                    if(a < 0) a = numSSID - 1;
                  } 
                }
              }while(digitalRead(butt4) == 0);
              strcpy(ssid,WiFi.SSID(a).c_str());
              EEPROM.writeString(addSSID, ssid);
              EEPROM.commit();
              Serial.println(EEPROM.readString(addSSID));
              delay(300);
              kondisi = 2;
              goto b;
    break;

    case 2  : pathIndex = 0;
              c = 0;
              pilihanHuruf = 0;
              memset(bufferPass, 0, sizeof(bufferPass));
              while(1){
                 memset(password, 0, sizeof(password));
                 display.clear();
                 display.drawRect(0, 0, 128, 13);
                 display.setTextAlignment(TEXT_ALIGN_LEFT);
                 display.setFont(ArialMT_Plain_10);
                 for(int i=0; i < pathIndex; i++){
                   password[i] = bufferPass[i];
                 } 
                 display.drawString(2, 0, password);
                 switch(pilihanHuruf){
                    case 0  : x = 0;         
                              switch(slide){
                                case 0  : for(int i=0; i < 42; i++){    
                                          display.setTextAlignment(TEXT_ALIGN_CENTER);
                                          display.setFont(ArialMT_Plain_10);
                                          if(i == 0)      {x = 0; y = 15;}
                                          else if(i == 14){x = 0; y = 30;}
                                          else if(i == 28){x = 0; y = 45;}
                                          display.drawString(6+(x*9), y, String(hurufKecil[i]));
                                          x++;
                                        } 
                                break;

                                case 1  : for(int i=42; i < sizeof(hurufKecil); i++){    
                                          display.setTextAlignment(TEXT_ALIGN_CENTER);
                                          display.setFont(ArialMT_Plain_10);
                                          if(i == 42)     {x = 0; y = 15;}
                                          else if(i == 56){x = 0; y = 30;}
                                          display.drawString(6+(x*9), y, String(hurufKecil[i]));
                                          x++;
                                        }
                                break;
                              }

                              display.setTextAlignment(TEXT_ALIGN_CENTER);
                              display.setFont(ArialMT_Plain_10);
                              display.drawString(6+(d*9), e, "_");
                              display.display();

                              if(digitalRead(butt3) == 1){
                                waktu = 0;
                                while(digitalRead(butt3) == 1){
                                  waktu++;
                                  delay(1);
                                  if(waktu >= 1000) break;
                                }
                                if(waktu >= 1000){
                                  pilihanHuruf = 1;
                                }
                                else if(waktu <= 500 && state2 == false){
                                recData(hurufKecil[c]);
                                }
                                state1 = true;
                                state2 = false;
                              }  
                    break;

                    case 1  : x = 0;
                              switch(slide){
                                case 0  : for(int i=0; i < 42; i++){    
                                          display.setTextAlignment(TEXT_ALIGN_CENTER);
                                          display.setFont(ArialMT_Plain_10);
                                          if(i == 0)      {x = 0; y = 15;}
                                          else if(i == 14){x = 0; y = 30;}
                                          else if(i == 28){x = 0; y = 45;}
                                          display.drawString(6+(x*9), y, String(hurufBesar[i]));
                                          x++;
                                        } 
                                break;

                                case 1  : for(int i=42; i < sizeof(hurufBesar); i++){    
                                          display.setTextAlignment(TEXT_ALIGN_CENTER);
                                          display.setFont(ArialMT_Plain_10);
                                          if(i == 42)     {x = 0; y = 15;}
                                          else if(i == 56){x = 0; y = 30;}
                                          display.drawString(6+(x*9), y, String(hurufBesar[i]));
                                          x++;
                                        }
                                break;
                              }

                              display.setTextAlignment(TEXT_ALIGN_CENTER);
                              display.setFont(ArialMT_Plain_10);
                              display.drawString(6+(d*9), e, "_");
                              display.display();

                              if(digitalRead(butt3) == 1){
                                waktu = 0;
                                while(digitalRead(butt3) == 1){
                                  waktu++;
                                  delay(1);
                                  if(waktu >= 1000) break;
                                }
                                if(waktu >= 1000){
                                  pilihanHuruf = 0;
                                }
                                else if(waktu <= 500 && state1 == false){
                                  recData(hurufBesar[c]);
                                }
                                state1 = false;
                                state2 = true;
                              }
                    break;
                 }
                 
                 if(digitalRead(butt1) == 1){
                  delay(150);
                  c++;
                  d++;
                  if(c > 63) c = 0;
                 }

                 if(digitalRead(butt2) == 1){
                  delay(150);
                  c--;
                  d--;
                  if(c < 0) c = 63;
                 } 

                 switch(c){
                  case 0   : d = 0;  e = 16; slide = 0;
                  break;
                  case 13  : d = 13; e = 16; slide = 0;
                  break;
                  case 14  : d = 0;  e = 31; slide = 0;
                  break;
                  case 27  : d = 13; e = 31; slide = 0;
                  break;
                  case 28  : d = 0; e = 46; slide = 0;
                  break;
                  case 41  : d = 13; e = 46; slide = 0;
                  break;
                  case 42  : d = 0; e = 16; slide = 1;
                  break;
                  case 55  : d = 13; e = 16; slide = 1;
                  break;
                  case 56  : d = 0; e = 31; slide = 1;
                  break;
                  case 63  : d = 7; e = 31; slide = 1;
                  break;
                }

                if(digitalRead(butt4) == 1){
                  waktu = 0;
                  while(digitalRead(butt4) == 1){
                    waktu++;
                    delay(1);
                    if(waktu >= 1000) break;
                  }
                  if(waktu >= 1000){
                    EEPROM.writeString(addPass, password);
                    EEPROM.commit();
                    Serial.println(EEPROM.readString(addPass));
                    delay(300);
                    kondisi = 0;
                    state3 = true;
                    goto a;
                  }
                  else if(waktu <= 300){
                    pathIndex = pathIndex - 1;
                    if(pathIndex < 0) pathIndex = 0;
                  }
                }
              }
    break;
  }
}

void recData(char huruf){
  bufferPass[pathIndex] = huruf;
  pathIndex++;
}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived [");
  Serial.print(SUB_TOPIC_STRING);
  Serial.print("] ");
  
  for (int i = 0; i < length; i++) {
    msg[i] = (char)payload[i];
  }
  do_actions(msg);
}

void reconnect(){
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId;
    clientId += String(random(0xffff), HEX);

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 5, "Attempting MQTT");
    display.drawString(64, 20, "Connection...");
    display.display();
          
    if (client.connect((char*) clientId.c_str(), mqtt_user, mqtt_password)){
        Serial.println("connected");
        const char* SUB_TOPIC = SUB_TOPIC_STRING.c_str();
        client.subscribe(SUB_TOPIC);
      
        display.setFont(ArialMT_Plain_16);
        display.drawString(64, 45, "Connected");
        display.display();
        delay(1000);
    }
       
    else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      
      display.setFont(ArialMT_Plain_16);
      display.drawString(64, 45, "Failed");
      display.display();
      delay(500);
    }  
    menu = 0;
  }
}

void BMP280_read(){
  temp = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(EEPROM.readInt(addAltitude));
}

void read_heart(){
  irValue = particleSensor.getIR();

  if(checkForBeat(irValue) == true){
    long delta = millis() - lastBeat;
    lastBeat = millis();
    beatsPerMinute = 60 / (delta / 1000.0);

    if(beatsPerMinute < 255 && beatsPerMinute > 20){
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;
      beatAvg = 0;
      
      for (byte i = 0 ; i < RATE_SIZE ; i++)
        beatAvg += rates[i];
        
      beatAvg /= RATE_SIZE;
    }
  }
}


void sendData_iot(String notif) {
   StaticJsonBuffer<1000> jsonBuffer;
   JsonObject& post = jsonBuffer.createObject();
   post["eventName"] = "isinus";
   post["status"] = "none";
   post["stat"] = notif;
   post["temp"] = temp;
   post["hum"] = humidity;
   post["press"] = pressure;
   post["alt"] = altitude;
   post["heart"] = beatsPerMinute;
   post["mac"] = "0042814750669404";

   char JSONmessageBuffer[1000];
   post.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
   Serial.println(JSONmessageBuffer);
   client.publish(topic_pub,JSONmessageBuffer);
}

void do_actions(const char* miseji){
   StaticJsonBuffer<800> jsonBuffer;
   JsonObject& root = jsonBuffer.parseObject(miseji);

      if(strcmp(root["action"],"MinTemp") == 0){
        setMinTemp = (int)root["param"]["Temp"];
        Serial.println("Data Baru!");
        Serial.println(setMinTemp);
        EEPROM.writeInt(addMinTemp, setMinTemp);
        EEPROM.commit();
        delay(1000);
      }   

      if(strcmp(root["action"],"MaxTemp") == 0){
        setMaxTemp = (int)root["param"]["Temp"];
        Serial.println("Data Baru!");
        Serial.println(setMaxTemp);
        EEPROM.writeInt(addMaxTemp, setMaxTemp);
        EEPROM.commit();
        delay(1000);
      }  
  Serial.println(miseji);
}

void pilihan(int minimal, int maksimal){
  if(digitalRead(butt1) == 1){
    while(digitalRead(butt1) == 1);
     menu++;
  }

  if(digitalRead(butt2) == 1){
    while(digitalRead(butt2) == 1);
     menu--;
  }

  if(menu <= minimal) menu = minimal;
  if(menu >= maksimal) menu = maksimal;
}
