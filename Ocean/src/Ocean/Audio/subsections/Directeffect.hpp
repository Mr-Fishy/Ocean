#pragma once
#include <phonon.h>
#include "../steamaudio_dataholding.hpp"

namespace sonar{
    class DirectEffect{
        //selects what both buffers should be. (in and out)
        public:
        DirectEffect(Ref<sonar::HRTF> hrtf,int channels = 1);
        ~DirectEffect();
        private:

        IPLDirectEffect effect = nullptr;
        IPLDirectEffectSettings settings{};
        IPLDirectEffectParams params{};
        

    };
}