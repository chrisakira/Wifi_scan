#include "functions.h" // funções para escanear a NET
#include "EasePubsub.h"// para facilitar a vida no pubsub
#include "EaseWifi.h"//facilita a vida no Wifi

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  Scan();// escanea os dispositivos próximos e salva no clients_known
  WifisetupT("WICK_MASTER_RACE","GETWICKED"); // Usuario e Senha do Wifi
  PubSubSetup("m15.cloudmqtt.com",15678,"zsquadnd","G450CJIQuxQq"); // Endereço , porta, usuario e senha do MQTT
  for(int i = 0 ; i < clients_known_count ; i++)
  {
    String ST = formatMac1(clients_known[i].station);
    int n = ST.length();   
    char MACchar[n+1];  
    strcpy(MACchar, ST.c_str()); 
    client.publish("Celular",MACchar);// publica os dispositivos encontrados
  }
  WiFi.mode(WIFI_OFF);
  apagarDevices(); // apaga os clientes salvos no clients_known
}