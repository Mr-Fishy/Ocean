#pragma once
#include <phonon.h>

namespace Ocean{
    namespace DOGsystem{
        class AudioBuffers{
            public:
            //Array data is stored contiguously.
            AudioBuffers(int numChannels =2, int numSamples =512);
            
            ~AudioBuffers();
            private:
            //for context
            IPLContext* ref_context = nullptr;

            //for audio
            IPLAudioBuffer buffer;
            IPLAudioBuffer tempBuffer;
            
        };
    }
}