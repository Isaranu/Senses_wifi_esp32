/*
  Use with SENSES WEIZENS board vs ThaiEasyElec UC20 board (Pi Hat style)
  And Dust sensor : PMS3003 Plantower http://www.plantower.com/en/content/?107.html
  
  For Library UC20.h, please download at : https://github.com/Isaranu/uc20
*/

#include "UC20_HTTP.h"

#define TX_PIN  32
#define RX_PIN  34
#define ucSerial Serial1

#define TX_PIN2  26
#define RX_PIN2  13
#define PMSserial Serial2

String userid = "{your-account-id}";
String key = "{your-device-key}";
String slot_number = "{your-slot-number}";
int data=0;

UC20_HTTP http;

//---------pm----------------//
unsigned int pm1 = 0;
unsigned int pm2_5 = 0;
unsigned int pm10 = 0;
//---------pm----------------//

void setup() {
  Serial.begin(9600);
  
  ucSerial.begin(115200,SERIAL_8N1,RX_PIN,TX_PIN);
  PMSserial.begin(9600 ,SERIAL_8N1,RX_PIN2,TX_PIN2);
  gsm.initSerial(ucSerial);

  Serial.println("############ UC20 HTTP ############");
  Serial.print("Power on module...");
  while(!gsm.powerOn(25)) // pin 25
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
  
   //----------------------PM--------------//
  int index = 0;
  char value;
  char previousValue;

  while (PMSserial.available()) {
 
    value = PMSserial.read();
    if ((index == 0 && value != 0x42) || (index == 1 && value != 0x4d)) {
      Serial.println("Cannot find the data header.");
      break;
    }

    if (index == 4 || index == 6 || index == 8 || index == 10 || index == 12 || index == 14) {
      previousValue = value;
    }
    else if (index == 5) {
      pm1 = 256 * previousValue + value;

    }
    else if (index == 7) {
      pm2_5 = 256 * previousValue + value;
     

    }
    else if (index == 9) {
      pm10 = 256 * previousValue + value;

    } else if (index > 15) {
      break;
    }
    index++;
  }

  while (PMSserial.available()) PMSserial.read();

 //-----------end-----------PM--------------//
  data = pm2_5;
  Serial.println(pm2_5);
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
