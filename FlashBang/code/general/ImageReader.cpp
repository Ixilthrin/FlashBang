#include "ImageReader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ImageReader::ImageReader(std::string filePath)
{
    _filePath = filePath;
    _width = 0;
    _height = 0;
    _componentCount = 0;
    _dataWasLoaded = false;
}

ImageReader::~ImageReader()
{
    if (_dataWasLoaded)
    {
        stbi_image_free(_imageData);
    }
}

void ImageReader::LoadData()
{
    _imageData = stbi_load(_filePath.c_str(), &_width, &_height, &_componentCount, STBI_rgb_alpha);
    _dataWasLoaded = true;
}

GLubyte* ImageReader::getImageData()
{
    if (!_dataWasLoaded)
    {
        LoadData();
    }
    return _imageData;
}

int ImageReader::getWidth()
{
    if (!_dataWasLoaded)
    {
        LoadData();
    }
    return _width;
}

int ImageReader::getHeight()
{
    if (!_dataWasLoaded)
    {
        LoadData();
    }
    return _height;
}

int ImageReader::getComponentCount()
{
    if (!_dataWasLoaded)
    {
        LoadData();
    }
    return _componentCount;
}