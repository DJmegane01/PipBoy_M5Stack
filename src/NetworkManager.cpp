#include "NetworkManager.h"

extern LcdUtil lcdUtil;

NetworkManager networkManager;

/*
 *
 */
void NetworkManager::init()
{
    // Wifi接続
    M5.Lcd.print("Connecting to Your SSID");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        M5.Lcd.print(".");
    }
    M5.Lcd.println("CONNECTED");
    M5.Lcd.clear();
}

void NetworkManager::connect()
{
    if (WiFi.isConnected())
    {
        int cnt = 0;
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED)
        {
        }
        {
            cnt++;
            delay(500);
            int x, y;
            std::tie(x, y) = lcdUtil.getLcdCenter();
            M5.Lcd.setCursor(x - 50, y);
            M5.Lcd.print(".");
        }
    }
}

/**
 * Wifiに接続済みの場合、切断し、Wifiをオフにする。
 */
void NetworkManager::disConnect()
{
    if (WiFi.isConnected())
    {
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
    }
}