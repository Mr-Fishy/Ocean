#pragma once
#include <phonon.h>
#include "../steamaudio_dataholding.hpp"

namespace sonar{

    class Binaural{

        Binaural();
        ~Binaural();

        private:
            global_audio_context* context = nullptr;
    };
}