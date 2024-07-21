#include "CardImageData.h"

using std::make_shared;

CardImageData::CardImageData(string path)
{
    _imageReader = make_shared<ImageReader>(path);
    _backImageReader = 0;
}

CardImageData::~CardImageData()
{
}

void CardImageData::setBackPath(string path)
{
    _backImageReader = make_shared<ImageReader>(path);
}

shared_ptr<ImageReader> CardImageData::getImageReader()
{
    return _imageReader;
}

shared_ptr<ImageReader> CardImageData::getBackImageReader()
{
    return _backImageReader;
}