#include "HRTF.hpp"
#include "Ocean/Primitives/Exceptions.hpp"
#include "Ocean/Primitives/Log.hpp"
#include <phonon.h>

sonar::HRTF::HRTF(int& sampling_rate_hz, int& frame_size, IPLHRTFType type, cstring file_path,const IPLuint8 *sofaData, int sofadatasize, IPLHRTFNormType){
    audio_settings->samplingRate = sampling_rate_hz;
    audio_settings->frameSize = frame_size;

    //This will change on how the sofa functionality would work.
    hrtf_settings->type = type;
    if(type == IPL_HRTFTYPE_DEFAULT){
        throw (Ocean::Exception(Ocean::Error::INVALID_ARGUMENT, "Set custom type, but used default!"));
    }
    this->hrtf_settings->sofaData = sofaData;
    this->hrtf_settings->sofaDataSize = sofadatasize;
    this->Initialize();
}

sonar::HRTF::HRTF(int& sampling_rate_hz, int& frame_size){
    this->audio_settings->samplingRate = sampling_rate_hz;
    this->audio_settings->frameSize = frame_size;

    //This will change on how the sofa functionality would work.
    this->hrtf_settings->type = IPL_HRTFTYPE_DEFAULT;
    this->Initialize();
}
IPLHRTF* sonar::HRTF::Initialize(){
    switch(iplHRTFCreate(*context_settings, audio_settings, hrtf_settings, hrtf)){
        case IPLerror::IPL_STATUS_FAILURE:
        case IPLerror::IPL_STATUS_INITIALIZATION:
        case IPLerror::IPL_STATUS_OUTOFMEMORY:
        case IPLerror::IPL_STATUS_SUCCESS:
            oprint("Yay it work :)");
        break;

        default:
            throw(Ocean::Exception(Ocean::Error::INVALID_ARGUMENT,"Invalid argument: This shouldnt be possible!"));
        break;
    }

    return hrtf;
}

sonar::HRTF::~HRTF(){

}