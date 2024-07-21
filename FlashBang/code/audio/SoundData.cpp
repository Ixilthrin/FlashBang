#include "SoundData.h"
#include "BinaryDataUtil.h"
#include "DataWriter.h"

SoundData::SoundData()
    : _data(0)
{
}

SoundData::~SoundData()
{
    if (_data)
    {
    delete [] _data;
    }
}

int SoundData::getNumSamples()
{
    int bytesPerSample = _bitsPerSample / 8;
    if (bytesPerSample * _numChannels > 0)
    {
    return _dataSize / (bytesPerSample * _numChannels);
    }
    return 0;
}

void SoundData::setData(unsigned char *data)
{
    if (_data)
    {
    delete [] data;
    }
    _data = data;
}

void SoundData::setDataValue(int index, unsigned char value)
{
    if (!_data)
    {
    _data = new unsigned char[_dataSize];
    }
    _data[index] = value;
}

// 0 ==> left, 1 ==> right, or vice versa, not really sure.
// All 16 bit (or greater) data is stored unsigned and little-endian
DiscreteFunction *SoundData::createDiscreteFunctionFromChannel(int channel)
{
    //OUTPUT("DF: numSamples = ");
    //OUTPUT(StringUtil::intToString(getNumSamples()));
    //OUTPUT("\n");
    DiscreteFunction *function = new DiscreteFunction(getNumSamples());
    int bytesPerSample = _bitsPerSample / 8;
    //OUTPUT("DF: bytesPerSample = ");
    //OUTPUT(StringUtil::intToString(bytesPerSample));
    //OUTPUT("\n");
    int allChannelDataLength = bytesPerSample * _numChannels;
    //OUTPUT("DF: allChannelDataLength = ");
    //OUTPUT(StringUtil::intToString(allChannelDataLength));
    //OUTPUT("\n");
    int offset = bytesPerSample * channel;
    //OUTPUT("DF: offset = ");
    //OUTPUT(StringUtil::intToString(offset));
    //OUTPUT("\n");
    for (int i = 0; i < getNumSamples(); ++i)
    {
    int baseDataIndex = i * allChannelDataLength + offset;
    unsigned int value = 0;
    for (int j = 0; j < bytesPerSample; ++j)
    {
        // Build the int value from the char values.
        // Do bit-wise or with shifted char data.
        unsigned char charData = (unsigned char) _data[baseDataIndex + j];
        unsigned int intData = (unsigned int) charData;
        unsigned int shiftedData = intData << (8 * j);
        value |= shiftedData;
        if (i < 10)
        {
        //OUTPUT("DF: byte = ");
        //OUTPUT(StringUtil::unsignedIntToString(charData));
        //OUTPUT("\n");
        //OUTPUT("DF: value = ");
        //OUTPUT(StringUtil::unsignedIntToString(value));
        //OUTPUT("\n");
        }
    }
    function->setValue(i, (float) ((int) value));
    }
    return function;
}

SoundData *SoundData::createSoundDataFromDiscreteFunctions(
                    int numChannels,
                    int samplesPerSecond,
                    int bitsPerSample,
                    DiscreteFunction *leftChannel,
                    DiscreteFunction *rightChannel)
{
    SoundData *soundData = new SoundData();
    soundData->setNumChannels(numChannels);
    soundData->setSamplesPerSecond(samplesPerSecond);
    soundData->setBitsPerSample(bitsPerSample);
    int bytesPerSample = bitsPerSample / 8;
    int allChannelDataLength = bytesPerSample * numChannels;
    int offset = bytesPerSample;
    int numSamples = leftChannel->getNumberOfPoints();
    if (numSamples > rightChannel->getNumberOfPoints())
    {
    numSamples = rightChannel->getNumberOfPoints();
    }
    int dataSize = numSamples * allChannelDataLength;
    soundData->setDataSize(dataSize);
    for (int i = 0; i < numSamples; ++i)
    {
    int leftBaseDataIndex = i * allChannelDataLength;
    int leftValue = (int) leftChannel->getValue(i);
    unsigned char * leftBytes = BinaryDataUtil::convertIntToLittleEndian(
                                 leftValue, 
                                 bytesPerSample
                                 );
for (int m = 0; m < bytesPerSample; ++m)
    {
        soundData->setDataValue(leftBaseDataIndex + m, leftBytes[m]);
    }
    delete [] leftBytes;

    int rightBaseDataIndex = i * allChannelDataLength + offset;
    int rightValue = (int) rightChannel->getValue(i);
    unsigned char * rightBytes = BinaryDataUtil::convertIntToLittleEndian(
                            rightValue, 
                            bytesPerSample
                            );
    for (int n = 0; n < bytesPerSample; ++n)
    {
        soundData->setDataValue(rightBaseDataIndex + n, rightBytes[n]);
    }
    delete [] rightBytes;
    }
    return soundData;
}

// The soundPacket will be repeated according to the specified
// frequency for the specified duration.
SoundData *SoundData::createSoundDataFromDiscreteFunction(
                                        int samplesPerSecond,
                    int bitsPerSample,
                    float frequency,
                    float duration,
                    DiscreteFunction *soundPacket)
{
    // This is experimental.
    int numberOfChannels = 1;
    int bytesPerSample = 1;
    unsigned int maskLowByte = 255;
    unsigned int maskHighByte = 32512;
    SoundData *data = new SoundData();
    data->setNumChannels(numberOfChannels);
    data->setSamplesPerSecond(44100);
    data->setBitsPerSample(8 * bytesPerSample);
    data->setDataSize(100 * 440 * bytesPerSample * numberOfChannels);
    int index = 0;

    DataWriter *writer = new DataWriter("sounddata.txt");
    for (int i = 0; i < 440; ++i)
    {
    for (int j = 0; j < 100; ++j)
    {
        unsigned int value = soundPacket->getValue(j);
        //OUTPUT("sound value = ");
        //OUTPUT(StringUtil::intToString((int) value));
        //OUTPUT("\n");
        unsigned int highValue = (value & maskHighByte);
        highValue >>= 8;
        //OUTPUT("sound high value = ");
        //OUTPUT(StringUtil::intToString((int) highValue));
        //OUTPUT("\n");
        unsigned int lowValue = (value & maskLowByte);
        //OUTPUT("sound low value = ");
        //OUTPUT(StringUtil::intToString((int) lowValue));
        //OUTPUT("\n");
        
        for (int k = 0; k < numberOfChannels; ++k)
        {
        data->setDataValue(index, lowValue);
        writer->writeInt(lowValue);
        ++index;
        if (bytesPerSample == 2)
        {
            data->setDataValue(index, highValue);
            writer->writeInt(highValue);
            ++index;
        }
        }
    }
    }
    delete writer;
    return data;
}
