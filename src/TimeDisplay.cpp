#include "TimeDisplay.h"

TimeDisplay timeDisplay;

TimeDisplay::TimeDisplay() : lastUpdateTime(0) {}

void TimeDisplay::init()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  lastUpdateTime = millis();
}

void TimeDisplay::update()
{
  unsigned long currentTime = millis();
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    M5.Lcd.println("Failed to obtain time");
    return;
  }
  if (currentTime - lastUpdateTime >= 1000)
  {
    lastUpdateTime = currentTime;
    displayTime(timeinfo.tm_hour,
                timeinfo.tm_min,
                timeinfo.tm_sec);
  }
}

void TimeDisplay::displayTime(int hour, int minute, int second)
{
  M5.Lcd.setTextSize(LcdTimeFontSize);
  M5.Lcd.setCursor(LcdTimeWidth, LcdTimeHeight);
  M5.Lcd.printf("%02d:%02d:%02d", hour, minute, second);
}
