#pragma once
#include "Ambisonics.hpp"
#include "Ocean/Primitives/Exceptions.hpp"
#include <cmath>
#include <cstdint>
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

    //more setup
    this->deeffectparams.order = this->effectsetting.maxOrder;
    this->deeffectparams.hrtf = sonar::global_audio_context::hrtfs[hrtf_name]->get_ptr();

    this->deeffectsetting.hrtf = this->deeffectparams.hrtf;
    this->deeffectsetting.maxOrder = this->deeffectparams.order;
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
    effectparams.order = effectsetting.maxOrder;
    effectparams.direction = this->direction_from_listener;
    iplAmbisonicsEncodeEffectApply(*this->enceffect, &this->effectparams, temp, outbuffer);

}
//yay it figures it out :)
void sonar::Ambisonic::decode(audiotype type, IPLCoordinateSpace3 *listener_orientation,Ref<IPLAudioBuffer> inbuffer, Ref<IPLAudioBuffer> outbuffer){
    //just selects the correct one.
    if(type == audiotype::binaural){
        this->decode(*listener_orientation, inbuffer, outbuffer);
    }else{
        this->decode(*listener_orientation,static_cast<IPLSpeakerLayoutType>(type),inbuffer, outbuffer);
    }
}
/*
find a better way to deal with this. (done)
*/
void sonar::Ambisonic::decode(IPLCoordinateSpace3 &listener_orientation,Ref<IPLAudioBuffer> inbuffer,Ref<IPLAudioBuffer> outbuffer){
    if(outbuffer.get()->numChannels != 2){
        Ocean::Exception(Ocean::Error::INVALID_ARGUMENT,"outbuffer must be stereo for this call.");
    }

    if(inbuffer.get()->numChannels != pow((this->effectsetting.maxOrder+1),2)){
        Ocean::Exception(Ocean::Error::INVALID_ARGUMENT, "inbuffer must be (n+1)^2 channels of input");
    }

    this->deeffectparams.orientation = listener_orientation;
    this->deeffectparams.binaural = IPL_TRUE;
    iplAmbisonicsDecodeEffectApply(this->dedeffect, &this->deeffectparams, inbuffer.get(), outbuffer.get());

}
            
//fix this (done maybe?) i forgot what to fix  :(
void sonar::Ambisonic::decode(IPLCoordinateSpace3 &listener_orientation,IPLSpeakerLayoutType layout,Ref<IPLAudioBuffer> inbuffer,Ref<IPLAudioBuffer> outbuffer){
    if(outbuffer.get()->numChannels != pow((this->effectsetting.maxOrder+1),2)){
        Ocean::Exception(Ocean::Error::INVALID_ARGUMENT, "inbuffer must be (n+1)^2 channels of input");
    }
    this->deeffectparams.orientation = listener_orientation;
    this->deeffectparams.binaural = IPL_FALSE;
    this->deeffectsetting.speakerLayout.type = layout;
    iplAmbisonicsDecodeEffectApply(this->dedeffect, &this->deeffectparams, inbuffer.get(), outbuffer.get());
}
