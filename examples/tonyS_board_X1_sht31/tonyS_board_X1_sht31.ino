#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

#include "Senses_wifi_esp32.h"

const char *ssid = "your-wifi-network-name";
const char *passw = "your-wifi-password";
const char *userid = "your-user-id";
const char *key = "your-device-key";

String response;

Adafruit_SHT31 sht31 = Adafruit_SHT31();
Senses_wifi_esp32 myiot;

void setup(){
  Serial.begin(9600);

  /* Pin 2 is VCC enabled for TonyS X1*/
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  
  response = myiot.connect(ssid, passw, userid, key);
  Serial.println(response);

  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
  
}

void loop(){

  response = "";

  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  response = myiot.send(1,t);
  response = myiot.send(2,h);
  
  Serial.println(response);

  delay(5000);
}
