#ifndef sounddata_h
#define sounddata_h

#include "DiscreteFunction.h"

class SoundData
{
 public:
    SoundData();
    ~SoundData();
    void setNumChannels(int n) { _numChannels = n; }
    int getNumChannels() { return _numChannels; }
    void setSamplesPerSecond(int n) { _samplesPerSecond = n; }
    int getSamplesPerSecond() { return _samplesPerSecond; }
    void setBitsPerSample(int n) { _bitsPerSample = n; }
    int getBitsPerSample() { return _bitsPerSample; }
    int getNumSamples();
    void setData(unsigned char *data);
    unsigned char *getData() { return _data; }
    void setDataValue(int index, unsigned char value);
    char getDataValue(int index) { return _data[index]; }
    void setDataSize(int dataSize) { _dataSize = dataSize; }
    int getDataSize() { return _dataSize; }
    // 0 ==> left channel, 1 ==> right channel
    DiscreteFunction *createDiscreteFunctionFromChannel(int channel);
    static SoundData *createSoundDataFromDiscreteFunctions(
                    int numChannels,
                    int samplesPerSecond,
                    int bitsPerSample,
                    DiscreteFunction *leftChannel,
                    DiscreteFunction *rightChannel);
    static SoundData *createSoundDataFromDiscreteFunction(
                                        int samplesPerSecond,
                    int bitsPerSample,
                    float frequency,
                    float duration,
                    DiscreteFunction *soundPacket);
 private:
    int _numChannels;
    int _samplesPerSecond;
    int _bitsPerSample;
    int _dataSize;
    unsigned char *_data;
};

#endif
