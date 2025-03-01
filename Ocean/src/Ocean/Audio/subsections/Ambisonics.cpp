#pragma once
#include "Ambisonics.hpp"
#include "Ocean/Primitives/Exceptions.hpp"
#include <cmath>
#include <phonon.h>


sonar::Ambisonic::Ambisonic(const char* hrtf_name, int order){
    
    this->effectsetting.maxOrder = order;
    this->hrtf_name = hrtf_name;
    //I understand it now.
    iplAmbisonicsEncodeEffectCreate(
        *sonar::global_audio_context::audio->context, 
        sonar::global_audio_context::hrtfs[hrtf_name].get()->get_audiosettings(),
        &this->effectsetting, 
        this->enceffect);
}
//WILL ADD IT TO THE AMBISONIC
void sonar::Ambisonic::encode(IPLVector3 vec,
    Ref<IPLAudioBuffer> inbuffer,
    IPLAudioBuffer* outbuffer)
    {    
    IPLAudioBuffer* temp  = inbuffer.get();

    if(temp->numChannels>1){
        Ocean::Exception(Ocean::Error::INVALID_ARGUMENT, "Must be mono channel.");
    }
    
    outbuffer->numSamples= temp->numSamples;
    outbuffer->numChannels = pow((this->effectsetting.maxOrder+1),2);

}