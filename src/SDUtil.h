#ifndef SDUtil_H
#define SDUtil_H

#include <M5Stack.h>
#include <SD.h>
#include <FS.h>

class SDUtil
{
public:
    void init();
    String getFilePath(String fileName);
    void createFileList();
    void resetFileList();

private:
    static const int maxFile = 100;
    String fileList[maxFile];
};

extern SDUtil sdUtil;

#endif