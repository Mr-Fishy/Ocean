#pragma once
#include <phonon.h>
#include "../steamaudio_dataholding.hpp"

namespace sonar{
    //per new spacialized sound source.
    class Binaural{
        public:
        Binaural(Ref<sonar::HRTF> hrtf,Ref<IPLAudioBuffer> inbuffer);
        ~Binaural();
        //gets the pointer for the class
        void spatialize(IPLBinauralEffectParams* params, IPLAudioBuffer* outbuffer);

        IPLBinauralEffect get_ptr(){return effect;}

        private:
            IPLBinauralEffectSettings effectsettings{};
            IPLAudioBuffer* buffer = nullptr;
            IPLBinauralEffect effect  =nullptr;
            IPLVector3 direction{0,0,0};

    };
}