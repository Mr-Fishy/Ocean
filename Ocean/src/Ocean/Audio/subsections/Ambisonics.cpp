#pragma once
#include "Ambisonics.hpp"
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

void sonar::Ambisonic::encode(IPLVector3 vec,
    const char* inbuffername,
    IPLAudioBuffer outbuffer){
        
    IPLAudioBuffer* temp  = sonar::global_audio_context::inbuffers[inbuffername].get();
    outbuffer.numSamples= temp->numSamples;
    outbuffer.numChannels = pow((this->effectsetting.maxOrder+1),2);

}