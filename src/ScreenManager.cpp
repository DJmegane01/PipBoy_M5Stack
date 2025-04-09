#include "ScreenManager.h"

int fileCount = 0;

extern HealthDisplay healthDisplay;
extern VersionDisplay versionDisplay;
extern SDUtil sdUtill;

ScreenManager screenManager;

void ScreenManager::init()
{
  M5.begin();
  M5.Power.begin();
  M5.Lcd.clear(BLACK);
  SPIFFS.begin();
  M5.Lcd.setBrightness(200);
  M5.Lcd.setTextColor(M5.Lcd.color565(0, 255, 0), M5.Lcd.color565(0, 0, 0));

  // SDカード読み込み初期化
  sdUtil.init();
  drawScreen(currentScreen);
}

void ScreenManager::update()
{
  if (M5.BtnA.wasPressed())
  {
    prevScreen();
  }
  if (M5.BtnC.wasPressed())
  {
    nextScreen();
  }
  if (currentScreen == 0)
  {
    // 健康画面表示
    healthDisplay.update();
  }
  else if (currentScreen == 1)
  {
    versionDisplay.update();
  }
  M5.update();
}

void ScreenManager::drawScreen(int screen)
{
  M5.Lcd.clear();
  if (screen == 0)
  {
    healthDisplay.init();
  }
  else if (screen == 1)
  {
    versionDisplay.init();
  }
}

void ScreenManager::nextScreen()
{
  currentScreen = (currentScreen + 1) % screenCnt;
  drawScreen(currentScreen);
}

void ScreenManager::prevScreen()
{
  currentScreen = (currentScreen - 1 + 2) % screenCnt;
  drawScreen(currentScreen);
}
