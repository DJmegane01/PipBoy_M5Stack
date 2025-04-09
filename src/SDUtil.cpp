#include "SDUtil.h"

SDUtil sdUtil;

void SDUtil::init()
{
    // SDカード読み込み初期化
    while (false == SD.begin(GPIO_NUM_4, SPI, 15000000))
    {
        Serial.println("SD wait...");
        delay(500);
    }
    if (fileList->length() > 0)
    {
        resetFileList();
    }
    createFileList();
}

/**
 * @brief SDカードを読み込み、ファイルリストを作成する。
 *
 */
void SDUtil::createFileList()
{
    int fileCount = 0;
    // SDカード読み込み
    File root = SD.open("/");
    if (root)
    {
        File file = root.openNextFile();
        while (file)
        {
            if (file.isDirectory())
            {
                // Dir skip
            }
            else
            {
                // File
                String filename = file.name();
                if (filename.indexOf(".png") != -1 || filename.indexOf(".bmp") != -1)
                {
                    // Find
                    fileList[fileCount] = filename;
                    fileCount++;
                }
            }
            // 次のファイルを読み込む
            file = root.openNextFile();
        }
    }
}

/*
 *ファイルパス取得関数
 */
String SDUtil::getFilePath(String targetFileName)
{
    String fileNameStr = "/";
    for (int i = 0; i < maxFile; i++)
    {
        if (fileList[i] == targetFileName)
        {
            String filename = fileList[i];
            fileNameStr += filename.c_str();
            break;
        }
    }
    return fileNameStr;
}

void SDUtil::resetFileList()
{
    fileList->clear();
}