#pragma once
#include <phonon.h>
#include "HRTF.hpp"
#include "../steamaudio_dataholding.hpp"

namespace sonar{
    //useful for ambient sounds, or spatialized music.

    class Ambisonic{
        public:
        //must be given a max order.
        Ambisonic(Ref<sonar::HRTF> hrtf,int order = 1);
        
        ~Ambisonic();
        private:
        IPLAmbisonicsEncodeEffect* effect = nullptr;
        IPLAmbisonicsEncodeEffectSettings effectsetting{};

    };

}