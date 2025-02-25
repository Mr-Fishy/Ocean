#pragma once
#include <phonon.h>
#include "../steamaudio_dataholding.hpp"

namespace sonar{
    //useful for ambient sounds, or spatialized music.

    class Ambisonic{
        public:
        //must be given a max order.
        Ambisonic(int n = 1);
        ~Ambisonic();
        private:

        IPLAmbisonicsEncodeEffectSettings effectsetting{};
        
    };

}