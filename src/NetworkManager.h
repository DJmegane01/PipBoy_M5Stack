#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <M5Stack.h>
#include "LcdUtil.h"
#include <WiFi.h>

class NetworkManager {
public:
  void init();
  void connect();
  void disConnect();

private:
  const char* ssid       = "Buffalo-2G-AC40";
  const char* password   = "csak3uie3sc4e";
};

#endif // NETWORKMANAGER_H
