#pragma once

#include "XYZ.h"

class Converter
{
    int _screenWidth;
    int _screenHeight;
public:
    Converter(int screenWidth, int screenHeight);
    float screenXToNDC(int x);
    float screenYToNDC(int y);
    float screenTranslationXToNDC(int transX);
    float screenTranslationYToNDC(int transY);
};