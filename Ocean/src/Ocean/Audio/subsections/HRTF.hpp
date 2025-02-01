#pragma once
#include <phonon.h>
#include "../steamaudio_dataholding.hpp"
namespace sonar{
  
    class HRTF{
        HRTF();
        ~HRTF();

        private:
            global_audio_context* context = nullptr;
    };
}