#include "EaseWifi.h"//facilita a vida no Wifi
#include "EasePubsub.h"// para facilitar a vida no pubsub
#include "functions.h" // funções para escanear a NET
const char* Ender = "104.196.45.155";
const int Porta = 1883;
const char* Usuario = "Akira";
const char* Senha = "Akira";
const char* MeuCelular = "00:0a:f5:be:5b:18";
void setup()
{
  Serial.begin(115200);  
}

void loop()
{
  Scan();// escanea os dispositivos próximos e salva no clients_known
  delay(1);
  char MACchar[sizeof(clients_known[0])];
  WifisetupT("WICK_MASTER_RACE","GETWICKED"); // Usuario e senha 
  PubSubSetup("m15.cloudmqtt.com",15678,"zsquadnd","G450CJIQuxQq"); // Endereço , porta, usuario e senha do MQTT
  client.publish("WifiDisp","BEGIN");
  for(int i = 0 ; i < clients_known_count ; i++)
  {
    delay(1);
    String ST = formatMac1(clients_known[i].station); 
    strcpy(MACchar, ST.c_str()); 
    WifisetupT("WICK_MASTER_RACE","GETWICKED"); // Usuario e senha 
    PubSubSetup("m15.cloudmqtt.com",15678,"zsquadnd","G450CJIQuxQq"); // Endereço , porta, usuario e senha do MQTT
    if(ST == "00:0a:f5:be:5b:18") client.publish("WifiDispC","Celular Akira");
    else
    { 
      client.publish("WifiDispD",MACchar);
    }
    
  }
  if(clients_known_count == 0 ) client.publish("test", "nada encontrado");
  delay(1);
  WifisetupT("WICK_MASTER_RACE","GETWICKED"); // Usuario e senha 
  PubSubSetup("m15.cloudmqtt.com",15678,"zsquadnd","G450CJIQuxQq"); // Endereço , porta, usuario e senha do MQTT
  client.publish("WifiDisp","END");
  apagarDevices(); // apaga os clientes salvos no clients_known
  
}