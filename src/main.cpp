#define M5STACK_MPU6886
#include <Arduino.h>
#include <M5Stack.h>
#include <SPI.h>
#include "time.h"
#include "StepCounter.h"
#include "TimeDisplay.h"
#include "HealthDisplay.h"
#include "NetworkManager.h"
#include "ScreenManager.h"

const int LcdTimeHeight = 210;
const int LcdTimeWidth = 70;
const int LcdTimeFontSize = 4;

static File f;

float accX, accY, accZ;
float bAccX, bAccY, bAccZ;
int walkCnt = 0;

extern NetworkManager networkManager;
extern ScreenManager screenManager;

// スリープ実行関数
void enable_deep_sleep(int time)
{
    // 画面を消す（スリープに入るだけでは画面は点灯したままになるため）
    M5.Lcd.setBrightness(0); // バックライトの明るさを0（消灯）～255（点灯）で制御
    // スリープ復帰時間を設定
    esp_sleep_enable_timer_wakeup(SLEEP_MIN(time));
    // スリープ開始
    esp_deep_sleep_start();
}

void setup()
{
    M5.begin();
    M5.Power.begin();
    M5.Lcd.clear(BLACK);
    SPIFFS.begin();
    M5.Lcd.setBrightness(200);
    M5.Lcd.setTextColor(M5.Lcd.color565(0, 255, 0), M5.Lcd.color565(0, 0, 0));

    // Wifi接続
    networkManager.init();
    screenManager.init();
}

void loop()
{
    M5.update();
    /*if (M5.BtnA.wasPressed())
    {
        // スリープ開始（1分設定）
        enable_deep_sleep(1);
    }*/

    screenManager.update();
    delay(10);
}