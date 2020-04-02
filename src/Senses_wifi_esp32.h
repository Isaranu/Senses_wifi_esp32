#ifndef Senses_wifi_esp32_h
#define Senses_wifi_esp32_h

#include "Arduino.h"
#include "HTTPClient.h"
#include "WiFi.h"

class Senses_wifi_esp32{

  public:
    void setPowerOff();
    void setPowerOn();
    String connect(const char *ssid, const char *passw, const char *userid, const char *key);
    String send(int slotnum, float data);
    String getDigitalControl(int controlport);
    String MACtoString(uint8_t* macAddress);
    const char *_ssid, *_passw;
    const char *_userid, *_key;
    String _controlresponse;

  private:
    int _slotnum;
    float _data;
    String _path;
    String _response;
    String _wfMcaddr;
    int _controlport;
};

#endif
