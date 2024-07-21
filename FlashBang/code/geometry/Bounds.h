#pragma once

#include "XYZ.h"

class Bounds
{
private:
    XYZ _upperLeft;
    float _width;
    float _height;
public:
    Bounds(XYZ upperLeft, int width, int height) 
    {
        _upperLeft = upperLeft;
        _width = width;
        _height = height;
    }

    bool contains(XYZ point);
    bool intersects(Bounds other);
};
