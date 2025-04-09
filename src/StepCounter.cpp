
#include "StepCounter.h"
StepCounter stepCounter(1.8);

StepCounter::StepCounter(float threshold) : stepCount(0), isStep(false), threshold(threshold) {}

void StepCounter::init()
{
  M5.IMU.Init();
}

void StepCounter::update()
{
  float accX = 0.0, accY = 0.0, accZ = 0.0;
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  float totalAcc = sqrt(accX * accX + accY * accY + accZ * accZ);
  if (totalAcc > threshold && !isStep)
  {
    stepCount++;
    isStep = true;
  }
  else if (totalAcc < threshold && isStep)
  {
    isStep = false;
  }
}

int StepCounter::getStepCount() const
{
  return stepCount;
}

void StepCounter::displayStepCnt()
{
  M5.Lcd.setTextSize(LcdFontSize);
  M5.Lcd.setCursor(LcdWidth, LcdHeight);
  M5.Lcd.printf("%05d", stepCount);
}
