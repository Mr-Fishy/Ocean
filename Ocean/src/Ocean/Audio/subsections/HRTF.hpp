#pragma once
#include <phonon.h>
#include "Ocean/Types/Strings.hpp"
namespace sonar{
    class HRTF{
        public:
        explicit HRTF(int& sampling_rate_hz, int& frame_size, IPLHRTFType type =IPL_HRTFTYPE_DEFAULT, cstring file_path = "");
        ~HRTF();
        private:

        IPLHRTFSettings hrtf_settings;
        IPLAudioSettings* audio_settings;

    };
}