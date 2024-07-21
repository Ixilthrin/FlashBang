#pragma once

#include <memory>
#include <string>
#include "ImageReader.h"

using std::string;
using std::shared_ptr;

class CardImageData
{
private:
    shared_ptr<ImageReader> _imageReader;
    shared_ptr<ImageReader> _backImageReader;
public:
    CardImageData(string path);
    ~CardImageData();
    void setBackPath(string path);
    shared_ptr<ImageReader> getImageReader();
    shared_ptr<ImageReader> getBackImageReader();
};