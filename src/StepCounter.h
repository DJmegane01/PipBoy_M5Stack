#ifndef STEPCOUNTER_H
#define STEPCOUNTER_H
#define M5STACK_MPU6886
#include <M5Stack.h>

class StepCounter
{
public:
  StepCounter(float threshold);
  void init();
  void update();
  int getStepCount() const;
  void displayStepCnt();

private:
  int stepCount;
  bool isStep;
  float threshold;
  const int LcdFontSize = 1;
  const int LcdWidth = 250;
  const int LcdHeight = 10;
};

extern StepCounter stepCounter;

#endif // STEPCOUNTER_H
