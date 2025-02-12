#pragma once



#include <phonon.h>
#include "Ocean/Audio/audio.hpp"
#include "audio.hpp"

namespace sonar{

    //DOES NOT DEALLOC BUFFERS.
    static IPLAudioBuffer* allocatebuffer(int channels =2, int samples =512){
        IPLAudioBuffer* buffer;
        iplAudioBufferAllocate(*sonar::steamaudio::context, channels, samples, buffer);
        return buffer;
    }

}