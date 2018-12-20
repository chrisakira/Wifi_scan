#include "functions.h"
#include "EasePubsub.h"
#include "EasePubsub.h"

void setup()
{
  Serial.begin(115200);
  WifisetupT("WICK_MASTER_RACE","GETWICKED");
  PubSubSetup("m15.cloudmqtt.com",15678,"zsquadnd","G450CJIQuxQq");
  client.publish("Teste","Conectado");
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  Scan();
  WifisetupT("WICK_MASTER_RACE","GETWICKED");
  PubSubSetup("m15.cloudmqtt.com",15678,"zsquadnd","G450CJIQuxQq");
  for(int i = 0 ; i < clients_known_count ; i++)
  {
    String ST = formatMac1(clients_known[i].station);
    int n = ST.length();   
    char MACchar[n+1];  
    strcpy(MACchar, ST.c_str()); 
    client.publish("Celular",MACchar);
  }
  WiFi.mode(WIFI_OFF);
  apagarDevices();
}