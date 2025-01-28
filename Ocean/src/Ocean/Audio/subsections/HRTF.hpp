#pragma once
#include <phonon.h>
#include "Ocean/Types/Strings.hpp"
#include "Ocean/Audio/audio.hpp"
#include "Ocean/Primitives/Exceptions.hpp"

namespace sonar{
    class HRTF{
        public:

        explicit HRTF(int& sampling_rate_hz, int& frame_size);
        explicit HRTF(int& sampling_rate_hz, int& frame_size, IPLHRTFType type =IPL_HRTFTYPE_DEFAULT, cstring file_path = "",const IPLuint8 *sofaData = nullptr, int sofadatasize =2);
        ~HRTF();

        static HRTF* GetInstance();

        static IPLHRTF* hrtf;

        static IPLHRTFSettings* hrtf_settings;
        static IPLAudioSettings* audio_settings;

        IPLHRTF* Initialize();
        private:
        IPLContext* context_settings = steamaudio::context;

    };
}