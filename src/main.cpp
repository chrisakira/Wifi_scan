
#include <set>
#include <string>
#include "functions.h"

#define disable 0
#define enable  1
#define SENDTIME 30000
#define MAXDEVICES 60
#define PURGETIME 600000
#define MINRSSI -100

unsigned int channel = 1;
int clients_known_count_old, aps_known_count_old;

String device[MAXDEVICES];
int nbrDevices = 0;
int usedChannels[15];

#ifndef CREDENTIALS
#define mySSID "WICK_MASTER_RACE"
#define myPASSWORD "GETWICKED"
#endif



void setup() {
  Serial.begin(115200);

  wifi_set_opmode(STATION_MODE);            // Promiscuous works only with station mode
  wifi_set_channel(channel);
  wifi_promiscuous_enable(disable);
  wifi_set_promiscuous_rx_cb(promisc_cb);   // Set up promiscuous callback
  wifi_promiscuous_enable(enable);
}


void purgedevice();
void showdevice();

void loop() {
  channel = 1;
  wifi_set_channel(channel);
  while (true) {
    nothing_new++;                          // Array is not finite, check bounds and adjust if required
    if (nothing_new > 200) {                // monitor channel for 200 ms
      nothing_new = 0;
      channel++;
      if (channel == 15) break;             // Only scan channels 1 to 14
      wifi_set_channel(channel);
    }
    delay(1);  // critical processing timeslice for NONOS SDK! No delay(0) yield()

    
  }
  void purgeDevice();
  void showDevices();
}

void purgeDevice() {
  for (int u = 0; u < clients_known_count; u++) {
    if ((millis() - clients_known[u].lastDiscoveredTime) > PURGETIME) {
      Serial.print("purge Client" );
      Serial.println(u);
      for (int i = u; i < clients_known_count; i++) memcpy(&clients_known[i], &clients_known[i + 1], sizeof(clients_known[i]));
      clients_known_count--;
      break;
    }
  }
  for (int u = 0; u < aps_known_count; u++) {
    if ((millis() - aps_known[u].lastDiscoveredTime) > PURGETIME) {
      Serial.print("purge Bacon" );
      Serial.println(u);
      for (int i = u; i < aps_known_count; i++) memcpy(&aps_known[i], &aps_known[i + 1], sizeof(aps_known[i]));
      aps_known_count--;
      break;
    }
  }
}


void showDevices() {
  Serial.println("");
  Serial.println("");
  Serial.println("-------------------Device DB-------------------");
  Serial.printf("%4d Devices + Clients.\n",aps_known_count + clients_known_count); // show count

  // show Beacons
  for (int u = 0; u < aps_known_count; u++) {
    Serial.printf( "%4d ",u); // Show beacon number
    Serial.print("B ");
    Serial.print(formatMac1(aps_known[u].bssid));
    Serial.print(" RSSI ");
    Serial.print(aps_known[u].rssi);
    Serial.print(" channel ");
    Serial.println(aps_known[u].channel);
  }

  // show Clients
  for (int u = 0; u < clients_known_count; u++) {
    Serial.printf("%4d ",u); // Show client number
    Serial.print("C ");
    Serial.print(formatMac1(clients_known[u].station));
    Serial.print(" RSSI ");
    Serial.print(clients_known[u].rssi);
    Serial.print(" channel ");
    Serial.println(clients_known[u].channel);
  }
}
