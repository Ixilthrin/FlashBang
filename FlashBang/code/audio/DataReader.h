#ifndef __DATA_READER_H__
#define __DATA_READER_H__

#include <fstream>
#include <string>
using namespace std;

class DataReader
{
 private:
    std::fstream *_stream;
 public:
    DataReader(std::string filePath);
    ~DataReader()
    {
    _stream->close();
    delete _stream;
    }
    int readInt();
    float readFloat();
    std::string readString();
};

#endif
