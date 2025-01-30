#include "Ocean/Types/SmartPtrs.hpp"
#include "subsections/Binaural.hpp"
#include "subsections/HRTF.hpp"
#include <phonon.h>
#include <unordered_map>

namespace sonar{
    struct global_audio_context{

        IPLContextSettings* context;
        std::unordered_map<const char*, Ref<sonar::HRTF>> hrtfs;
        std::unordered_map<const char*, Ref<sonar::Binaural>> binaural;
        
    };
}