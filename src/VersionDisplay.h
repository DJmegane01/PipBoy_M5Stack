#ifndef VERSIONDISPLAY_H
#define VERSIONDISPLAY_H
#include <M5Stack.h>
#include "StepCounter.h"
#include "TimeDisplay.h"
#include "SDUtil.h"

class VersionDisplay
{
public:
    void init();
    void update();

private:
    const int maxFile = 100;
    int fileCount = 0;
    float accX, accY, accZ;
    float bAccX, bAccY, bAccZ;
    int walkCnt = 0;
};

extern VersionDisplay versionDisplay;
#endif // VERSIONDISPLAY_H
