/*
  Use with SENSES WEIZENS board vs ThaiEasyElec UC20 board (Pi Hat style)
  For Library UC20.h, please download at : https://github.com/Isaranu/uc20
*/

#include "UC20_HTTP.h"

#define TX_PIN  32
#define RX_PIN  34
#define ucSerial Serial1

String userid = "{your-account-id}";
String key = "{your-device-key}";
String slot_number = "{your-slot-number}";
int data=0;

UC20_HTTP http;

void setup() {
  Serial.begin(9600);
  ucSerial.begin(115200,SERIAL_8N1,RX_PIN,TX_PIN);
  gsm.initSerial(ucSerial);

  Serial.println("############ UC20 HTTP ############");
  Serial.print("Power on module...");
  while(!gsm.powerOn(25)) 
    Serial.print(".");
  Serial.println(" READY");
  Serial.print  ("Network registration...");
  Serial.println(gsm.network_regis_state()? " READY":" NOT READY");
  Serial.println("IMEI       : " + gsm.get_IMEI());
  Serial.println("IMSI       : " + gsm.get_IMSI());
  Serial.println("ICCID      : " + gsm.get_ICCID());
  Serial.println("Operator   : " + gsm.get_operator());
  Serial.print  ("RSSI       : ");
  Serial.println(gsm.get_signal_quality());
  if(!gsm.connected()) 
    gsm.connect_internet();
  Serial.println("IP Address : " + gsm.getIPaddress());
  Serial.println();
  
}

void loop() {
  
  data = random(20, 100);
  String HOST = "http://www.sensesiot.com:4000/send/" + userid + "/" + key + "/" + slot_number + "/" + String(data);
  
  http.setInit();
  if(!http.setUrl(HOST)) {
    Serial.println("HOST name is invalid");
    return;
  }
  http.get();
  Serial.print("- GET ");
  Serial.println(HOST);
  http.readResp(true);
  Serial.println();

  delay(5000);

}
