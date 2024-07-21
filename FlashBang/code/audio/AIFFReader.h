#ifndef __AIFF_READER_H__
#define __AIFF_READER_H__

// An AIFF file is a digital audio file.
// Usual extensions are .aiff, .aif, and .aifc
// The latter extension is the compressed version,
// which this class will probably not support.
//
// An AIFF file is a big-endian PCM binary file.
// This differs from a WAV file, which is litte-endian.
// 
// For the Mac, Apply created their own version of 
// the AIFF file format that is little-endian.  It is
// called AIFF-C/sowt.  It differs from the standard
// AIFF file format only in that it is little-endian.
// Upon reading a sound file created with GarageBand,
// which is an Apply application, the file format
// for the exported aif file seems to be big-endian,
// so I am not sure where the special Apple little-endian
// aif format is used.

// Initially the reader will only be concerned with
// the common chunk, which stores the sound file
// attributes, and the sound data chunk.

// The sampling rate is encoded in an 80-bit IEEE
// 754 floating point number.
// According to the documentation, the first bit
// represents the sign, the next 15 represent the
// exponent, and the next 64 bits represent the
// mantissa.
// According to Google, the significand is 64 bits,
// the exponent field is 15 bits, and one bit is for
// sign.  The exponent field uses Excess-N representation, 
// meaning that 16383 has to be subtracted from the 
// written exponent.  This is called excess-16383.
// The exponent field 32767 is reserved.
#include <string>
#include "BinaryFileReader.h"
#include "SoundData.h"
using namespace std;
// Reads an AIFF file and returns a SoundData object.
class AIFFReader
{
 private:
    void readFile(string filePath);
    void readAIFFHeader(BinaryFileReader *reader);
    int readChunk(BinaryFileReader *reader);
    void readCHANChunk(BinaryFileReader *reader);
    void readCommonChunk(BinaryFileReader *reader);
    void readCommentChunk(BinaryFileReader *reader);
    void readDataChunk(BinaryFileReader *reader);
    void skipChunk(BinaryFileReader *reader);
    SoundData *soundData;
    enum
    {
        CHAN_CHUNK,
        COMMON_CHUNK,
        COMMENTS_CHUNK,
        DATA_CHUNK,
        UNKNOWN_CHUNK
    };
 public:
    AIFFReader() {}
    ~AIFFReader() {}
    SoundData *getSoundData(string filePath);
};

#endif
