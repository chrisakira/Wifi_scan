#include <ESP8266WiFi.h>
#include "structures.h"

#define MAX_APS_TRACKED 100
#define MAX_CLIENTS_TRACKED 200

beaconinfo aps_known[MAX_APS_TRACKED];                    // Array to save MACs of known APs
int aps_known_count = 0;                                  // Number of known APs
int nothing_new = 0;
clientinfo clients_known[MAX_CLIENTS_TRACKED];            // Array to save MACs of known CLIENTs
int clients_known_count = 0;                              // Number of known CLIENTs




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
          print_client(ci);
          nothing_new = 0;
        }
    }
  } else {
    struct sniffer_buf *sniffer = (struct sniffer_buf*) buf;

    if ((sniffer->buf[0] == 0x08) || (sniffer->buf[0] == 0x88)) {
      struct clientinfo ci = parse_data(sniffer->buf, 36, sniffer->rx_ctrl.rssi, sniffer->rx_ctrl.channel);
      if (memcmp(ci.bssid, ci.station, ETH_MAC_LEN)) {
        if (register_client(ci) == 0) {
          print_client(ci);
          nothing_new = 0;
        }
      }
    }
  }
}





