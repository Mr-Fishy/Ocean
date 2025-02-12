#pragma once
#include <phonon.h>
#include "../steamaudio_dataholding.hpp"

namespace sonar{

    class Binaural{

        Binaural(Ref<sonar::HRTF> hrtf,IPLfloat32 spatialblend = 1, IPLVector3 direction = IPLVector3{0,0,0});
        ~Binaural();
            

        private:
           IPLVector3 direction{0,0,0};

    };
}