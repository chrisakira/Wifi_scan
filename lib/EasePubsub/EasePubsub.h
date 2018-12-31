#pragma once
#include "EaseWifi.h"
#include <PubSubClient.h>


WiFiClient espClient;
PubSubClient client(espClient);

void mqtt_callback(const char* topic, byte* payload, unsigned int length);

void PubSubSetup(const char* mqttServer,const int mqttPort,const char* mqttUser,const char* mqttPassword)
{
  client.setServer(mqttServer, mqttPort);
  client.setCallback(mqtt_callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void PubSubSetup(const char* mqttServer,const int mqttPort)
{
  client.setServer(mqttServer, mqttPort);
  client.setCallback(mqtt_callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void PubSubReconnect(const char* mqttUser,const char* mqttPassword)
{
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
void mqtt_callback(const char* topic, byte* payload, unsigned int length) {
  unsigned int i = 0;
  String msg;
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for( i = 0; i < length; i++)
  {
     char c = (char)payload[i];
     msg += c;
  }
  Serial.println(msg);
  Serial.println();
  Serial.println("-----------------------");
  client.publish("CONF","1");
  
}
