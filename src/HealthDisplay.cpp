#include "HealthDisplay.h"
#include "StepCounter.h"
#include "TimeDisplay.h"

extern StepCounter stepCounter;
extern TimeDisplay timeDisplay;
extern SDUtil sdUtil;
HealthDisplay healthDisplay;

void HealthDisplay::init()
{
    M5.IMU.Init();

    // PNGファイルの読み込みPIPBoyアイコン表示
    // M5.Lcd.drawPngFile(SD,"/VaultBoy.png",100,30,240,240);
    String fileName = "valuboy2.png";
    String fileNameStr = sdUtil.getFilePath(fileName);
    if (fileNameStr.indexOf(".png") != -1)
    {
        M5.Lcd.drawPngFile(SD, (const char *)&fileNameStr, 70, 25);
    }
    else
    {
        M5.Lcd.setCursor(130, 50);
        M5.Lcd.setTextSize(2);
        M5.Lcd.printf("%s", "PNG Not Found");
    }

    // バージョン表示
    M5.Lcd.setCursor((M5.Lcd.width() / 2) - 90, 10);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("%s", "PIP BOY ver:0.3"); // (文字列, x座標, y座標) ※フォント省略可

    timeDisplay.init();

    // 歩数計起動
    stepCounter.init();
    stepCounter.update();
}

void HealthDisplay::update()
{
    stepCounter.update();
    timeDisplay.update();
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(10, 70);
    M5.Lcd.printf("STEP:%04d", stepCounter.getStepCount());
}