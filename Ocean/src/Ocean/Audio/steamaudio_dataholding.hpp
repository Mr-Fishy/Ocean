#pragma once
#include "Ocean/Types/SmartPtrs.hpp"
#include "audio.hpp"
#include <phonon.h>
#include <unordered_map>

namespace sonar{
    //classes with no definitions.
    class HRTF;
    class Binaural;

    struct global_audio_context{
        static sonar::steamaudio* audio;
        std::unordered_map<const char*, Ref<sonar::HRTF>> hrtfs;
        std::unordered_map<const char*, Ref<sonar::Binaural>> binaural;
        
    };

    
}