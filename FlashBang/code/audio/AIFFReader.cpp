#include "AIFFReader.h"

// This reads 10 bytes that represent an IEEE 754 80-bit 
// floating point number.
static int readSamplingRate(BinaryFileReader *reader)
{
    unsigned int sign = 0;
    unsigned char bytes[10];
    for (int i = 0; i < 10; ++i)
    {
    bytes[i] = reader->readChar();
    }

    // Get the sign - shift right by 7 bits.
    sign = bytes[0] >> 7;
    unsigned char bitMask = 127;  // All 1's except left-most bit is 0.
    // Get the first byte representing the exponent
    unsigned char exp0 = bytes[0] & bitMask;
    // Get the second byte of the exponent.
    unsigned char exp1 = bytes[1];
    // Calculate the exponent
    unsigned int exponent = ((unsigned int) exp0) << 8 | ((unsigned int) exp1);
    //OUTPUT("Sampling rate exponent = ");
    //OUTPUT(StringUtil::intToString((int) exponent));
    //OUTPUT("\n");

    // Get the 1st 4 bytes of the mantissa (the others are ignored).
    unsigned int mantissa = 0;
    mantissa |= (unsigned int) (bytes[6] << 24);
    mantissa |= (unsigned int) (bytes[5] << 16);
    mantissa |= (unsigned int) (bytes[4] << 8);
    mantissa |= (unsigned int) bytes[3];
    //OUTPUT("Sampling rate mantissa = ");
    //OUTPUT(StringUtil::intToString((int) mantissa));
    //OUTPUT("\n");
    return 0;
}

SoundData *AIFFReader::getSoundData(string filePath)
{
    soundData = new SoundData();
    readFile(filePath);
    return soundData;
}

void AIFFReader::readFile(string filePath) 
{
    BinaryFileReader *reader = new BinaryFileReader(filePath);
    readAIFFHeader(reader);
    bool dataChunkWasRead = false;
    bool formatChunkWasRead = false;
    while (!dataChunkWasRead || !formatChunkWasRead)
    {
    int chunkType = readChunk(reader);
    switch (chunkType)
    {
    case COMMON_CHUNK:
        formatChunkWasRead = true;
        break;
    case DATA_CHUNK:
        dataChunkWasRead = true;
        break;
    }
    }
    delete reader;
}

void AIFFReader::readAIFFHeader(BinaryFileReader *reader)
{
    //OUTPUT("header = ");  // Should be "FORM"
    int i = 0;
    for (i = 0; i < 4; ++i) {
    unsigned char c = reader->readChar();
    //OUTPUT(StringUtil::charToString(c));
    }
    //OUTPUT("\n");
    // Read size (discard) = file-size - 8
    reader->readInt32LittleEndian(4);
    // Read format - Should be "AIFF"
    //OUTPUT("format = ");
    for (i = 0; i < 4; ++i) {
    unsigned char c = reader->readChar();
    //OUTPUT(StringUtil::charToString(c));
    }
    //OUTPUT("\n");
}

int AIFFReader::readChunk(BinaryFileReader *reader)
{
    // format chunk name
    //OUTPUT("chunk name = ");
    unsigned char *c = new unsigned char[4];
    for (int i = 0; i < 4; ++i) {
    c[i] = reader->readChar();
    //OUTPUT(StringUtil::charToString(c[i]));
    }
    //OUTPUT("\n");
    if (c[0] == 'C' && c[1] == 'O' && c[2] == 'M' && c[3] == 'M')
    {
    readCommonChunk(reader);
    return COMMON_CHUNK;
    }
    if (c[0] == 'S' && c[1] == 'S' && c[2] == 'N' && c[3] == 'D')
    {
    readDataChunk(reader);
    return DATA_CHUNK;
    }
    if (c[0] == 'C' && c[1] == 'O' && c[2] == 'M' && c[3] == 'T')
    {
    readCommentChunk(reader);
    return COMMON_CHUNK;
    }
    if (c[0] == 'C' && c[1] == 'H' && c[2] == 'A' && c[3] == 'N')
    {
    readCHANChunk(reader);
    return CHAN_CHUNK;
    }
    skipChunk(reader);
    return UNKNOWN_CHUNK;
}

void AIFFReader::readCHANChunk(BinaryFileReader *reader)
{
    int chunkSize = reader->readInt32BigEndian(4);
    for (int i = 0; i < chunkSize; ++i)
    {
    reader->readChar();
    }
}

void AIFFReader::readCommonChunk(BinaryFileReader *reader)
{
    int chunk1Size = reader->readInt32BigEndian(4);
    //OUTPUT("format chunk size = ");
    //OUTPUT(StringUtil::intToString(chunk1Size));
    //OUTPUT("\n");
    int extraBytes = chunk1Size - 16;
    // number of channels
    soundData->setNumChannels(reader->readInt32BigEndian(2));
    //OUTPUT("number of channels = ");
    //OUTPUT(StringUtil::intToString(soundData->getNumChannels()));
    //OUTPUT("\n");
    // Number of sample frames.
    int sampleFrames = reader->readInt32BigEndian(4);
    // bits per sample
    soundData->setBitsPerSample(reader->readInt32BigEndian(2));
    //OUTPUT("Bits per sample = ");
    //OUTPUT(StringUtil::intToString(soundData->getBitsPerSample()));
    //OUTPUT("\n");
    // byte rate
    int samplingRate = readSamplingRate(reader);
    //soundData->setSamplesPerSecond(22050);
    soundData->setSamplesPerSecond(44100);

}

void AIFFReader::readCommentChunk(BinaryFileReader *reader)
{
    int chunk1Size = reader->readInt32BigEndian(4);
    //OUTPUT("Comment chunk size = ");
    //OUTPUT(StringUtil::intToString(chunk1Size));
    //OUTPUT("\n");
    for (int i = 0; i < chunk1Size; ++i)
    {
    reader->readChar();  // Discard
    }
}

void AIFFReader::readDataChunk(BinaryFileReader *reader)
{
    int dataSize = reader->readInt32BigEndian(4);
    //OUTPUT("Data size = ");
    //OUTPUT(StringUtil::intToString(dataSize));
    //OUTPUT("\n");
    int offset = reader->readInt32BigEndian(4);  // Normally zero.
    //OUTPUT("Offset = ");
    //OUTPUT(StringUtil::intToString(offset));
    //OUTPUT("\n");
    soundData->setDataSize(dataSize);
    for (int index = 0; index < dataSize; ++index) {
    unsigned char c0 = reader->readChar();
    unsigned char c1 = reader->readChar();
    soundData->setDataValue(index, c1);
    ++index;
    soundData->setDataValue(index, c0);
    /*
    unsigned int tempValue = c0 << 8 | c1;
    //32768
    int tempValue2 = (int) tempValue;
    //tempValue2 += 32768;
    unsigned int tempValue3 = (unsigned int) tempValue2;
    unsigned char x0 = (unsigned char) tempValue3;
    tempValue3 >>= 8;
    unsigned char x1 = (unsigned char) tempValue3;
    soundData->setDataValue(index, x0);
    ++index;
    soundData->setDataValue(index, x1);
    */
    }
}

void AIFFReader::skipChunk(BinaryFileReader *reader)
{
    int chunkSize = reader->readInt32BigEndian(4);
    for (int i = 0; i < chunkSize; ++i)
    {
    reader->readChar();
    }
}
