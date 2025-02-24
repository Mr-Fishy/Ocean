#pragma once



#include <phonon.h>
#include "Ocean/Audio/audio.hpp"
#include "Ocean/Types/SmartPtrs.hpp"
#include "audio.hpp"
#include "steamaudio_dataholding.hpp"

namespace sonar{
    struct convert{

    //For temp buffers, must be not interleaved.
    static void allocatebuffer(std::string name,IPLAudioBuffer &buff);

    //this is for incoming, adds to the map
    static void deinterleave(std::string name, float* indata, int channels, int samples);
    static void deinterleave(std::string name, float* indata, IPLAudioBuffer* buff);

    //this is for outgoing, gets from the map.
    static void interleave(std::string name, float* outdata);

    };
}