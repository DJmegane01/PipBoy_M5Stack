#include "LcdUtil.h"

LcdUtil lcdUtil;

std::tuple<int, int> LcdUtil::getLcdCenter()
{
    return std::make_tuple(lcdWidth / 2, lcdHeight / 2);
}

int LcdUtil::getLcdHalfHeight()
{
    return lcdHeight / 2;
}

int LcdUtil::getLcdHalfWidth()
{
    return lcdWidth / 2;
}
