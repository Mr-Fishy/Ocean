#pragma once
#include "Ocean/Primitives/Exceptions.hpp"
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
        std::unordered_map<const char*, Ref<IPLAudioBuffer>> inbuffers;
        std::unordered_map<const char*, Ref<IPLAudioBuffer>> outbuffer;
        
        std::unordered_map<const char*, Ref<IPLAudioBuffer>> tmpbuff;


        static std::unordered_map<const char*, Ref<sonar::HRTF>> hrtfs;
        
        static std::unordered_map<const char*, Ref<sonar::Binaural>> binaural;
        
    };



    //if there is error checking within the audio system, use this.
    static bool audioerror(IPLerror error){
        //if it errors return true
        switch(error){
            case IPL_STATUS_FAILURE:
                Ocean::Exception(Ocean::SYSTEM_ERROR,"General failure");
                return true;
                break;
            case IPL_STATUS_OUTOFMEMORY:
                Ocean::Exception(Ocean::SYSTEM_ERROR,"Out of memory");
                return true;
                break;
            case IPL_STATUS_INITIALIZATION:
                Ocean::Exception(Ocean::SYSTEM_ERROR,"Could not initialize external dependency");
                return true;
                break;

                
            default:
                return false;
        }
        return true;
    }

    
}