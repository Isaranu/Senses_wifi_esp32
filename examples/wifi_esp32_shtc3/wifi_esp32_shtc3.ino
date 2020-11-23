/* ---------------------------------------------
  Use Temperatures & Humidity sensor : SHTC3
  Wiring connection
  [Weizen ESP32] -- [SHTC3]
    5V - Vcc
    Gnd - Gnd
    GPIO21 - SDA
    GPIO22 - SCL
------------------------------------------------ */

#include "SparkFun_SHTC3.h"
#include "Senses_wifi_esp32.h"

const char *ssid = "your-wifi-network-name";
const char *passw = "your-wifi-password";
const char *userid = "your-senses-user-id";
const char *key = "your-device-key";

String response;

SHTC3 mySHTC3;
Senses_wifi_esp32 myiot;

void setup(){

  Wire.begin();
  Serial.print("Beginning sensor. Result = ");
  errorDecoder(mySHTC3.begin());

  Serial.begin(9600);
  response = myiot.connect(ssid, passw, userid, key);
  Serial.println(response);

}

void loop(){
  
  SHTC3_Status_TypeDef result = mySHTC3.update();             
  printInfo();                                                
  delay(190);    

  response = myiot.send(1, mySHTC3.toDegC());
  Serial.println(response);
  
  response = myiot.send(2, mySHTC3.toPercent());
  Serial.println(response);
  
  delay(5000);
  
}

void printInfo(){
  if(mySHTC3.lastStatus == SHTC3_Status_Nominal){
    Serial.print("RH = "); 
    Serial.print(mySHTC3.toPercent());                        
    Serial.print("%, T = "); 
    Serial.print(mySHTC3.toDegC());                           
    Serial.println(" deg C"); 
  }else{
    Serial.print("Update failed, error: "); 
    errorDecoder(mySHTC3.lastStatus);
    Serial.println();
  }
}

void errorDecoder(SHTC3_Status_TypeDef message){
  switch(message){
    case SHTC3_Status_Nominal : Serial.print("Nominal"); break;
    case SHTC3_Status_Error : Serial.print("Error"); break;
    case SHTC3_Status_CRC_Fail : Serial.print("CRC Fail"); break;
    default : Serial.print("Unknown return code"); break;
  }
}
