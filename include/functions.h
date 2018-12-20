#include <ESP8266WiFi.h>
#include "structures.h"
#include <set>
#include <string>


#define MAX_APS_TRACKED 100
#define MAX_CLIENTS_TRACKED 200
#define MINRSSI -40
#define disable 0
#define enable 1
#define SENDTIME 30000
#define MAXDEVICES 10
#define PURGETIME 600000


unsigned int channel = 1;
int clients_known_count_old, aps_known_count_old;
int nbrDevices = 0;
int usedChannels[15];
beaconinfo aps_known[MAX_APS_TRACKED];                    // Array to save MACs of known APs
int aps_known_count = 0;                                  // Number of known APs
int nothing_new = 0;
clientinfo clients_known[MAX_CLIENTS_TRACKED];            // Array to save MACs of known CLIENTs
int clients_known_count = 0;   
String device[MAXDEVICES];


String formatMac1(uint8_t mac[ETH_MAC_LEN]) {
  String hi = "";
  for (int i = 0; i < ETH_MAC_LEN; i++) {
    if (mac[i] < 16) hi = hi + "0" + String(mac[i], HEX);
    else hi = hi + String(mac[i], HEX);
    if (i < 5) hi = hi + ":";
  }
  return hi;
}

int register_client(clientinfo &ci) {
  int known = 0;
  if(ci.rssi > MINRSSI)
  {
  for (int u = 0; u < clients_known_count; u++)
  {
    if (! memcmp(clients_known[u].station, ci.station, ETH_MAC_LEN)) {
      clients_known[u].lastDiscoveredTime = millis();
      clients_known[u].rssi = ci.rssi;
      known = 1;
      break;
    }
  }
  if (! known) {
    ci.lastDiscoveredTime = millis();
    for (int u = 0; u < aps_known_count; u++) {
      if (! memcmp(aps_known[u].bssid, ci.bssid, ETH_MAC_LEN)) {
        ci.channel = aps_known[u].channel;
        break;
      }
    }
    if (ci.channel != 0) {
      memcpy(&clients_known[clients_known_count], &ci, sizeof(ci));
      clients_known_count++;
    }

    if ((unsigned int) clients_known_count >=
        sizeof (clients_known) / sizeof (clients_known[0]) ) {
      clients_known_count = 0;
    }
  }
  return known;
  }
  return 0;
}

String print_client(clientinfo ci)
{
  String hi = "";
  if (ci.err != 0) {
    // nothing
  } else {
    Serial.printf("CLIENT: ");
    Serial.print(formatMac1(ci.station));
    if(formatMac1(ci.station) == "80:ad:16:e5:86:05") Serial.print("meu Celular");  
    Serial.printf("   % 4d\r\n", ci.rssi);
//    }
  }
  return hi;
}

void promisc_cb(uint8_t *buf, uint16_t len)
{
  if (len == 12) {
  } else if (len == 128) {
    struct sniffer_buf2 *sniffer = (struct sniffer_buf2*) buf;
    if ((sniffer->buf[0] == 0x40)) {
      struct clientinfo ci = parse_probe(sniffer->buf, 36, sniffer->rx_ctrl.rssi);
        if (register_client(ci) == 0) {
          //print_client(ci);
          nothing_new = 0;
        }
    }
  } else {
    struct sniffer_buf *sniffer = (struct sniffer_buf*) buf;

    if ((sniffer->buf[0] == 0x08) || (sniffer->buf[0] == 0x88)) {
      struct clientinfo ci = parse_data(sniffer->buf, 36, sniffer->rx_ctrl.rssi, sniffer->rx_ctrl.channel);
      if (memcmp(ci.bssid, ci.station, ETH_MAC_LEN)) {
        if (register_client(ci) == 0) {
          //print_client(ci);
          nothing_new = 0;
        }
      }
    }
  }
}

void showDevices()
{
  Serial.println("");
  Serial.println("");
  Serial.println("-------------------Device DB-------------------");
  Serial.printf("%4d Devices + Clients.\n", aps_known_count + clients_known_count); // show count

  // show Clients
  for (int u = 0; u < clients_known_count; u++)
  {
    Serial.printf("%4d ", u); // Show client number
    Serial.print("C ");
    Serial.print(formatMac1(clients_known[u].station));
    Serial.print(" RSSI ");
    Serial.print(clients_known[u].rssi);
    Serial.print(" channel ");
    Serial.println(clients_known[u].channel);
  }
}

void Scan()
{
  wifi_set_opmode(STATION_MODE); // Promiscuous works only with station mode
  wifi_set_channel(channel);
  wifi_promiscuous_enable(disable);
  wifi_set_promiscuous_rx_cb(promisc_cb); // Set up promiscuous callback
  wifi_promiscuous_enable(enable);
  for (int i = 0; i < 10; i++)
  {
    channel = 1;
    wifi_set_channel(channel);
    while (true)
    {
      nothing_new++;
      if (nothing_new > 100)
      {
        nothing_new = 0;
        channel++;
        if (channel == 15)
          break;
        wifi_set_channel(channel);
      }
      delay(1);
    }
  }
  wifi_promiscuous_enable(disable);
  showDevices();
  yield();
}

void apagarDevices()
{
  for (int i = 0; i < clients_known_count; i++)
  {
    for (int u = 0; u < ETH_MAC_LEN; u++)
    {
      clients_known[i].station[u] = 0;
      clients_known[i].ap[u] = 0;
      clients_known[i].bssid[u] = 0;
    }
    clients_known[i].channel = 0;
    clients_known[i].err = 0;
    clients_known[i].lastDiscoveredTime = 0;
    clients_known[i].rssi = 0;
    clients_known[i].seq_n = 0;
  }
  clients_known_count = 0;
}






