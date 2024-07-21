#ifndef binarydatautil_h
#define binarydatautil_h

class BinaryDataUtil
{
 public:
    static unsigned char *convertIntToLittleEndian(int value, int numBytes);
};


#endif
