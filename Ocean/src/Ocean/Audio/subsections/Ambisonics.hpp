#pragma once
#include <cstddef>
#include <phonon.h>
#include "HRTF.hpp"
#include "../steamaudio_dataholding.hpp"

namespace sonar{
    //useful for ambient sounds, or spatialized music.

    class Ambisonic{
        public:
        //must be given a max order.
            Ambisonic(const char* hrtf_name, int order = 1);
            //Will place the outbuffer on the global context.
            void encode(IPLVector3 vec,
                Ref<IPLAudioBuffer> inbuffer,
                IPLAudioBuffer* outbuffer);

            ~Ambisonic();
        private:
            IPLAmbisonicsDecodeEffect dedeffect = nullptr;

            const char* hrtf_name = nullptr;
            IPLAmbisonicsEncodeEffect* enceffect = nullptr;
            IPLAmbisonicsEncodeEffectSettings effectsetting{};

    };

}