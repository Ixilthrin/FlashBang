/*
 *  SoundStore.h
 *  Galileo
 *
 *  Created by David Stover on 1/11/09.
 *  Copyright 2009 MEK Graphics (Max Evan Kyle Technologies). All rights reserved.
 */

#include "SoundStore.h"
#include "WaveReader.h"
#include "AIFFReader.h"

SoundStore::SoundStore()
{
}

SoundStore::~SoundStore()
{
}

void SoundStore::fillBuffer(int index, SoundData *soundData)
{
	// Load sound data
    ALenum format;  // AL_FORMAT_MONO8, AL_FORMAT_MONO16, AL_FORMAT_STEREO8, AL_FORMAT_STEREO16
    ALsizei size;
    ALvoid* data;
    ALsizei freq;
    ALboolean loop;

    int nChannels = soundData->getNumChannels();
    int bitDepth = soundData->getBitsPerSample();
    int perSecond = soundData->getSamplesPerSecond();
    switch (nChannels)
    {
    case 1:
        if (bitDepth == 8)
        {
            format = AL_FORMAT_MONO8;
            break;
        }
        else if (bitDepth == 16)
        {
            format = AL_FORMAT_MONO16;
            break;
        }
    case 2:
        if (bitDepth == 8)
        {
            format = AL_FORMAT_STEREO8;
            break;
        }
        else if (bitDepth == 16)
        {
            format = AL_FORMAT_STEREO16;
            break;
        }
    default:
        return;
    }

    if (true)
    {
        size = soundData->getDataSize();
        printf("data size is %d\n", size);
        freq = perSecond;
        printf("freq = %d\n", freq);
        data = soundData->getData();
    }
    else
    {
        //alutLoadWAVFile("boop.wav", &format, &data, &size, &freq, &loop);
    }

    alBufferData(buffers[index], format, data, size, freq);
    //alutUnloadWAV(format, data, size, freq);
    if (alGetError() != AL_NO_ERROR)
    {
        printf("OpenAL error trying to create the buffer\n");
        return;
    }
}

void SoundStore::setup()
{
    // Initialization 
    ALCdevice *device = alcOpenDevice(NULL); // select the "preferred device" 

    if (device) {
        ALCcontext *context = alcCreateContext(device, NULL);
        alcMakeContextCurrent(context);
    }
    else
    {
        printf("cannot create device\n");
        return;
    }

    // Position of the source sound.
    ALfloat sourcePos[] = { 0.0, 0.0, 0.0 };

    // Velocity of the source sound.
    ALfloat sourceVel[] = { 0.0, 0.0, 0.0 };


    // Position of the listener.
    ALfloat listenerPos[] = { 0.0, 0.0, 1.0 };

    // Velocity of the listener.
    ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };

    // Orientation of the listener. (first 3 elements are "at", second 3 are "up")
    ALfloat listenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

    // Load sound data
    //ALenum format;  // AL_FORMAT_MONO8, AL_FORMAT_MONO16, AL_FORMAT_STEREO8, AL_FORMAT_STEREO16
    //ALsizei size;
    //ALvoid* data;
    //ALsizei freq;
    //ALboolean loop;

    int numberOfSounds = 2;
    buffers = new ALuint[numberOfSounds];

    // Load wav data into a buffer.
    printf("About to load data into buffer.\n");
    alGenBuffers(numberOfSounds, buffers);
    printf("Checking for error\n");
    if (alGetError() != AL_NO_ERROR)
        return;
    printf("No error (yet)\n");

    WaveReader *wavReader = new WaveReader();
    AIFFReader *aiffReader = new AIFFReader();

    SoundData *soundData = wavReader->getSoundData("../FlashBangProject/resources/sliding2.wav");
    fillBuffer(0, soundData);

    soundData = wavReader->getSoundData("../FlashBangProject/resources/flipping5.wav");
    fillBuffer(1, soundData);

    // Bind buffer with a source.
    source = new ALuint[numberOfSounds];
    alGenSources(numberOfSounds, source);

    if (alGetError() == AL_INVALID_OPERATION)
    {
        printf("Invalid operation\n");
    }

    if (alGetError() != AL_NO_ERROR)
    {
        printf("OpenAL error trying to bind source\n");
        return;
    }

    bool loop = false;

    for (int i = 0; i < numberOfSounds; ++i)
    {
        alSourcei(source[i], AL_BUFFER, buffers[i]);
        alSourcef(source[i], AL_PITCH, 1.0f);
        alSourcef(source[i], AL_GAIN, 1.0f);
        alSourcefv(source[i], AL_POSITION, sourcePos);
        alSourcefv(source[i], AL_VELOCITY, sourceVel);
        alSourcei(source[i], AL_LOOPING, loop);
    }

    // Set listener values.
    alListenerfv(AL_POSITION, listenerPos);
    alListenerfv(AL_VELOCITY, listenerVel);
    alListenerfv(AL_ORIENTATION, listenerOri);

    // Clear the error state
    alGetError();
    printf("finished setup\n");
}

void SoundStore::play(int sound)
{
	stop(sound);
    alSourcePlay(source[sound]);
}

void SoundStore::stop(int sound)
{
	alSourceStop(source[sound]);
}