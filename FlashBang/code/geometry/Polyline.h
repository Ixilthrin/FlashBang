#pragma once

#include <vector>
#include "XYZ.h"

class Polyline
{
private:
    std::vector<XYZ> _points;
public:
    Polyline();
    std::vector<XYZ> getPoints();
};