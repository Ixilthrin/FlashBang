/*
 *  SoundStore.h
 *  Galileo
 *
 *  Created by David Stover on 1/11/09.
 *  Copyright 2009 MEK Graphics (Max Evan Kyle Technologies). All rights reserved.
 *  
 *  The goal here is to explore sound technologies.  In particular
 *  OpenAL will be used.
 *  
 *  Here are the steps that were followed in the process of discovery:
 *  (1) Added OpenAL framework to the project.  Note that the audio 
 *      units framework has already been added, as well as the
 *      audio toolkit framework.  I am not sure what is needed at
 *      this point.
 *  (2) Added constructor and virtual destructor.
 *  (3) Added public method setup().
 *  (4) I looked at the OpenAL framework header files, and so I added
 *      the following includes:
 *      
 *          #include <OpenAL/al.h>
 *          #include <OpenAL/alc.h>
 *          #include <OpenAL/MacOSX_OALExtensions.h>

 *  (5) The samples I see so far use WAV files.  Eventually I want
 *      to be able to use AIFF files, and other sound formats as well.
 *      For now I will use open-can.wav.  I do not believe this is a 
 *      stereo file, so there might be a problem playing it properly.
 *  (6) Added buffer and source fields.
 *  (7) Added position arrays inside the setup() method.
 *  (8) The functions from alut to load and unload a
 *      wav file do not compile, because the alut is
 *      no longer a part of Mac's openAL framework.
 *      But I should be able to read the wav format
 *      using my own code that is already developed.
 *      I just need to copy it from my windows box.
 *  (9) I copied my files to a memory card.  This is viewable
 *      on my Mac in the /Volumes directory under "No Name" because
 *      the memory card has no specified name.
 *  (10) I copied my files and ran the build.  When alGetError() is
 *       called, the linker complains that _alGetError() symbol
 *       cannot be found.  So there is something wrong with
 *       the binaries in the openAL framework.
 *  (11) Fixed the problem by moving the OpenAL framework in
 *       /Library/Frameworks to a different name (OpenAL.framework.hidden)
 *       The first question is why was it there and not working,
 *       and the second question is what happens if this program
 *       is used on other computers?  I am not sure if this is
 *       something I installed, or was there when I got the computer.
 *       According to the time stamps, it looks like something I
 *       installed, but I don't see a year on the date, only the month
 *       and day.
 *  (12) Loaded the sound data into the buffer.  Note that I am not sure
 *       if the data returned by the SoundData object needs to be released,
 *       or when.
 *  (13) Added the code to bind the buffer to the source.
 *  (14) Added the play() method.
 *  (15) It still was not working.  I added the device and the context,
 *       and then it worked.  There is a lot of static in the sound,
 *       so this might be a problem with the sound data, such as endian-ness.
 */

//#ifdef WIN32
#include <al.h>
#include <alc.h>
//#include <alut.h>
//#else
//#include <OpenAL/al.h>
//#include <OpenAL/alc.h>
//#include <OpenAL/MacOSX_OALExtensions.h>
//#endif

#include "SoundData.h"

class SoundStore
{
private:
    // Buffers hold sound data.
    ALuint *buffers;
    
    // Sources are points emitting sound.
    ALuint *source;
    void fillBuffer(int index, SoundData *soundData);
public:
    SoundStore();
    virtual ~SoundStore();
    void setup();
    void play(int sound);
	void stop(int sound);
};


