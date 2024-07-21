#ifndef __DATA_WRITER_H__
#define __DATA_WRITER_H__

#include <fstream>
#include <string>

// To be used for serializing data to a file.
class DataWriter
{
 private:
    std::fstream *_stream;
 public:
    DataWriter(std::string filePath)
    {
    _stream = new fstream(filePath.c_str(), ios_base::out);
    }
    ~DataWriter()
    {
    _stream->close();
    delete _stream;
    }
    void writeInt(int v)
    {
    *_stream << v << ' ';
    }
    void writeFloat(float v)
    {
    *_stream << v << ' ';
    }
    void writeString(std::string v)
    {
    *_stream << v << ' ';
    }
};

#endif
