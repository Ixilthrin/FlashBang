#pragma once

class OverheadCamera
{
private:
    float _translationX;
    float _translationY;
    float _zoomFactor;

public:
    OverheadCamera();

    void setTranslationX(float translation);
    void setTranslationY(float translation);
    void setZoomFactor(float zoomFactor);

    float getTranslationX();
    float getTranslationY();
    float getZoomFactor();
};