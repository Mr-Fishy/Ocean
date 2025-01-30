#pragma once

#include <phonon.h>
#include "HRTF.hpp"
#include "Ocean/Audio/audio.hpp"
#include "convert.hpp"


namespace sonar{

    class Binaural{
        public:
    
        
        ~Binaural();
        
        static IPLBinauralEffect* effect;
        Ref<sonar::Binaural> Create(IPLAudioBuffer* inbuffer, IPLAudioBuffer* outbuffer);
        private:

        Binaural();


        IPLAudioSettings* audio_settings = sonar::HRTF::audio_settings;
        IPLHRTF* hrtf = sonar::HRTF::hrtf;
        IPLBinauralEffectSettings Binaural_settings;
        IPLContext* context_settings = steamaudio::context;

    };

}