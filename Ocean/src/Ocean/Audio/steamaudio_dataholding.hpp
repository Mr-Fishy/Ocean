#pragma once
#include "Ocean/Primitives/Exceptions.hpp"
#include "Ocean/Primitives/Macros.hpp"
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
        //in case i need to keep track of this stuff.
        OC_STATIC_INLINE std::unordered_map<const char*, Ref<IPLAudioBuffer>> buffers;
        OC_STATIC_INLINE std::unordered_map<const char*, Ref<IPLAudioBuffer>> inbuffers;
        OC_STATIC_INLINE std::unordered_map<const char*, Ref<IPLAudioBuffer>> outbuffer;
        
        OC_STATIC_INLINE std::unordered_map<const char*, Ref<IPLAudioBuffer>> tmpbuffer;


        static std::unordered_map<const char*, Ref<sonar::HRTF>> hrtfs;
        
        static std::unordered_map<const char*, Ref<sonar::Binaural>> binaural;
        
    };



    //if there is error checking within the audio system, use this.
    static bool audioerror(IPLerror error){
        //if it errors return true
        switch(error){
            case IPL_STATUS_FAILURE:
                throw Ocean::Exception(Ocean::SYSTEM_ERROR,"General failure");
                return true;
                break;
            case IPL_STATUS_OUTOFMEMORY:
                throw Ocean::Exception(Ocean::SYSTEM_ERROR,"Out of memory");
                return true;
                break;
            case IPL_STATUS_INITIALIZATION:
                throw Ocean::Exception(Ocean::SYSTEM_ERROR,"Could not initialize external dependency");
                return true;
                break;

                
            default:
                return false;
        }
        return true;
    }

    
}