/*
  Use with "Senses Snack Basic board" !
  - What is Senses Snack Basic board -
  A ready shield board compatible use for Senses's WEIZEN board.
  It's built-in 128x64 OLED screen and DHT Digtial Temperature & Humidity sensor.
  Please Enjoy !!
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>

#include "Senses_wifi_esp32.h"
#include "DHT.h"

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "your-wifi-network-name";
const char *passw = "your-wifi-password";
const char *userid = "your-senses-user-id";
const char *key = "your-device-key";

String response;
float t,h;

Senses_wifi_esp32 myiot;

void setup(){
  
  Serial.begin(9600);
  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();
  
  display.setTextColor(WHITE);

  display.setTextSize(2);
  display.setCursor(32,10);
  display.println("SENSES");

  display.setTextSize(1);
  display.setCursor(32,26);
  display.println("IoT platform");

  display.setCursor(12,40);
  display.println("with Senses SNACK !");

  display.setCursor(32,54);
  display.println("starting...");
  
  display.display();

  response = myiot.connect(ssid, passw, userid, key);
  Serial.println(response);
  
  delay(2000);
  display.clearDisplay();
  
}

void loop() {
  
  response = "";

  display.setTextSize(1);
  display.setCursor(24,50);
  display.print("DHT reading..");
  display.display();
  delay(1000);
  display.setTextColor(BLACK);
  display.setCursor(24,50);
  display.print("DHT reading..");
  display.display();

  display.setTextColor(WHITE);

  /* - DHT sensor reading - */
  t = dht.readTemperature();
  h = dht.readHumidity();

  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  delay(1000);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(5,10);
  display.print("T " + String(t));
  
  display.setTextSize(1);
  display.print(" C");

  display.setTextSize(2);
  display.setCursor(5,30);
  display.print("H " + String(h));

  display.setTextSize(1);
  display.print(" % RH");
  
  display.display();

  Serial.println("Temperature is " + String(t) + " celcuis");
  Serial.println("Humidity is " + String(h) + " %RH");
  Serial.println("----------------------------------------");
  
  Serial.println("Sending data to SENSES IoT platform..");
  
  display.setTextSize(1);
  display.setCursor(0,50);
  display.print("Updating Dashboard..");
  display.display();
  
  response = myiot.send(1,t);
  Serial.println(response);

  response = myiot.send(2,h);
  Serial.println(response);

  delay(5000);  

  display.setTextColor(BLACK);
  display.setCursor(0,50);
  display.print("Updating Dashboard..");
  display.display();

  display.setTextColor(WHITE);
}
