#pragma once

class OverheadCamera
{
private:
    int _translationX;
    int _translationY;
    int _zoomFactor;

public:
    OverheadCamera();

    void setTranslationX(int translation);
    void setTranslationY(int translation);
    void setZoomFactor(float zoomFactor);

    int getTranslationX();
    int getTranslationY();
    float getZoomFactor();
};