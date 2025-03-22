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

    class Ambisonic;

    struct global_audio_context{
        static sonar::steamaudio* audio;
        //in case i need to keep track of this stuff.
        OC_STATIC_INLINE std::unordered_map<const char*, Ref<IPLAudioBuffer>> buffers;
        OC_STATIC_INLINE std::unordered_map<const char*, Ref<IPLAudioBuffer>> inbuffers;
        OC_STATIC_INLINE std::unordered_map<const char*, Ref<IPLAudioBuffer>> outbuffer;
        
        OC_STATIC_INLINE std::unordered_map<const char*, Ref<IPLAudioBuffer>> tmpbuffer;


        static std::unordered_map<const char*, Ref<sonar::HRTF>> hrtfs;
        
        static std::unordered_map<const char*, Ref<sonar::Binaural>> binaural;

        static std::unordered_map<const char*, Ref<sonar::Ambisonic>> ambisonics;
        




        //TODO: THIS
        /*RIGHT IS DERIVED FROM THE CROSS PRODUCT OF THE FORWARD AND UP VECTORS
        FORWARD IS THE DIR THE LISTENER IS FACING
        UP IS THE DIRECTION CONSIDERED UP FOR THE LISTENER
        */
        //need to move this later, as there needs to be one per listener.
        //remember roll, pitch and yaw
        OC_STATIC_INLINE IPLCoordinateSpace3 coordinatespace = IPLCoordinateSpace3{
        //Rightis just the cross product of the up and ahead vectors
        IPLVector3{1,0,0},
        //the direction the listeners head is facing
        IPLVector3{0,1,0},
        //the Direction the listener is facing
        IPLVector3{0,0,-1},
        //origin in 3d space
        IPLVector3{0,0,0}};
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