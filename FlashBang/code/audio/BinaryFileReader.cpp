#include "BinaryFileReader.h"

BinaryFileReader::BinaryFileReader(string f)
{
    fileName = f;
    fopen_s(&file, f.c_str(), "rb");
}    

BinaryFileReader::~BinaryFileReader()
{
    fclose(file);
}

int BinaryFileReader::readInt32BigEndian(int byteCount)
{
    unsigned char *buffer;
    buffer = (unsigned char *) malloc(byteCount);
    fread(buffer, 1, byteCount, file);
    int value = 0;
    for (int index = 0; index < byteCount; ++index) {
        value += (int) (((unsigned int) buffer[index] << (8 * (byteCount - index - 1))));
    }
    free(buffer);
    return value;
}

int BinaryFileReader::readInt32LittleEndian(int byteCount)
{
    unsigned char *buffer;
    buffer = (unsigned char *) malloc(byteCount);
    fread(buffer, 1, byteCount, file);
    int value = 0;
    for (int index = 0; index < byteCount; ++index) {
        value += (int) ((((unsigned int) buffer[index]) << (8 * index)));
    }
    free(buffer);
    return value;
}

unsigned char BinaryFileReader::readChar()
{
    unsigned char *buffer;
    buffer = (unsigned char *) malloc(1);
    fread(buffer, 1, 1, file);
    unsigned char c = (char) buffer[0];
    free(buffer);
    return c;
}
