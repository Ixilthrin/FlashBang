#ifndef wavereader_h
#define wavereader_h

#include <string>
#include "BinaryFileReader.h"
#include "SoundData.h"
using namespace std;
// Reads a wave file and returns a SoundData object.
class WaveReader
{
 private:
    void readFile(string filePath);
    void readRIFFHeader(BinaryFileReader *reader);
    int readChunk(BinaryFileReader *reader);
    void readFormatChunk(BinaryFileReader *reader);
    void readDataChunk(BinaryFileReader *reader);
    void skipChunk(BinaryFileReader *reader);
    SoundData *soundData;
    enum
    {
        DATA_CHUNK,
        FORMAT_CHUNK,
        UNKNOWN_CHUNK
    };
 public:
    WaveReader() {}
    ~WaveReader() {}
    SoundData *getSoundData(string filePath);
};

#endif
