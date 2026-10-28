#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <M5Stack.h>
#include "LcdUtil.h"
#include <WiFi.h>

class NetworkManager
{
public:
  void init();
  void connect();
  void disConnect();

private:
  const char *ssid = "SSID";
  const char *password = "PASSWORD";
};

#endif // NETWORKMANAGER_H
