#include "DataReader.h"

DataReader::DataReader(std::string filePath)
{
    _stream = new fstream(filePath.c_str(), ios_base::in);
}

int DataReader::readInt()
{
    int v;
    *_stream >> v;
    if (_stream->fail())
    {
        // Send message to logger
    }
    return v;
}

float DataReader::readFloat()
{
    float v;
    *_stream >> v;
    if (_stream->fail())
    {
        // Send message to logger
    }
    return v;
}

std::string DataReader::readString()
{
    std::string v;
    *_stream >> v;
    if (_stream->fail())
    {
        // send message to logger.
    }
    return v;
}
