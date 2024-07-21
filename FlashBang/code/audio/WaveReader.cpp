#include "WaveReader.h"

SoundData *WaveReader::getSoundData(string filePath)
{
    soundData = new SoundData();
    readFile(filePath);
    return soundData;
}

void WaveReader::readFile(string filePath) 
{
    BinaryFileReader *reader = new BinaryFileReader(filePath);
    readRIFFHeader(reader);
    bool dataChunkWasRead = false;
    bool formatChunkWasRead = false;
    while (!dataChunkWasRead || !formatChunkWasRead)
    {
    int chunkType = readChunk(reader);
    switch (chunkType)
    {
    case DATA_CHUNK:
        dataChunkWasRead = true;
        break;
    case FORMAT_CHUNK:
        formatChunkWasRead = true;
        break;
    }
    }
    delete reader;
}

void WaveReader::readRIFFHeader(BinaryFileReader *reader)
{
    //OUTPUT("header = ");  // Should be "RIFF"
    printf("header = ");
    int i = 0;
    for (i = 0; i < 4; ++i) {
        unsigned char c = reader->readChar();
        //OUTPUT(StringUtil::charToString(c));
        printf("%c", c);
    }
    //OUTPUT("\n");
    printf("\n");
    // Read size (discard) = file-size - 8
    reader->readInt32LittleEndian(4);
    // Read format - Should be "WAVE"
    //OUTPUT("format = ");
    for (i = 0; i < 4; ++i) {
    unsigned char c = reader->readChar();
    //OUTPUT(StringUtil::charToString(c));
    }
    //OUTPUT("\n");
}

int WaveReader::readChunk(BinaryFileReader *reader)
{
    // format chunk name
    //OUTPUT("chunk name = ");
    unsigned char *c = new unsigned char[4];
    for (int i = 0; i < 4; ++i) {
    c[i] = reader->readChar();
    //OUTPUT(StringUtil::charToString(c[i]));
    }
    //OUTPUT("\n");
    if (c[0] == 'f' && c[1] == 'm' && c[2] == 't')
    {
    readFormatChunk(reader);
    return FORMAT_CHUNK;
    }
    if (c[0] == 'd' && c[1] == 'a' && c[2] == 't' && c[3] == 'a')
    {
    readDataChunk(reader);
    return DATA_CHUNK;
    }
    skipChunk(reader);
    return UNKNOWN_CHUNK;
}

void WaveReader::readFormatChunk(BinaryFileReader *reader)
{
    int chunk1Size = reader->readInt32LittleEndian(4);
    //OUTPUT("format chunk size = ");
    //OUTPUT(StringUtil::intToString(chunk1Size));
    //OUTPUT("\n");
    int extraBytes = chunk1Size - 16;
    // Audio format  - should be 1 for noncompressed data
    // compressed WAVE are not handled here  
    int format = reader->readInt32LittleEndian(2);
    //OUTPUT("format = ");
    //OUTPUT(StringUtil::intToString(format));
    //OUTPUT("\n");
    // number of channels
    soundData->setNumChannels(reader->readInt32LittleEndian(2));
    // sample rate
    soundData->setSamplesPerSecond(reader->readInt32LittleEndian(4));
    // byte rate
    reader->readInt32LittleEndian(4);
    // block align
    reader->readInt32LittleEndian(2);
    // bits per sample
    soundData->setBitsPerSample(reader->readInt32LittleEndian(2));
    int numExtraBytes = 0;
    if (extraBytes > 0)
    {
    numExtraBytes = reader->readInt32LittleEndian(2);
    //OUTPUT("# extra bytes = ");
    //OUTPUT(StringUtil::intToString(extraBytes));
    //OUTPUT("\n");
    }
    for (int ex = 0; ex < numExtraBytes; ++ex)
    {
    reader->readChar();
    }
}

void WaveReader::readDataChunk(BinaryFileReader *reader)
{
    int dataSize = reader->readInt32LittleEndian(4);
    soundData->setDataSize(dataSize);
    for (int index = 0; index < dataSize; ++index) {
        soundData->setDataValue(index, reader->readChar());
    }
}

void WaveReader::skipChunk(BinaryFileReader *reader)
{
    int chunkSize = reader->readInt32LittleEndian(4);
    for (int c = 0; c < chunkSize; ++c)
    {
    reader->readChar();
    }
}
