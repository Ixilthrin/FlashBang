#include "Card.h"

#include <iostream>
using std::cout;
using std::endl;

Card::Card(
    int width, 
    int height, 
    int translationX, 
    int translationY, 
    std::string imagePath)
{
    _width = width;
    _height = height;
    _translationX = translationX;
    _translationY = translationY;
    _imagePath = imagePath;
    _flippedImagePath = "";
    _isFlipped = false;
    _requestFlip = false;
}

void Card::setTranslationX(int translation)
{
    _translationX = translation;
}

int Card::getWidth()
{
    return _width;
}

int Card::getHeight()
{
    return _height;
}

void Card::setTranslationY(int translation)
{
    _translationY = translation;
}

int Card::getTranslationX()
{
    return _translationX;
}

int Card::getTranslationY()
{
    return _translationY;
}

std::string Card::getImagePath()
{
    return _imagePath;
}

std::string Card::getFlippedImagePath()
{
    return _flippedImagePath;
}

void Card::setFlippedImagePath(std::string path)
{
    _flippedImagePath = path;
}

bool Card::hasFlipSide()
{
    return _flippedImagePath.length() > 0;
}

void Card::flip()
{
    _requestFlip = true;
}

bool Card::isFlipped()
{
    return _isFlipped;
}

bool Card::contains(int x, int y)
{
    if (x > _translationX
        && y > _translationY
        && x < _translationX + _width
        && y < _translationY + _height)
    {
        return true;
    }

    return false;
}

bool Card::requestFlip()
{
    return _requestFlip;
}

void Card::flipHalfComplete()
{
    _isFlipped = !_isFlipped;
}

void Card::flipComplete()
{
    _requestFlip = false;
}

std::string Card::getVertexShaderPath()
{
    return "../FlashBang/shaders/translate_rotatey_zoom_texture.vert.glsl";
    //return "../FlashBang/shaders/translation_and_texture.vert.glsl";
    //return "../FlashBang/shaders/default.vert.glsl";
}

std::string Card::getFragmentShaderPath()
{
    return "../FlashBang/shaders/texture.frag.glsl";
    //return "../FlashBang/shaders/default.frag.glsl";
}
