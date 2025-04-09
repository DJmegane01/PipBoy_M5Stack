#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <M5Stack.h>
#include "HealthDisplay.h"
#include "VersionDisplay.h"
#include "SDUtil.h"

class ScreenManager
{
public:
  void init();
  void update();
  void drawScreen(int screen);
  void nextScreen();
  void prevScreen();

private:
  int currentScreen = 0;
  const int screenCnt = 2;
  static const int maxFile = 100;
  String fileList[maxFile];
};

extern ScreenManager screenManager;

#endif // SCREENMANAGER_H
