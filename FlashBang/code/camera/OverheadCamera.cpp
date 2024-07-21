#include "OverheadCamera.h"

#include <iostream>
using std::cout;
using std::endl;

OverheadCamera::OverheadCamera()
{
    _translationX = 0;
    _translationY = 0;
    _zoomFactor = 0;
}

void OverheadCamera::setTranslationX(int translation)
{
    _translationX = translation;
}

void OverheadCamera::setTranslationY(int translation)
{
    _translationY = translation;
}

void OverheadCamera::setZoomFactor(float zoomFactor)
{
    _zoomFactor = zoomFactor;
}

int OverheadCamera::getTranslationX()
{
    return _translationX;
}

int OverheadCamera::getTranslationY()
{
    return _translationY;
}

float OverheadCamera::getZoomFactor()
{
    return _zoomFactor;
}
