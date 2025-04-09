#ifndef TIMEDISPLAY_H
#define TIMEDISPLAY_H

#include <M5Stack.h>

class TimeDisplay
{
public:
  TimeDisplay();
  void init();
  void update();
  void displayTime(int hour, int minute, int second);

private:
  unsigned long lastUpdateTime;
  const int LcdTimeHeight = 210;
  const int LcdTimeWidth = 70;
  const int LcdTimeFontSize = 4;
  // SDカードのピン設定
  const int chipSelect = 4;

  const char *ssid = "Buffalo-2G-AC40";
  const char *password = "csak3uie3sc4e";
  const char *ntpServer = "ntp.nict.jp";
  const long gmtOffset_sec = 3600 * 9;
  const int daylightOffset_sec = 0;
};

extern TimeDisplay timeDisplay;

#endif // TIMEDISPLAY_H
