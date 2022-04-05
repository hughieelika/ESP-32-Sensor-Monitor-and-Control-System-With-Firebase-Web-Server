// Load Wi-Fi library
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <FirebaseESP32.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define FIREBASE_HOST "https://esp32-webserver-bab81-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "4dd8P9OJCYPsSyJApCshsqOxEjff0XgwDt82rwDT"
#define WIFI_SSID "PCHR Reborn"
#define WIFI_PASSWORD "gaming123"
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
#define LedPin 5

Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK); // software SPI

// Set web server port number to 80
WiFiServer server(80);

// Variable switch
String LedStatus;

// Firebase data
FirebaseData firebaseData; //initate Firebase data

void setup(){
// Connect to Wi-Fi network with SSID and password
Serial.begin(115200);
pinMode(5, OUTPUT); 
Serial.println(F("BMP280 test"));

if (!bmp.begin(0x76)) {
Serial.println("Could not find a valid BMP280 sensor, check wiring!");
while (1);
}

WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}
// Print local IP address and start web server
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
Serial.println();

Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
Firebase.reconnectWiFi(true);
Firebase.setMaxRetry(firebaseData, 3); //optional
Firebase.setMaxErrorQueue(firebaseData, 30); //optional
}

void loop(){
  // Menampilkan suhu dan kelembaban pada serial monitor
  Serial.print("Temperature: ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
  Serial.print("Pressure: ");
  Serial.print(bmp.readPressure());
  Serial.println(" kPa");
  Serial.print("Altitude: ");
  Serial.print(bmp.readAltitude());
  Serial.println(" m");
  Serial.println();

  // Memberikan status suhu dan kelembaban kepada firebase
  if (Firebase.setFloat(firebaseData, "/Temperature", bmp.readTemperature())){
      Serial.println("Suhu terkirim");
    } else{
      Serial.println("Suhu tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    } 
    
  if (Firebase.setFloat(firebaseData, "/Pressure", bmp.readPressure())){
      Serial.println("Tekanan terkirim");
      Serial.println();
    } else{
      Serial.println("Tekanan tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    }

  if (Firebase.setFloat(firebaseData, "/Altitude", bmp.readAltitude())){
      Serial.println("Ketinggian terkirim");
    } else{
      Serial.println("Ketinggian tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    } 

if(Firebase.getString(firebaseData, "/LedStatus"))
  {
    String LedStatus = firebaseData.stringData();
    if(LedStatus.toInt() == 1){
      digitalWrite(LedPin, LOW);
      Serial.println("off");
    }
    else {
      digitalWrite(LedPin, HIGH);
      Serial.println("on");
    }
  }else{
    Serial.print("Error in getInt, ");
    Serial.println(firebaseData.errorReason());
  } 
delay(1000);
}