#pragma once

#include <phonon.h>
#include "HRTF.hpp"
#include "Ocean/Audio/audio.hpp"
#include "convert.hpp"


namespace sonar{

    class Binaural{
        public:
    
        Binaural();
        ~Binaural();
        
        static IPLBinauralEffect* effect;
        void Initialize(IPLAudioBuffer* inbuffer, IPLAudioBuffer* outbuffer);
        private:

        IPLAudioSettings* audio_settings = sonar::HRTF::audio_settings;
        IPLHRTF* hrtf = sonar::HRTF::hrtf;
        IPLBinauralEffectSettings Binaural_settings;
        IPLContext* context_settings = steamaudio::context;

    };

}