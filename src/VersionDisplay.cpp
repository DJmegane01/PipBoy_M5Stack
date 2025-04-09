#include <iterator>  // std::begin, std::end
#include <algorithm> // std::find
#include "VersionDisplay.h"
#include "LcdUtil.h"
#include "SDUtil.h"

extern LcdUtil lcdUtil;
extern SDUtil sdUtil;

VersionDisplay versionDisplay;

void VersionDisplay::init()
{
    // PNGファイルの読み込みPIPBoyロゴ表示
    // M5.Lcd.drawPngFile(SD,"/VaultBoy.png",100,30,240,240);
    String logoName = "piplogo.png";
    M5.Lcd.setTextSize(1);
    String fileNameStr = sdUtil.getFilePath(logoName);
    M5.Lcd.drawPngFile(SD, (const char *)&fileNameStr, 0, 10);

    int x, y;
    std::tie(x, y) = lcdUtil.getLcdCenter();
    // バージョン表示
    M5.Lcd.setCursor(x, y + 30);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("%s", "ver:0.3"); // (文字列, x座標, y座標) ※フォント省略可
}

void VersionDisplay::update()
{
}