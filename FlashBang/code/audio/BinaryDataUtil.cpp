#include "BinaryDataUtil.h"

unsigned char *BinaryDataUtil::convertIntToLittleEndian(int value, 
                            int numBytes)
{
    unsigned char *bytes = new unsigned char[numBytes];
    // 255 represents 8 bits all turned on.
    unsigned int bitMask = 255;
    for (int index = 0; index < numBytes; ++index)
    {
    bytes[index] = (unsigned char) ((bitMask & value) >> (8 * index));
    bitMask = bitMask << 8;
    }
    return bytes;
}
