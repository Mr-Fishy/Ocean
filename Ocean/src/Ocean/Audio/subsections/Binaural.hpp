#pragma once

#include <phonon.h>
#include "HRTF.hpp"


namespace sonar{

    class Binaural{
        public:
    
        Binaural();
        ~Binaural();

        static IPLBinauralEffect* effect;
        
        private:
        IPLAudioSettings* audio_settings = sonar::HRTF::audio_settings;
        IPLHRTF* hrtf = sonar::HRTF::hrtf;
        IPLBinauralEffectSettings Binaural_settings;
        IPLContext* context_settings = steamaudio::context;

    };

}