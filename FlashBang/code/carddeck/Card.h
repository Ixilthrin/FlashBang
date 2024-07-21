#pragma once

#include "ImageReader.h"
#include <string>
#include <vector>

class Card
{
private:
    int _width;
    int _height;
    int _translationX;
    int _translationY;
    std::string _imagePath;
    std::string _flippedImagePath;
    bool _isFlipped;
    bool _requestFlip;

public:
    Card(int width,
        int height,
        int translationX,
        int translationY,
        std::string imagePath = "");

    void setTranslationX(int translation);
    void setTranslationY(int translation);
    int getWidth();
    int getHeight();
    int getTranslationX();
    int getTranslationY();
    std::string getImagePath();
    std::string getFlippedImagePath();
    void setFlippedImagePath(std::string path);
    bool hasFlipSide();
    void flip();
    bool isFlipped();
    bool contains(int x, int y);
    bool requestFlip();
    void flipHalfComplete();
    void flipComplete();
    static std::string getVertexShaderPath();
    static std::string getFragmentShaderPath();
};
