#include "Converter.h"

Converter::Converter(int screenWidth, int screenHeight)
{
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
}


float Converter::screenXToNDC(int x)
{
    float outX = x;

    outX = outX * 2 / (float) _screenWidth;

    outX -= 1;

    return outX;
}

float Converter::screenYToNDC(int y)
{
    float outY = -(float)y + (float)_screenHeight;

    outY = outY * 2 / (float)_screenHeight;

    outY -= 1;

    return outY;
}

float Converter::screenTranslationXToNDC(int transX)
{
    return (float)transX * 2.f / (float)_screenWidth;
}

float Converter::screenTranslationYToNDC(int transY)
{
    return -(float)transY * 2.f / (float)_screenHeight;
}