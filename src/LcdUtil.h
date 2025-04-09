#ifndef LCDUTIL_H
#define LCDUTIL_H

#include <M5Stack.h>

class LcdUtil
{
public:
  std::tuple<int, int> getLcdCenter();
  int getLcdHalfWidth();
  int getLcdHalfHeight();

private:
  const int lcdWidth = M5.Lcd.width();
  const int lcdHeight = M5.Lcd.height();
};

extern LcdUtil lcdUtil;

#endif // LCDUTIL_H
