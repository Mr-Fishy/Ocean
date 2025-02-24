#pragma once
#include <phonon.h>
#include "../steamaudio_dataholding.hpp"

namespace sonar{
    //per new spacialized sound source.
    class Binaural{
        public:
        Binaural(std::string hrtfname);
        ~Binaural();
        //gets the pointer for the class
        void start(IPLBinauralEffectParams* params);

        IPLBinauralEffect get_ptr(){return effect;}

        private:
            IPLBinauralEffect effect  =nullptr;
            IPLVector3 direction{0,0,0};

    };
}