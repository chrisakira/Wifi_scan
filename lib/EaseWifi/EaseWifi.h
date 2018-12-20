#pragma once
#include <ESP8266WiFi.h>

 void WifisetupST(const char* SoftApName, const char* WifiScan, const char* WifiPass)
 {

   WiFi.mode(WIFI_AP_STA);
   Serial.println(WiFi.softAP(SoftApName) ? "Ready" : "Failed!");
   IPAddress myIP = WiFi.softAPIP();
   Serial.print("HotSpt IP:");
   Serial.println(myIP);
   Serial.printf("MAC address = %s\n", WiFi.softAPmacAddress().c_str());
   WiFi.begin(WifiScan, WifiPass);

   Serial.print("Connecting");
   while (WiFi.status() != WL_CONNECTED)
   {
     delay(500);
     Serial.print(".");
   }
   Serial.println();
   Serial.print("Connected, IP address: ");
   Serial.println(WiFi.localIP());
   Serial.printf("Connected, mac address: %s\n", WiFi.macAddress().c_str());
   Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
   WiFi.setAutoReconnect(true);
   WiFi.setAutoConnect(true);
}
void WifisetupS(const char* SoftApName)
{

   WiFi.mode(WIFI_AP);
   Serial.println(WiFi.softAP(SoftApName) ? "Ready" : "Failed!");
   IPAddress myIP = WiFi.softAPIP();
   Serial.print("HotSpt IP:");
   Serial.println(myIP);
   Serial.printf("MAC address = %s\n", WiFi.softAPmacAddress().c_str());
}
void WifisetupT(const char* WifiScan, const char* WifiPass)
 {

   WiFi.mode(WIFI_STA);
   WiFi.begin(WifiScan, WifiPass);
   Serial.print("Connecting");
   while (WiFi.status() != WL_CONNECTED)
   {
     delay(500);
     Serial.print(".");
   }
   Serial.println();
   Serial.print("Connected, IP address: ");
   Serial.println(WiFi.localIP());
   Serial.printf("Connected, mac address: %s\n", WiFi.macAddress().c_str());
   Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
   WiFi.setAutoReconnect(true);
   WiFi.setAutoConnect(true);
}
