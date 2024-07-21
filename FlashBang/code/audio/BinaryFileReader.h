#ifndef binaryfilereader
#define binaryfilereader

#include <string>
#include <stdio.h>

using namespace std;

class BinaryFileReader {
    string fileName;
    FILE *file;
 public:
    BinaryFileReader(string f);
    ~BinaryFileReader();
    int readInt32BigEndian(int byteCount);
    int readInt32LittleEndian(int byteCount);
    unsigned char readChar();
};

#endif
