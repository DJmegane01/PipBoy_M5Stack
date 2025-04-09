#ifndef HEALTHDISPLAY_H
#define HEALTHDISPLAY_H
#include <M5Stack.h>
#include "SDUtil.h"

class HealthDisplay
{
public:
    void init();
    void update();

private:
    float accX, accY, accZ;
    float bAccX, bAccY, bAccZ;
    int walkCnt = 0;
};

extern HealthDisplay healthDisplay;
#endif // HEALTHDISPLAY_H
