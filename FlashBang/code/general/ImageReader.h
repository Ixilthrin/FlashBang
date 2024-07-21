#pragma once

#include <glad/glad.h>
#include <string>

class ImageReader
{
private:
    std::string _filePath;
    GLubyte* _imageData;
    void LoadData();
    int _height;
    int _width;
    int _componentCount;
    bool _dataWasLoaded;
public:
    ImageReader(std::string filePath);
    ~ImageReader();
    GLubyte* getImageData();
    int getWidth();
    int getHeight();
    int getComponentCount();
};